// Modules to control application life and create native browser window
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const serve = require('electron-serve');
const loadURL = serve({ directory: 'public' });

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
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

/**  @type {WebSocket.Server | undefined} */
let wss = undefined;

function getLocalIPList(_event, _args) {
    return os.networkInterfaces();
}

async function startWebSocketServer(_event, args) {
    if (await tcpPortUsed.check(args.port)) {
        return { success: false, message: `Port ${args.port} is already in use` };
    }
    try {
        wss = new WebSocket.Server({ port: args.port });
        waitForWSConnection();
    } catch (_) {
        return { success: false, message: `Unable to start WebSocket Server, Error: ${error}` };
    }
    return { success: true };
}

/**
 * https://stackoverflow.com/a/48161723
 * @param {string} message 
 * @returns {Promise<string>}
 */
async function sha256(message) {
    // encode as UTF-8
    const msgBuffer = new TextEncoder().encode(message);

    // hash the message
    const hashBuffer = await crypto.subtle.digest('SHA-256', msgBuffer);

    // convert ArrayBuffer to Array
    const hashArray = Array.from(new Uint8Array(hashBuffer));

    // convert bytes to hex string                  
    const hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
    return hashHex;
}

const authenication_code = 'HI';

/** @type {string} */
let authenication_hash;

async function initialize_authenication_hash() {
    authenication_hash = await sha256(authenication_code);
}

initialize_authenication_hash();

/**
 * This function only allows a single connection to the WebSocket server.
 */
async function waitForWSConnection() {
    wss.once('connection', (ws) => {
        ws.once('close', () => {
            if (wss === undefined) {
                return;
            }
            setTimeout(waitForWSConnection);
        });
        ws.once('message', async (message) => {
            try {
                const hash = await sha256(message);
                if (hash === authenication_hash) {
                    ws.on('message', onRaspberryPiMessage);
                } else {
                    ws.close();
                }
            } catch (error) {
                console.error("Error while processing message:", error);
            }
        });
    });
}

/**
 * 
 * @param {WebSocket.RawData} message 
 */
function onRaspberryPiMessage(message) {

}

function closeWebSocketServer(_event, _args) {
    if (wss) {
        wss.close();
    }
}

function onClose() {
    closeWebSocketServer();
}

app.on('window-all-closed', onClose);
ipcMain.handle('getLocalIPList', getLocalIPList);
ipcMain.handle('startWebSocketServer', startWebSocketServer);
ipcMain.handle('closeWebSocketServer', closeWebSocketServer);
