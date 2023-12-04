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

	void add_room(std::string name, std::shared_ptr<Room> room) {
		rooms[name] = room;
	}

	std::shared_ptr<Room> get_room(std::string name) {
		return rooms[name];
	}

	void remove_room(std::string name) {
		rooms.erase(name);
	}

	std::map<std::string, std::shared_ptr<Room>> get_rooms() {
		return rooms;
	}

	bool room_exists(std::string name) {
		return rooms.find(name) != rooms.end();
	}

private:
	RoomManager() {}
	static std::atomic<RoomManager*> pinstance;
	static std::mutex m_;

	static std::map<std::string, std::shared_ptr<Room>> rooms;
};

std::atomic<RoomManager*> RoomManager::pinstance{ nullptr };
std::mutex RoomManager::m_;

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