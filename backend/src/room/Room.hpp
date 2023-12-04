#ifndef ROOM_HPP
#define ROOM_HPP

#pragma once

#include <algorithm> // std::none_of
#include <vector>
#include <memory>

#include "User.hpp"

class Room
{
public:
	Room(std::shared_ptr<User> owner) : owner(owner) {
		this->users.push_back(owner);
	};

	void set_car_user(std::shared_ptr<User> car_user) {
		bool is_user_in_room = std::any_of(this->users.begin(), this->users.end(), [car_user](const std::shared_ptr<User>& user) {
			return user->get_id() == car_user->get_id();
			});
		if (is_user_in_room) {
			this->car_user = car_user;
		}
	}

	void add_user(std::shared_ptr<User> user) {
		this->users.push_back(user);
	}

	void remove_user(std::shared_ptr<User> user) {
		this->users.erase(std::remove_if(this->users.begin(), this->users.end(), [user](const std::shared_ptr<User>& u) {
			return u->get_id() == user->get_id();
			}), this->users.end());
	}

	std::vector<std::shared_ptr<User>> get_users() {
		return this->users;
	}

private:
	std::shared_ptr<User> owner;
	std::shared_ptr<User> car_user;
	std::vector<std::shared_ptr<User>> users;
};

#endif