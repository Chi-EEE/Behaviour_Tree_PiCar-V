import { writable } from 'svelte/store';

export const websocket_server_port = writable(3000);
export const websocket_server_connected = writable(false);