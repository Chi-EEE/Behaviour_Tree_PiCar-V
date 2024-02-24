const { ipcRenderer, contextBridge } = require('electron')

const WINDOW_API = {
    getLocalIPList: () => ipcRenderer.invoke("getLocalIPList")
}

contextBridge.exposeInMainWorld("api", WINDOW_API)