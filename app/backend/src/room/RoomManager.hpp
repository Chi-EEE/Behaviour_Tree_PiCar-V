#ifndef ROOMMANAGER_HPP
#define ROOMMANAGER_HPP

#pragma once

#include <map>
#include <memory>

#include "Room.hpp"

class RoomManager : public drogon::Plugin<RoomManager>
{
public:
	void initAndStart(const Json::Value& config) override {
	};

	void shutdown() override {
	};

	void createRoom(std::string name, std::shared_ptr<User> owner) {
		std::shared_ptr<Room> room = std::make_shared<Room>(owner);
		this->rooms[name] = room;
	}

	std::shared_ptr<Room> getRoom(std::string name) {
		return this->rooms[name];
	}

	void removeRoom(std::string name) {
		this->rooms.erase(name);
	}

	std::map<std::string, std::shared_ptr<Room>> getRooms() const {
		return this->rooms;
	}

	bool hasRoom(std::string name) {
		return this->rooms.find(name) != this->rooms.end();
	}

private:
	std::map<std::string, std::shared_ptr<Room>> rooms;
};

#endif