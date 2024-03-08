// Modules to control application life and create native browser window
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const serve = require('electron-serve');
const loadURL = serve({ directory: 'public' });

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
/** @type {BrowserWindow | undefined} */
let mainWindow;

function isDev() {
    return !app.isPackaged;
}

function createWindow() {
    // Create the browser window.
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            preload: path.join(__dirname, 'preload.js'),
            devTools: isDev(),
            // enableRemoteModule: true,
            // contextIsolation: false
        },
        icon: path.join(__dirname, 'public/favicon.png'),
        show: false
    });

    // This block of code is intended for development purpose only.
    // Delete this entire block of code when you are ready to package the application.
    if (isDev()) {
        mainWindow.loadURL('http://localhost:8080/');
    } else {
        loadURL(mainWindow);
    }

    // Uncomment the following line of code when app is ready to be packaged.
    // loadURL(mainWindow);

    // Open the DevTools and also disable Electron Security Warning.
    // process.env['ELECTRON_DISABLE_SECURITY_WARNINGS'] = true;
    // mainWindow.webContents.openDevTools();

    // Emitted when the window is closed.
    mainWindow.on('closed', function () {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null
    });

    // Emitted when the window is ready to be shown
    // This helps in showing the window gracefully.
    mainWindow.once('ready-to-show', () => {
        mainWindow.show()
    });
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow);

// Quit when all windows are closed.
app.on('window-all-closed', function () {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') app.quit()
});

app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (mainWindow === null) createWindow()
});
// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.

const tcpPortUsed = require('tcp-port-used');
const os = require('os')
const WebSocket = require('ws');

/**
 * @param {number} min 
 * @param {number} max 
 * @returns {number}
 */
function getRandomInt(min, max) {
    return min + (Math.floor(Math.random() * max) - min);
}

function getLocalIPList(_event, _args) {
    return os.networkInterfaces();
}

class Code {
    constructor() {
    }

    get() {
        return this._code;
    }

    generate() {
        this._code = getRandomInt(0, 9999);
        mainWindow.webContents.send('update-code', this._code);
        console.log("Code:", this._code);
    }
}

class WebSocketServer {
    constructor() {
        /** @type {WebSocket.Server | undefined} */
        this._wss = undefined;
        /** @type {Code} */
        this._code = new Code();
    }

    /**
     * Call this function to start the WebSocket server.
     * @param {number} port 
     */
    connect(port) {
        this._wss = new WebSocket.Server({ port: port });
    }

    /**
     * Call this function to close the WebSocket server.
     */
    close() {
        if (this._wss !== undefined) {
            this._wss.close();
        }
    }

    /** @type {WebSocket.RawData} */
    onRaspberryPiMessage(message) {
        
    }

    /**
     * This function only allows a single connection to the WebSocket server.
     */
    async waitForWSConnection() {
        this._code.generate();
        this._wss.once('connection', (ws) => {
            ws.once('close', () => {
                if (wss === undefined) {
                    return;
                }
                this._code.generate();
                setTimeout(waitForWSConnection);
            });
    
            let connected = false;
            
            ws.once('message', async (message) => {
                try {
                    const inputted_code = Number(message);
                    if (this._code.get() === inputted_code) {
                        connected = true;
                        ws.on('message', this.onRaspberryPiMessage);
                    } else {
                        ws.close();
                    }
                } catch (error) {
                    console.error("Error while processing message:", error);
                }
            });
    
            // Close the connection if the client does not send any message within 5 seconds.
            setTimeout(() => {
                if (connected || wss === undefined) {
                    return;
                }
                if (ws.readyState !== ws.CLOSED) {
                    ws.close();
                }
            }, 5000);
        });
    }
}

const websocket_server = new WebSocketServer();

// ================== IPC Handlers ==================

async function startWebSocketServer(_event, args) {
    if (await tcpPortUsed.check(args.port)) {
        return { success: false, message: `Port ${args.port} is already in use` };
    }
    try {
        websocket_server.connect(args.port);
        websocket_server.waitForWSConnection();
        return { success: true };
    } catch (error) {
        return { success: false, message: `Unable to start WebSocket Server, Error: ${error}` };
    }
}

function closeWebSocketServer(_event, _args) {
    websocket_server.close();
}

function onClose() {
    closeWebSocketServer();
}

app.on('window-all-closed', onClose);
ipcMain.handle('getLocalIPList', getLocalIPList);
ipcMain.handle('startWebSocketServer', startWebSocketServer);
ipcMain.handle('closeWebSocketServer', closeWebSocketServer);
