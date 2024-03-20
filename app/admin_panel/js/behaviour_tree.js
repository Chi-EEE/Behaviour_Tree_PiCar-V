const { ipcMain } = require('electron');
const { websocket_server } = require('./websocket');

function sendBehaviourTree(_event, args) {
    const behaviour_tree = args.data;
    websocket_server.sendBehaviourTree(behaviour_tree);
}

function startBehaviourTree(_event, _args) {
    websocket_server.startBehaviourTree();
}

function stopBehaviourTree(_event, _args) {
    websocket_server.stopBehaviourTree()
}

ipcMain.handle('sendBehaviourTree', sendBehaviourTree);
ipcMain.handle('startBehaviourTree', startBehaviourTree);
ipcMain.handle('stopBehaviourTree', stopBehaviourTree);

module.exports = {
    sendBehaviourTree: sendBehaviourTree,
    startBehaviourTree: startBehaviourTree,
    stopBehaviourTree: stopBehaviourTree,
}