import { writable } from 'svelte/store';

export const websocket_server_connection_state = writable(false);
export const websocket_server_port = writable(3000);
export const websocket_server_code = writable(-1);

async function main() {
    const websocket_server_result = await api.getWebSocketServer();
    if (websocket_server_result.success) {
        websocket_server_connection_state.set(true);
        websocket_server_port.set(websocket_server_result.port);
        websocket_server_code.set(websocket_server_result.code);
    }
}
main();

window.api.onConnection((value) => {
    value = JSON.parse(value);
    console.log(value);
    raspberry_pi_uuid_list.update((list) => {
        list.push(value.uuid);
        return list;
    });
});

window.api.onDisconnection((value) => {
    value = JSON.parse(value);
    console.log(value);
    raspberry_pi_uuid_list.update((list) => {
        list = list.filter((uuid) => uuid !== value.uuid);
        return list;
    });
});

window.api.onMessage((value) => {
    value = JSON.parse(value);
});