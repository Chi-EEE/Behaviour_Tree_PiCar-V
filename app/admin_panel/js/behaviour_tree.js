const { ipcMain } = require('electron');
const { websocket_server } = require('./websocket');
const { getStore } = require('./store')

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

function getBehaviourTreeList() {
    const store = getStore();
    return store.behaviour_tree_list;
}

/**
 * 
 * @param {any[]} behaviour_tree_list 
 */
function updateBehaviourTreeList(behaviour_tree_list) {
    const store = getStore();
    store.set("behaviour_tree_list", behaviour_tree_list);
}

async function saveBehaviourTree(_event, args) {
    const name = args.name;
    const code = args.code;
    let behaviour_tree_list = getBehaviourTreeList();
    updateBehaviourTreeList(behaviour_tree_list.concat({name: name, code: code}));
}

ipcMain.handle('sendBehaviourTree', sendBehaviourTree);
ipcMain.handle('startBehaviourTree', startBehaviourTree);
ipcMain.handle('stopBehaviourTree', stopBehaviourTree);

ipcMain.handle('getBehaviourTreeList', getBehaviourTreeList);
ipcMain.handle('saveBehaviourTree', saveBehaviourTree);

module.exports = {
    sendBehaviourTree: sendBehaviourTree,
    startBehaviourTree: startBehaviourTree,
    stopBehaviourTree: stopBehaviourTree,
    getBehaviourTreeList: getBehaviourTreeList,
    saveBehaviourTree: saveBehaviourTree,
}