#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "../room/RoomManager.hpp"
#include "../room/User.hpp"

using json = nlohmann::json;

class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
public:
#pragma region Message
    virtual void handleNewMessage(const drogon::WebSocketConnectionPtr&,
        std::string&&,
        const drogon::WebSocketMessageType&) override;
#pragma endregion

#pragma region User
    void handleUserMessage(const drogon::WebSocketConnectionPtr&,
        std::string&&,
        const drogon::WebSocketMessageType&);
    void handleUserCommand(json&, std::vector<std::string>&, std::shared_ptr<Room>&);
#pragma endregion

#pragma region Car
    void handleCarMessage(const drogon::WebSocketConnectionPtr&,
        std::string&&,
        const drogon::WebSocketMessageType&);
#pragma endregion

#pragma region General
    virtual void handleConnectionClosed(
        const drogon::WebSocketConnectionPtr&) override;

    virtual void handleNewConnection(const drogon::HttpRequestPtr&,
        const drogon::WebSocketConnectionPtr&) override;

    void handleCreateRequest(const drogon::HttpRequestPtr&,
        const drogon::WebSocketConnectionPtr&);

    void handleJoinRequest(const drogon::HttpRequestPtr&,
        const drogon::WebSocketConnectionPtr&);
#pragma endregion
    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws/room", drogon::Get);
    WS_PATH_LIST_END
private:
    drogon::PubSubService<std::string> chat_rooms;
};

void WebSocketChat::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
    std::string&& message,
    const drogon::WebSocketMessageType& type)
{
    if (type != drogon::WebSocketMessageType::Text) {
		return;
	}
    auto& user = wsConnPtr->getContextRef<User>();
    switch (user.getType()) {
    case UserType::User:
    {
        this->handleUserMessage(wsConnPtr, std::move(message), type);
        break;
    }
    case UserType::Car: {
        this->handleCarMessage(wsConnPtr, std::move(message), type);
        break;
    }
    }
}

void santize_name(std::string& name) {
    for (int i = 0; i < name.size(); i++) {
        if (name[i] < 'A' || name[i] > 'Z' && name[i] < 'a'
            || name[i] > 'z') {
            name.erase(i, 1);
            i--;
        }
    }
};

///// <summary>
///// </summary>
///// <param name="wsConnPtr"></param>
///// <param name="message"></param>
///// <param name="type"></param>
//void WebSocketChat::handleFirstMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
//    std::string&& message,
//    const drogon::WebSocketMessageType& type)
//{
//    spdlog::info("Received a first message from: {} | WebSocketChat::handleFirstMessage", wsConnPtr->peerAddr().toIp());
//    //auto& user = wsConnPtr->getContextRef<User>();
//    //if (type == drogon::WebSocketMessageType::Ping)
//    //{
//    //    auto message_json = json::parse(message);
//
//    //    std::string request = message_json["request"];
//    //    if (request == "create") {
//    //        this->handleCreateRequest(message_json, wsConnPtr);
//    //        return;
//    //    }
//    //    else if (request == "join") {
//    //        this->handleJoinRequest(message_json, wsConnPtr);
//    //        return;
//    //    }
//    //    /*std::string name = message_json["name"].get<std::string>();
//    //    santize_name(name);
//    //    name.resize(20);
//    //    user.set_name(name);
//    //    if (message_json["type"] == "user") {
//    //        user.set_type(UserType::User);
//    //        spdlog::info("{} is a user", wsConnPtr->peerAddr().toIp());
//    //    }
//    //    else if (message_json["type"] == "car") {
//    //        user.set_type(UserType::Car);
//    //        spdlog::info("{} is a car", wsConnPtr->peerAddr().toIp());
//    //    }*/
//    //    spdlog::error("Invalid request '{}' from {} | WebSocketChat::handleNewConnection", request, wsConnPtr->peerAddr().toIp());
//    //    wsConnPtr->forceClose();
//    //    return;
//    //}
//}

// https://stackoverflow.com/a/46931770
std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void WebSocketChat::handleUserMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
    std::string&& message,
    const drogon::WebSocketMessageType& type)
{
    auto& user = wsConnPtr->getContextRef<User>();
    spdlog::debug("Received a message from user: {} | WebSocketChat::handleUserMessage", wsConnPtr->peerAddr().toIp());
    try {
        json message_json = json::parse(message);
        const std::string message_data = message_json["data"].get<std::string>();

        auto& room = RoomManager::instance()->getRoom(user.getChatRoomName());

        json out_json;
        out_json["name"] = user.getName();
        bool is_command = !message_data.rfind("/", 0);
        if (is_command) {
            this->handleUserCommand(out_json, split(message_data.c_str(), ' '), room);
        }
        else {
            out_json["type"] = "message";
            out_json["data"] = message_data;
        }
        this->chat_rooms.publish(user.getChatRoomName(), out_json.dump());
    }
    catch (std::exception c) {
        spdlog::error("Invalid JSON from {} | WebSocketChat::handleUserMessage", wsConnPtr->peerAddr().toIp());
    }
}

void WebSocketChat::handleUserCommand(json& out_json, std::vector<std::string>& split_string, std::shared_ptr<Room>& room) {
    out_json["type"] = "command";
    std::string command_type = split_string[0];
    command_type.erase(0, 1); // Remove the slash
    if (command_type == "move") {
        out_json["command"] = "move";
        if (split_string.size() == 2) {
            int distance = std::atoi(split_string[1].c_str());
            out_json["distance"] = distance;
            if (room->getCarUser() != nullptr)
                room->getCarUser()->getConnection()->send(out_json.dump());
        }
        return;
    }
    else if (command_type == "turn") {

    }
    else if (command_type == "stop") {

    }
    else if (command_type == "speed") {

    }
}

void WebSocketChat::handleCarMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
    std::string&& message,
    const drogon::WebSocketMessageType& type)
{
    auto& user = wsConnPtr->getContextRef<User>();
    spdlog::debug("Received a message from car: {} | WebSocketChat::handleCarMessage", wsConnPtr->peerAddr().toIp());
    switch (type) {
    case drogon::WebSocketMessageType::Text:
        this->chat_rooms.publish(user.getChatRoomName(), message);
        break;
    }
}

void WebSocketChat::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
    spdlog::info("Connection closed from: {} | WebSocketChat::handleConnectionClosed", conn->peerAddr().toIp());
    if (conn->hasContext()) {
        auto& user = conn->getContextRef<User>();
        std::string room_name = user.getChatRoomName();

        auto& room = RoomManager::instance()->getRoom(room_name);
        room->removeUser(user);
        if (room->getUsers().empty()) {
            RoomManager::instance()->removeRoom(room_name);
        };

        this->chat_rooms.unsubscribe(room_name, user.getId());
    }
}

void WebSocketChat::handleNewConnection(const drogon::HttpRequestPtr& req,
    const drogon::WebSocketConnectionPtr& conn)
{
    spdlog::info("New connection from: {} | WebSocketChat::handleNewConnection", conn->peerAddr().toIp());
    std::string request = req->getParameter("request");
    if (request == "create") {
        this->handleCreateRequest(req, conn);
        return;
    }
    if (request == "join") {
        this->handleJoinRequest(req, conn);
        return;
    }
    spdlog::error("Invalid request '{}' from {} | WebSocketChat::handleNewConnection", request, conn->peerAddr().toIp());
    conn->forceClose();
}

/// <summary>
/// room_name
/// type
/// </summary>
/// <param name="req"></param>
/// <param name="conn"></param>
inline void WebSocketChat::handleCreateRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
    std::string room_name = req->getParameter("room_name");
    if (RoomManager::instance()->hasRoom(room_name)) {
        spdlog::error("Room {} already exists | WebSocketChat::handleCreateRequest", room_name);
        conn->forceClose();
        return;
    }
    spdlog::info("Creating room {} from {} | WebSocketChat::handleCreateRequest", room_name, req->peerAddr().toIp());
    auto user = std::make_shared<User>(
        this->chat_rooms.subscribe(room_name,
            [conn](const std::string& topic,
                const std::string& message) {
                    (void)topic;
                    conn->send(message);
            }),
        conn,
        room_name,
        UserType::User
    );
    auto room = std::make_shared<Room>(user);
    RoomManager::instance()->addRoom(room_name, room);
    conn->setContext(user);
}

inline void WebSocketChat::handleJoinRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
    std::string room_name = req->getParameter("room_name");
    if (!RoomManager::instance()->hasRoom(room_name)) {
        spdlog::error("Room {} does not exist | WebSocketChat::handleJoinRequest", room_name);
        conn->forceClose();
        return;
    }
    spdlog::info("Joining room {} from {} | WebSocketChat::handleJoinRequest", room_name, req->peerAddr().toIp());
    UserType type = UserType::User;
    if (req->getParameter("type") == "car") {
        type = UserType::Car;
    }
    auto user = std::make_shared<User>(
        this->chat_rooms.subscribe(room_name,
            [conn](const std::string& topic,
                const std::string& message) {
                    (void)topic;
                    conn->send(message);
            }),
        conn,
        room_name,
        type
    );
    auto room = RoomManager::instance()->getRoom(room_name);
    room->addUser(user);
    conn->setContext(user);
}