const { ipcMain } = require('electron');
const { websocket_server } = require('./websocket');

function unselectRaspberryPi(_event, _args) {
    websocket_server._selected_raspberry_pi = undefined;
}

function selectRaspberryPi(_event, args) {
    const uuid = args.uuid;
    return websocket_server.selectRaspberryPi(uuid);
}

function getRaspberryPiList(_event, _args) {
    return Array.from(websocket_server.getRaspberryPiMap().keys());
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