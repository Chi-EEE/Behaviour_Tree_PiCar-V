const { ipcMain } = require('electron');
const { websocket_server } = require('./websocket');

function connectRaspberryPi(_event, args) {
    const uuid = args.uuid;
    if (websocket_server._raspberry_pi_clients.has(uuid)) {
        websocket_server._client = { uuid: uuid, ws: websocket_server._raspberry_pi_clients.get(uuid) };
        return { success: true, message: `Connected to Raspberry Pi with UUID: ${uuid}` };
    } else {
        return { success: false, message: `Raspberry Pi with UUID: ${uuid} not found` };
    }
}

function disconnectRaspberryPi(_event, _args) {
    websocket_server._client = undefined;
}

function connectRaspberryPi(_event, args) {
    const uuid = args.uuid;
    if (websocket_server._raspberry_pi_clients.has(uuid)) {
        websocket_server._client = websocket_server._raspberry_pi_clients.get(uuid);
        return { success: true, message: `Connected to Raspberry Pi with UUID: ${uuid}` };
    } else {
        return { success: false, message: `Raspberry Pi with UUID: ${uuid} not found` };
    }
}

function getRaspberryPi(_event, _args) {
    return { uuid: websocket_server._client.uuid }
}

ipcMain.handle('connectRaspberryPi', connectRaspberryPi);
ipcMain.handle('disconnectRaspberryPi', disconnectRaspberryPi);
ipcMain.handle('getRaspberryPi', getRaspberryPi);

module.exports = {
    connectRaspberryPi: connectRaspberryPi,
    disconnectRaspberryPi: disconnectRaspberryPi,
    getRaspberryPi: getRaspberryPi
}