import { writable } from 'svelte/store';

export const websocket_server_port = writable(3000);
export const websocket_server_connected = writable(false);

export const connected = writable(false);

window.api.onConnect(() => {
    connected.update(true);
});

window.api.onDisconnect(() => {
    connected.update(false);
});
