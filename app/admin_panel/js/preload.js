const { ipcRenderer, contextBridge } = require('electron')

const WINDOW_API = {
    getLocalIPList: () => ipcRenderer.invoke("getLocalIPList"),

    startWebSocketServer: (/** @type {number} */ port) => ipcRenderer.invoke("startWebSocketServer", port),
    closeWebSocketServer: () => ipcRenderer.invoke("closeWebSocketServer"),
    getWebSocketServer: () => ipcRenderer.invoke("getWebSocketServer"),

    isConnected: () => ipcRenderer.invoke("isConnected"),

    onMessage: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onMessage', (_event, /** @type {string} */ value) => callback(value)),
    
    onConnection: (/** @type {(any) => void} */ callback) => ipcRenderer.on('onConnection', (_event, /** @type {any} */ args) => callback(args)),
    onDisconnection: (/** @type {(any) => void} */ callback) => ipcRenderer.on('onDisconnection', (_event, /** @type {any} */ args) => callback(args)),

    selectRaspberryPi: (/** @type {any} */ args) => ipcRenderer.invoke("selectRaspberryPi", args),
    unselectRaspberryPi: () => ipcRenderer.invoke("unselectRaspberryPi"),
    getRaspberryPiList: () => ipcRenderer.invoke("getRaspberryPiList"),
    getSelectedRaspberryPi: () => ipcRenderer.invoke("getSelectedRaspberryPi"),

    sendBehaviourTree: (/** @type {any} */ args) => ipcRenderer.invoke("sendBehaviourTree", args),
    startBehaviourTree: () => ipcRenderer.invoke("startBehaviourTree"),
    stopBehaviourTree: () => ipcRenderer.invoke("stopBehaviourTree"),
    
    getBehaviourTreeList: () => ipcRenderer.invoke("getBehaviourTreeList"),
    saveBehaviourTree: (/** @type {any} */ args) => ipcRenderer.invoke("saveBehaviourTree", args),
    removeBehaviourTree: (/** @type {any} */ args) => ipcRenderer.invoke("removeBehaviourTree", args)
}

contextBridge.exposeInMainWorld("api", WINDOW_API)