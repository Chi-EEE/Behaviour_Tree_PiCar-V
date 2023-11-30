#ifndef VEHICLEUSER_H
#define VEHICLEUSER_H

#pragma once

#include "User.h"

class VehicleUser : public User
{
public:
    VehicleUser(drogon::SubscriberID id, std::string chat_room_name);
    ~VehicleUser();
    UserType get_type() const override { return UserType::Car; }
private:

};

#endif