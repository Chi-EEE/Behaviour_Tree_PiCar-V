//#ifndef ROOMMANAGER_HPP
//#define ROOMMANAGER_HPP
//
//#pragma once
//
//#include <atomic>
//#include <mutex>
//
//
//// https://stackoverflow.com/a/27977363
//class RoomManager {
//public:
//	static RoomManager* instance();
//
//
//private:
//	RoomManager() {}
//	static std::atomic<RoomManager*> pinstance;
//	static std::mutex m_;
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
	std::map<std::string, std::shared_ptr<Room>> rooms = {};
};

#endif