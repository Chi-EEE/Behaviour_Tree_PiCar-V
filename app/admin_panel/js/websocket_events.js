const { ipcMain } = require('electron');
const tcpPortUsed = require('tcp-port-used');

const { websocket_server } = require('./websocket');

async function startWebSocketServer(_event, args) {
    if (await tcpPortUsed.check(args.port)) {
        return { success: false, message: `Port ${args.port} is already in use` };
    }
    try {
        websocket_server.connect(args.port);
        websocket_server.startConnections();
        return { success: true, code: websocket_server.generateCode() };
    } catch (error) {
        return { success: false, message: `Unable to start WebSocket Server, Error: ${error}` };
    }
}

function closeWebSocketServer(_event, _args) {
    websocket_server.close();
}

function getWebSocketServer(_event, _args) {
    if (websocket_server.isConnected()) {
        return { success: true, code: websocket_server.getCode(), port: websocket_server.getPort() };
    }
    else {
        return { success: false };
    }
}

ipcMain.handle('startWebSocketServer', startWebSocketServer);
ipcMain.handle('closeWebSocketServer', closeWebSocketServer);
ipcMain.handle('getWebSocketServer', getWebSocketServer);

module.exports = {
    startWebSocketServer: startWebSocketServer,
    closeWebSocketServer: closeWebSocketServer,
    getWebSocketServer: getWebSocketServer
}