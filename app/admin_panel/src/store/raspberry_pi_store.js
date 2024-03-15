import { writable } from 'svelte/store';

export const raspberry_pi_uuid_list = writable([]);

export const selected_raspberry_pi_uuid = writable("");

async function main() {
    raspberry_pi_uuid_list.set(await api.getRaspberryPiList());
    await api.unselectRaspberryPi();
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