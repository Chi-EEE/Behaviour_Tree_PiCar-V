const os = require('os')
const { ipcMain } = require('electron');

function getLocalIPList(_event, _args) {
    return os.networkInterfaces();
}

ipcMain.handle('getLocalIPList', getLocalIPList);

module.exports = {
    getLocalIPList: getLocalIPList
};