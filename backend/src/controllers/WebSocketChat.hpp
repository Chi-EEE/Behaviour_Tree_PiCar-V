 #pragma once

 #include <drogon/WebSocketController.h>
 #include <drogon/PubSubService.h>
 #include <drogon/HttpAppFramework.h>

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
     drogon::PubSubService<std::string> chatRooms_;
 };

 struct Subscriber
 {
     std::string chatRoomName_;
     drogon::SubscriberID id_;
 };

 void WebSocketChat::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
     std::string&& message,
     const drogon::WebSocketMessageType& type)
 {
     // write your application logic here
     LOG_DEBUG << "new websocket message:" << message;
     if (type == drogon::WebSocketMessageType::Ping)
     {
         LOG_DEBUG << "recv a ping";
     }
     else if (type == drogon::WebSocketMessageType::Text)
     {
         auto& s = wsConnPtr->getContextRef<Subscriber>();
         chatRooms_.publish(s.chatRoomName_, message);
     }
 }

 void WebSocketChat::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
 {
     LOG_DEBUG << "websocket closed!";
     auto& s = conn->getContextRef<Subscriber>();
     chatRooms_.unsubscribe(s.chatRoomName_, s.id_);
 }

 void WebSocketChat::handleNewConnection(const drogon::HttpRequestPtr& req,
                                        const drogon::WebSocketConnectionPtr &conn)
{
    LOG_DEBUG << "new websocket connection!";
    conn->send("haha!!!");
    Subscriber s;
    s.chatRoomName_ = req->getParameter("room_name");
    s.id_ = chatRooms_.subscribe(s.chatRoomName_,
                                [conn](const std::string &topic,
                                        const std::string &message) {
                                    //Supress unused variable warning
                                    (void)topic;
                                    conn->send(message);
                                });
    conn->setContext(std::make_shared<Subscriber>(std::move(s)));
}