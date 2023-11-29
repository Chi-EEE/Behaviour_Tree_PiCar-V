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
	virtual UserType type() const { return UserType::User; }
	std::string chat_room_name() const { return chat_room_name; }
	drogon::SubscriberID id() const { return id; }
private:
	std::string chat_room_name;
	drogon::SubscriberID id;
};

#endif