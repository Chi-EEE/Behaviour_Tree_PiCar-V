import { writable } from 'svelte/store';

export const raspberry_pi_uuid_list = writable([]);

export const selected_raspberry_pi_uuid = writable("");

async function main() {
    raspberry_pi_uuid_list.set(await api.getRaspberryPiList());
    await api.unselectRaspberryPi();
}
main();