import { writable } from 'svelte/store';

export const websocket_server_port = writable(3000);

export const websocket_server_connected = writable(false);

export const raspberry_pi_uuid_list = writable([]);


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
    console.log(value);
});