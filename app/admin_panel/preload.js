const { ipcRenderer, contextBridge } = require('electron')

const WINDOW_API = {
    getLocalIPList: () => ipcRenderer.invoke("getLocalIPList"),
    startWebSocketServer: (/** @type {number} */ port) => ipcRenderer.invoke("startWebSocketServer", port),
    closeWebSocketServer: () => ipcRenderer.invoke("closeWebSocketServer"),

    isConnected: () => ipcRenderer.invoke("isConnected"),
    onConnect: (/** @type {() => void} */ callback) => ipcRenderer.on('onConnect', callback),
    onDisconnect: (/** @type {() => void} */ callback) => ipcRenderer.on('onDisconnect', callback),

    onMessage: (/** @type {(string) => void} */ callback) => ipcRenderer.on('onMessage', (_event, /** @type {string} */ value) => callback(value)),
}

contextBridge.exposeInMainWorld("api", WINDOW_API)