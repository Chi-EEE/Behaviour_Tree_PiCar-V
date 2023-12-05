#ifndef USER_HPP
#define USER_HPP

#pragma once

#include <drogon/PubSubService.h>

#include "UserType.hpp"

class User
{
public:
	User(drogon::SubscriberID id, std::string chat_room_name, drogon::WebSocketConnectionPtr conn) : id(id), connection(conn), chat_room_name(chat_room_name) {};

	void set_type(UserType type) { this->type = type; }

	UserType get_type() const { return this->type; }
	std::string get_chat_room_name() const { return this->chat_room_name; }
	drogon::SubscriberID get_id() const { return this->id; }

	void set_name(std::string name) { this->name = name; }
	std::string get_name() const { return this->name; }

	const drogon::WebSocketConnectionPtr get_connection() const { return this->connection; }

private:
	UserType type = UserType::Unknown;
	const std::string chat_room_name;
	const drogon::SubscriberID id;
	const drogon::WebSocketConnectionPtr connection;

	std::string name;
};

#endif