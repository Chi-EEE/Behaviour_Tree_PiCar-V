const { ipcRenderer, contextBridge } = require('electron')

const WINDOW_API = {
    getLocalIPList: () => ipcRenderer.invoke("getLocalIPList"),

    startWebSocketServer: (/** @type {number} */ port) => ipcRenderer.invoke("startWebSocketServer", port),
    closeWebSocketServer: () => ipcRenderer.invoke("closeWebSocketServer"),
    getWebSocketServer: () => ipcRenderer.invoke("getWebSocketServer"),

    isConnected: () => ipcRenderer.invoke("isConnected"),

    onMessage: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onMessage', (_event, /** @type {string} */ value) => callback(value)),
    
    onConnection: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onConnection', (_event, /** @type {string} */ value) => callback(value)),
    onDisconnection: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onDisconnection', (_event, /** @type {string} */ value) => callback(value)),

    connectRaspberryPi: (/** @type {string} */ uuid) => ipcRenderer.invoke("connectRaspberryPi", uuid),
    disconnectRaspberryPi: () => ipcRenderer.invoke("disconnectRaspberryPi"),
    getRaspberryPi: () => ipcRenderer.invoke("getRaspberryPi"),
}

contextBridge.exposeInMainWorld("api", WINDOW_API)