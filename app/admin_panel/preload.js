const { ipcRenderer, contextBridge } = require('electron')

const WINDOW_API = {
    getLocalIPList: () => ipcRenderer.invoke("getLocalIPList"),
    startWebSocketServer: (/** @type {number} */ port) => ipcRenderer.invoke("startWebSocketServer", port),
    closeWebSocketServer: () => ipcRenderer.invoke("closeWebSocketServer"),
    onUpdateCode: (/** @type {(number) => void} */ callback) => ipcRenderer.on('update-code', (_event, /** @type {number} */ value) => callback(value)),
}

contextBridge.exposeInMainWorld("api", WINDOW_API)