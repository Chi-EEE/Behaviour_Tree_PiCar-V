#ifndef USER_HPP
#define USER_HPP

#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>

#include "UserType.hpp"

class User
{
public:
	User(drogon::SubscriberID id, drogon::WebSocketConnectionPtr conn, std::string chat_room_name, UserType type) : id(id), connection(conn), chat_room_name(chat_room_name), type(type) {};

	UserType getType() const { return this->type; }
	std::string getChatRoomName() const { return this->chat_room_name; }
	drogon::SubscriberID getId() const { return this->id; }

	void setName(std::string name) { this->name = name; }
	std::string getName() const { return this->name; }

	const drogon::WebSocketConnectionPtr getConnection() const { return this->connection; }

private:
	const UserType type;
	const std::string chat_room_name;
	const drogon::SubscriberID id;
	const drogon::WebSocketConnectionPtr connection;

	std::string name;
};

#endif