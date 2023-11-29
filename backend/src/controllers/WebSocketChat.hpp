#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>
#include "../room/User.h"

#include <nlohmann/json.hpp>
//#include <nlohmann/json-schema.hpp>

using json = nlohmann::json;
//using json_validator = nlohmann::json_schema::json_validator;

class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
public:
	virtual void handleNewMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&) override;
	virtual void handleConnectionClosed(
		const drogon::WebSocketConnectionPtr&) override;
	virtual void handleNewConnection(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&) override;
	WS_PATH_LIST_BEGIN
		WS_PATH_ADD("/chat", drogon::Get);
	WS_PATH_LIST_END
private:
	drogon::PubSubService<std::string> chat_rooms;
};

void WebSocketChat::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& subscriber = wsConnPtr->getContextRef<User>();
	if (subscriber.type_ == SubscriberType::Unknown) {
		if (type == drogon::WebSocketMessageType::Ping)
		{
			auto j = json::parse(message);
			if (j["type"] == "user") {
				subscriber.type_ = SubscriberType::User;
				spdlog::debug("Received a ping from user: {} | WebSocketChat::handleNewMessage", wsConnPtr->peerAddr().toIp());
			}
			else if (j["type"] == "car") {
				subscriber.type_ = SubscriberType::Car;
				spdlog::debug("Received a ping from car: {} | WebSocketChat::handleNewMessage", wsConnPtr->peerAddr().toIp());
				LOG_DEBUG << "";
			}
		}
		return;
	}
	switch (type) {
	case drogon::WebSocketMessageType::Text:
		chat_rooms.publish(s.chatRoomName_, message);
		break;
	}
	/*if (type == drogon::WebSocketMessageType::Ping)
	{

		s.type_ = SubscriberType::User;
		LOG_DEBUG << "recv a ping";
		LOG_DEBUG << "new websocket message:" << message;
	}
	else if (type == drogon::WebSocketMessageType::Text)
	{
		auto& s = wsConnPtr->getContextRef<Subscriber>();
		chatRooms_.publish(s.chatRoomName_, message);
	}*/
}

void WebSocketChat::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
	LOG_DEBUG << "websocket closed!";
	auto& user = conn->getContextRef<User>();
	chat_rooms.unsubscribe(, user.id());
}

void WebSocketChat::handleNewConnection(const drogon::HttpRequestPtr& req,
	const drogon::WebSocketConnectionPtr& conn)
{
	LOG_DEBUG << "new websocket connection!";
	conn->send("haha!!!");
	User user(
		chat_rooms.subscribe(req->getParameter("room_name"),
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		req->getParameter("room_name")
	);
	conn->setContext(std::make_shared<User>(std::move(user)));
}