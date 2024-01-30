#ifndef ROOM_HPP
#define ROOM_HPP

#pragma once

#include <algorithm> // std::none_of
#include <vector>
#include <memory>

#include "../user/User.hpp"

class Room
{
public:
	Room(std::shared_ptr<User> owner) : owner(owner)
	{
		this->users.push_back(owner);
	};

	std::shared_ptr<User> getCarUser() const
	{
		return this->car_user;
	}

	void addUser(std::shared_ptr<User> user)
	{
		this->users.push_back(user);
		if (user->getType() == UserType::Car)
		{
			this->setCarUser(user);
		}
	}

	void removeUser(User user)
	{
		this->users.erase(std::remove_if(this->users.begin(), this->users.end(), [user](const std::shared_ptr<User> &user_ptr)
										 { return user_ptr->getId() == user.getId(); }),
						  this->users.end());
		// No need to check if owner is in room, because if owner leaves, the room is deleted anyways after the last user leaves
		if (this->owner->getId() == user.getId())
		{
			if (!this->isEmpty())
				this->owner = this->users[0];
			else
				this->owner = nullptr;
		}
		if (this->car_user != nullptr && this->car_user->getId() == user.getId())
		{
			this->car_user = nullptr;
		}
	}

	int getSize() const
	{
		return this->users.size();
	}

	bool isEmpty() const
	{
		return this->users.size() == 0;
	}

private:
	// Assume that the user is in the room
	void setCarUser(std::shared_ptr<User> car_user)
	{
		this->car_user = car_user;
	}

private:
	std::shared_ptr<User> owner;
	std::shared_ptr<User> car_user;
	std::vector<std::shared_ptr<User>> users;
};

#endif