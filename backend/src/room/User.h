#ifndef USER_H
#define USER_H

#pragma once

#include <drogon/PubSubService.h>

#include "UserType.h"

class User
{
public:
	User(drogon::SubscriberID id, std::string chat_room_name);
	~User();

	void set_type(UserType type) { this->type = type; }

	UserType get_type() const { return this->type; }
	std::string get_chat_room_name() const { return this->chat_room_name; }
	drogon::SubscriberID get_id() const { return this->id; }
private:
	UserType type = UserType::Unknown;
	const std::string chat_room_name;
	const drogon::SubscriberID id;
};

#endif