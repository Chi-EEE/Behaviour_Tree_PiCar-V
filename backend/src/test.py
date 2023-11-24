import asyncio
import websockets

async def send_messages(uri, messages):
    async with websockets.connect(uri) as websocket:
        for message in messages:
            print(f"Sending message: {message}")
            await websocket.send(message)
            response = await websocket.recv()
            print(f"Received response: {response}")

if __name__ == "__main__":
    # Replace 'ws://example.com/socket' with your WebSocket server URI
    websocket_uri = "ws://localhost:8848/chat"

    # Replace ['message1', 'message2', ...] with the messages you want to send
    messages_to_send = ['Hello', 'How are you?', 'WebSocket test']

    asyncio.get_event_loop().run_until_complete(send_messages(websocket_uri, messages_to_send))