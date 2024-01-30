#ifndef ROOM_WEBSOCKET_H
#define ROOM_WEBSOCKET_H

#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "utils/Utility.hpp"

#include "behaviour_tree/BehaviourTreeParser.hpp"

#include "../room/RoomManager.hpp"

#include "../room/Room.hpp"
#include "../user/User.hpp"

class RoomWebSocket : public drogon::WebSocketController<RoomWebSocket>
{
public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/ws/room", drogon::Get);
    WS_PATH_LIST_END;

    virtual void handleNewMessage(const drogon::WebSocketConnectionPtr &,
                                  std::string &&,
                                  const drogon::WebSocketMessageType &) override;

    virtual void handleNewConnection(const drogon::HttpRequestPtr &,
                                     const drogon::WebSocketConnectionPtr &) override;

    virtual void handleConnectionClosed(
        const drogon::WebSocketConnectionPtr &) override;

    void handleCreateRequest(const drogon::HttpRequestPtr &,
                             const drogon::WebSocketConnectionPtr &,
                             const std::string &encoded_room_name);

    void handleJoinRequest(const drogon::HttpRequestPtr &,
                           const drogon::WebSocketConnectionPtr &,
                           const std::string &encoded_room_name);

#pragma region Default User
    void handleUserMessage(const drogon::WebSocketConnectionPtr &,
                           std::string &&,
                           const drogon::WebSocketMessageType &);
    void handleBehaviourTree(const drogon::WebSocketConnectionPtr &wsConnPtr, const std::string &message_data, User &user, std::shared_ptr<Room> &room);
#pragma endregion

#pragma region Car User
    void handleCarMessage(const drogon::WebSocketConnectionPtr &,
                          std::string &&,
                          const drogon::WebSocketMessageType &);
#pragma endregion

private:
    drogon::PubSubService<std::string> chat_rooms;
};

#endif // !ROOM_WEBSOCKET_H