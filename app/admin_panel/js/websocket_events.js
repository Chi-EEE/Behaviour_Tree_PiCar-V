const { ipcMain } = require('electron');
const tcpPortUsed = require('tcp-port-used');

const { websocket_server } = require('./websocket');

async function startWebSocket(_event, args) {
    if (await tcpPortUsed.check(args.port)) {
        return { success: false, message: `Port ${args.port} is already in use` };
    }
    try {
        websocket_server.connect(args.port);
        websocket_server.createConnection();
        return { success: true, code: websocket_server.generateCode() };
    } catch (error) {
        return { success: false, message: `Unable to start WebSocket Server, Error: ${error}` };
    }
}

function closeWebSocket(_event, _args) {
    websocket_server.close();
}

function getWebSocket(_event, _args) {
    if (websocket_server._wss !== undefined) {
        return { success: true, code: websocket_server.getCode(), port: websocket_server._port };
    }
    else {
        return { success: false };
    }
}

ipcMain.handle('startWebSocketServer', startWebSocket);
ipcMain.handle('closeWebSocketServer', closeWebSocket);
ipcMain.handle('getWebSocketServer', getWebSocket);

module.exports = {
    startWebSocket: startWebSocket,
    closeWebSocket: closeWebSocket,
    getWebSocket: getWebSocket
}