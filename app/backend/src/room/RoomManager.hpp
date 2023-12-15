#ifndef ROOMMANAGER_HPP
#define ROOMMANAGER_HPP

#pragma once

#include <atomic>
#include <mutex>
#include <map>
#include <memory>

#include "Room.hpp"

// https://stackoverflow.com/a/27977363
class RoomManager {
public:
	static RoomManager* instance();

	void addRoom(std::string name, std::shared_ptr<Room> room) {
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
	RoomManager() {}
	static std::atomic<RoomManager*> pinstance;
	static std::mutex m_;

	static std::map<std::string, std::shared_ptr<Room>> rooms;
};

std::atomic<RoomManager*> RoomManager::pinstance{ nullptr };
std::mutex RoomManager::m_;
std::map<std::string, std::shared_ptr<Room>> RoomManager::rooms;

RoomManager* RoomManager::instance() {
	if (pinstance == nullptr) {
		std::lock_guard<std::mutex> lock(m_);
		if (pinstance == nullptr) {
			pinstance = new RoomManager();
		}
	}
	return pinstance;
}

#endif