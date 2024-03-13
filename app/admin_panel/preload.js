const { ipcRenderer, contextBridge } = require('electron')

const WINDOW_API = {
    getLocalIPList: () => ipcRenderer.invoke("getLocalIPList"),
    startWebSocketServer: (/** @type {number} */ port) => ipcRenderer.invoke("startWebSocketServer", port),
    closeWebSocketServer: () => ipcRenderer.invoke("closeWebSocketServer"),

    isConnected: () => ipcRenderer.invoke("isConnected"),

    onMessage: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onMessage', (_event, /** @type {string} */ value) => callback(value)),
    
    onConnection: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onConnection', (_event, /** @type {string} */ value) => callback(value)),
    onDisconnection: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onDisconnection', (_event, /** @type {string} */ value) => callback(value)),

    connectToRaspberryPi: (/** @type {string} */ uuid) => ipcRenderer.invoke("connectToRaspberryPi", uuid),
}

contextBridge.exposeInMainWorld("api", WINDOW_API)