const { app, ipcMain } = require('electron');
const { websocket_server } = require('./websocket');
const { getStore } = require('./store')
const util = require('util');
const exec = util.promisify(require('child_process').exec);
const Os = require('os');
const path = require("path");
const fs = require("fs");

function isDev() {
    return !app.isPackaged;
}

const base_path = app.getAppPath();
const resources_dir = isDev() ? path.join(base_path, 'resources') : path.join(base_path, '..', '..', 'resources');

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
    updateBehaviourTreeList(behaviour_tree_list.concat({ uuid: uuid, name: name, code: code }));
}

async function removeBehaviourTree(_event, args) {
    const uuid = args.uuid;
    let behaviour_tree_list = getBehaviourTreeList();
    behaviour_tree_list = behaviour_tree_list.filter((behaviour_tree) => behaviour_tree.uuid !== uuid);
    updateBehaviourTreeList(behaviour_tree_list);
}

async function validateBehaviourTree(_event, args) {
    /** @type {string} */
    let code = args.data;
    code = code.replace(new RegExp('"|\n'), "'");
    const behaviour_tree_validator = path.join(resources_dir, Os.platform(), Os.arch(), 'behaviour_tree_validator.exe');
    if (!fs.existsSync(behaviour_tree_validator)) {
        return JSON.stringify({ success: false, error: `The current platform / architecture is not supported.` });
    }
    const command = `${behaviour_tree_validator} --behaviour_tree @"\n${code}\n"@`;
    const { stdout, stderr } = await exec(command, { 'shell': 'powershell.exe' });
    return stdout;
}

ipcMain.handle('sendBehaviourTree', sendBehaviourTree);
ipcMain.handle('startBehaviourTree', startBehaviourTree);
ipcMain.handle('stopBehaviourTree', stopBehaviourTree);

ipcMain.handle('getBehaviourTreeList', getBehaviourTreeList);
ipcMain.handle('saveBehaviourTree', saveBehaviourTree);
ipcMain.handle('removeBehaviourTree', removeBehaviourTree);

ipcMain.handle('validateBehaviourTree', validateBehaviourTree);

module.exports = {
    sendBehaviourTree: sendBehaviourTree,
    startBehaviourTree: startBehaviourTree,
    stopBehaviourTree: stopBehaviourTree,
    getBehaviourTreeList: getBehaviourTreeList,
    saveBehaviourTree: saveBehaviourTree,
    removeBehaviourTree: removeBehaviourTree,
    validateBehaviourTree: validateBehaviourTree,
}