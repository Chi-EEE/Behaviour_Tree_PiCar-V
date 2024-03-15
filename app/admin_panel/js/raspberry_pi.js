const { ipcMain } = require('electron');
const { websocket_server } = require('./websocket');

function selectRaspberryPi(_event, args) {
    const uuid = args.uuid;
    if (websocket_server._raspberry_pi_map.has(uuid)) {
        websocket_server._selected_raspberry_pi = { uuid: uuid, ws: websocket_server._raspberry_pi_map.get(uuid) };
        return { success: true, message: `Connected to Raspberry Pi with UUID: ${uuid}` };
    } else {
        return { success: false, message: `Raspberry Pi with UUID: ${uuid} not found` };
    }
}

function unselectRaspberryPi(_event, _args) {
    websocket_server._selected_raspberry_pi = undefined;
}

function selectRaspberryPi(_event, args) {
    const uuid = args.uuid;
    if (websocket_server._raspberry_pi_map.has(uuid)) {
        websocket_server._selected_raspberry_pi = websocket_server._raspberry_pi_map.get(uuid);
        return { success: true, message: `Connected to Raspberry Pi with UUID: ${uuid}` };
    } else {
        return { success: false, message: `Raspberry Pi with UUID: ${uuid} not found` };
    }
}

function getRaspberryPiList(_event, _args) {
    return { clients: Array.from(websocket_server.getRaspberryPiMap().keys()) };
}

function getSelectedRaspberryPi(_event, _args) {
    return { uuid: websocket_server.getSelectedRaspberryPi().uuid }
}

ipcMain.handle('selectRaspberryPi', selectRaspberryPi);
ipcMain.handle('unselectRaspberryPi', unselectRaspberryPi);
ipcMain.handle('getRaspberryPiList', getRaspberryPiList);
ipcMain.handle('getSelectedRaspberryPi', getSelectedRaspberryPi);

module.exports = {
    selectRaspberryPi: selectRaspberryPi,
    unselectRaspberryPi: unselectRaspberryPi,
    getRaspberryPiList: getRaspberryPiList,
    getSelectedRaspberryPi: getSelectedRaspberryPi
}