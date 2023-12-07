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

	void setCarUser(std::shared_ptr<User> car_user) {
		bool is_user_in_room = std::any_of(this->users.begin(), this->users.end(), [car_user](const std::shared_ptr<User>& user) {
			return user->getId() == car_user->getId();
			});
		if (is_user_in_room) {
			this->car_user = car_user;
		}
	}

	std::shared_ptr<User> getCarUser() const {
		return this->car_user;
	}

	void addUser(std::shared_ptr<User> user) {
		this->users.push_back(user);
	}

	void removeUser(User user) {
		this->users.erase(std::remove_if(this->users.begin(), this->users.end(), [user](const std::shared_ptr<User>& user_ptr) {
			return user_ptr->getId() == user.getId();
			}
		), this->users.end());
		// TODO: Check if owner / car_user has left the room
	}

	std::vector<std::shared_ptr<User>> getUsers() {
		return this->users;
	}

private:
	std::shared_ptr<User> owner;
	std::shared_ptr<User> car_user;
	std::vector<std::shared_ptr<User>> users;
};

#endif