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
    return store.get("behaviour_tree_list");
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
    const uuid = crypto.randomUUID();
    const name = args.name;
    const code = args.code;
    let behaviour_tree_list = getBehaviourTreeList();
    updateBehaviourTreeList(behaviour_tree_list.concat({uuid: uuid, name: name, code: code}));
}

async function removeBehaviourTree(_event, args) {
    const uuid = args.uuid;
    let behaviour_tree_list = getBehaviourTreeList();
    behaviour_tree_list = behaviour_tree_list.filter((behaviour_tree) => behaviour_tree.uuid !== uuid);
    updateBehaviourTreeList(behaviour_tree_list);
}

ipcMain.handle('sendBehaviourTree', sendBehaviourTree);
ipcMain.handle('startBehaviourTree', startBehaviourTree);
ipcMain.handle('stopBehaviourTree', stopBehaviourTree);

ipcMain.handle('getBehaviourTreeList', getBehaviourTreeList);
ipcMain.handle('saveBehaviourTree', saveBehaviourTree);
ipcMain.handle('removeBehaviourTree', removeBehaviourTree);

module.exports = {
    sendBehaviourTree: sendBehaviourTree,
    startBehaviourTree: startBehaviourTree,
    stopBehaviourTree: stopBehaviourTree,
    getBehaviourTreeList: getBehaviourTreeList,
    saveBehaviourTree: saveBehaviourTree,
    removeBehaviourTree: removeBehaviourTree,
}