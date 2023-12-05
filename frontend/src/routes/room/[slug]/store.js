import { readable } from "svelte/store";

export const socketStore = readable({}, set => {
    const socket = new WebSocket("ws://localhost:8000/ws/ping");

    socket.addEventListener("message", function (event) {
        const data = JSON.parse(event.data);
        set(data);
    });

    return () => { socket.close() };
});