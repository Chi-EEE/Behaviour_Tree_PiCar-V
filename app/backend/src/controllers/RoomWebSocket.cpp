#include "RoomWebSocket.h"

void RoomWebSocket::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	if (type != drogon::WebSocketMessageType::Text) {
		return;
	}
	auto& user = wsConnPtr->getContextRef<User>();
	switch (user.getType()) {
	case UserType::Default:
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

void RoomWebSocket::handleNewConnection(const drogon::HttpRequestPtr& req,
	const drogon::WebSocketConnectionPtr& conn)
{
	spdlog::info("New connection from: {} | RoomWebSocket::handleNewConnection", conn->peerAddr().toIp());
	std::string request = req->getParameter("request");
	if (request == "create") {
		this->handleCreateRequest(req, conn);
		return;
	}
	if (request == "join") {
		this->handleJoinRequest(req, conn);
		return;
	}
	spdlog::error("Invalid request '{}' from {} | RoomWebSocket::handleNewConnection", request, conn->peerAddr().toIp());
	conn->forceClose();
}

void RoomWebSocket::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
	spdlog::info("Connection closed from: {} | RoomWebSocket::handleConnectionClosed", conn->peerAddr().toIp());
	if (conn->hasContext()) {
		auto& user = conn->getContextRef<User>();
		std::string room_name = user.getChatRoomName();

		RoomManager* room_manager = drogon::app().getPlugin<RoomManager>();
		auto& room = room_manager->getRoom(room_name);
		room->removeUser(user);
		if (room->isEmpty()) {
			room_manager->removeRoom(room_name);
		};

		this->chat_rooms.unsubscribe(room_name, user.getId());
	}
}

inline void RoomWebSocket::handleCreateRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	std::string room_name = req->getParameter("room_name");
	if (room_name.size() < 3) {
		spdlog::info("Room name {} is too short | RoomWebSocket::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	utils::Utility::encode(room_name);
	RoomManager* room_manager = drogon::app().getPlugin<RoomManager>();
	if (room_manager->hasRoom(room_name)) {
		spdlog::error("Room {} already exists | RoomWebSocket::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	spdlog::info("Creating room {} from {} | RoomWebSocket::handleCreateRequest", room_name, req->peerAddr().toIp());
	auto user = std::make_shared<User>(
		this->chat_rooms.subscribe(room_name,
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		conn,
		room_name,
		UserType::Default
	);
	room_manager->createRoom(room_name, user);
	conn->setContext(user);
}

inline void RoomWebSocket::handleJoinRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	std::string room_name = req->getParameter("room_name");
	if (room_name.size() < 3) {
		spdlog::info("Room name {} is too short | RoomWebSocket::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	utils::Utility::encode(room_name);
	RoomManager* room_manager = drogon::app().getPlugin<RoomManager>();
	if (!room_manager->hasRoom(room_name)) {
		spdlog::error("Room {} does not exist | RoomWebSocket::handleJoinRequest", room_name);
		conn->forceClose();
		return;
	}
	spdlog::info("Joining room {} from {} | RoomWebSocket::handleJoinRequest", room_name, req->peerAddr().toIp());
	UserType type = UserType::Default;
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
	auto room = room_manager->getRoom(room_name);
	room->addUser(user);
	if (type == UserType::Car) {
		room->setCarUser(user);
	}
	conn->setContext(user);
}