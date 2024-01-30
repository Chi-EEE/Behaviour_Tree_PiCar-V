#include "RoomWebSocket.h"

void RoomWebSocket::handleNewMessage(const drogon::WebSocketConnectionPtr &wsConnPtr,
                                     std::string &&message,
                                     const drogon::WebSocketMessageType &type)
{
    if (type != drogon::WebSocketMessageType::Text)
    {
        return;
    }
    auto &user = wsConnPtr->getContextRef<User>();
    switch (user.getType())
    {
    case UserType::Default:
    {
        this->handleUserMessage(wsConnPtr, std::move(message), type);
        break;
    }
    case UserType::Car:
    {
        this->handleCarMessage(wsConnPtr, std::move(message), type);
        break;
    }
    }
}

void RoomWebSocket::handleNewConnection(const drogon::HttpRequestPtr &req,
                                        const drogon::WebSocketConnectionPtr &conn)
{
    spdlog::info("New connection from: {} | RoomWebSocket::handleNewConnection", conn->peerAddr().toIp());

    // Since this is the RoomWebSocket, we expect the request to have a room_name parameter
    std::string room_name = req->getParameter("room_name");
    if (room_name.size() < 3)
    {
        spdlog::info("Room name {} is too short | RoomWebSocket::handleNewConnection", room_name);
        conn->forceClose();
        return;
    }
    // Encode after so that special characters are counted in the length
    utils::Utility::encode(room_name);

    const std::string request = req->getParameter("request");
    switch (utils::Utility::hash(request))
    {
    case utils::Utility::hash("create"):
    {
        this->handleCreateRequest(req, conn, room_name);
        break;
    }

    case utils::Utility::hash("join"):
    {
        this->handleJoinRequest(req, conn, room_name);
        break;
    }
    default:
    {
        spdlog::error("Invalid request '{}' from {} | RoomWebSocket::handleNewConnection", request, conn->peerAddr().toIp());
        conn->send("Invalid request");
        conn->forceClose();
    }
    }
}

void RoomWebSocket::handleConnectionClosed(const drogon::WebSocketConnectionPtr &conn)
{
    spdlog::info("Connection closed from: {} | RoomWebSocket::handleConnectionClosed", conn->peerAddr().toIp());
    if (conn->hasContext())
    {
        auto &user = conn->getContextRef<User>();
        std::string room_name = user.getChatRoomName();

        RoomManager *room_manager = drogon::app().getPlugin<RoomManager>();
        auto &room = room_manager->getRoom(room_name);
        room->removeUser(user);
        if (room->isEmpty())
        {
            room_manager->removeRoom(room_name);
        };

        this->chat_rooms.unsubscribe(room_name, user.getId());
    }
}

inline void RoomWebSocket::handleCreateRequest(const drogon::HttpRequestPtr &req, const drogon::WebSocketConnectionPtr &conn, const std::string &encoded_room_name)
{
    RoomManager *room_manager = drogon::app().getPlugin<RoomManager>();
    if (room_manager->hasRoom(encoded_room_name))
    {
        spdlog::error("Room {} already exists | RoomWebSocket::handleCreateRequest", encoded_room_name);
        conn->send("Room already exists");
        conn->forceClose();
        return;
    }
    spdlog::info("Creating room {} from {} | RoomWebSocket::handleCreateRequest", encoded_room_name, req->peerAddr().toIp());
    auto user = std::make_shared<User>(
        this->chat_rooms.subscribe(encoded_room_name,
                                   [conn](const std::string &topic,
                                          const std::string &message)
                                   {
                                       (void)topic;
                                       conn->send(message);
                                   }),
        conn,
        encoded_room_name,
        UserType::Default);
    room_manager->createRoom(encoded_room_name, user);
    conn->setContext(user);
}

inline void RoomWebSocket::handleJoinRequest(const drogon::HttpRequestPtr &req, const drogon::WebSocketConnectionPtr &conn, const std::string &encoded_room_name)
{
    RoomManager *room_manager = drogon::app().getPlugin<RoomManager>();
    if (!room_manager->hasRoom(encoded_room_name))
    {
        spdlog::error("Room {} does not exist | RoomWebSocket::handleJoinRequest", encoded_room_name);
        conn->send("Room does not exist");
        conn->forceClose();
        return;
    }
    spdlog::info("Joining room {} from {} | RoomWebSocket::handleJoinRequest", encoded_room_name, req->peerAddr().toIp());
    UserType type = UserType::Default;
    // TODO: Add a password for this
    if (req->getParameter("type") == "car")
    {
        type = UserType::Car;
    }
    std::shared_ptr<User> user = std::make_shared<User>(
        this->chat_rooms.subscribe(encoded_room_name,
                                   [conn](const std::string &topic,
                                          const std::string &message)
                                   {
                                       (void)topic;
                                       conn->send(message);
                                   }),
        conn,
        encoded_room_name,
        type);
    auto room = room_manager->getRoom(encoded_room_name);
    room->addUser(user);
    conn->setContext(user);
}