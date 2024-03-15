const WebSocket = require('ws');

/**
 * @param {number} min 
 * @param {number} max 
 * @returns {number}
 */
function getRandomInt(min, max) {
    return min + (Math.floor(Math.random() * max) - min);
}

class Code {
    constructor() {
        this._code = 0;
    }

    get() {
        return this._code;
    }

    generate() {
        this._code = getRandomInt(0, 99999);
    }
}

class WebSocketServer {
    constructor() {
        /** @type {WebSocket.Server | undefined} */
        this._wss = undefined;

        /** @type {number | undefined} */
        this._port = undefined;

        /** @type {Code} */
        this._code = new Code();

        /** @type {Map<string, WebSocket>} */
        this._raspberry_pi_clients = new Map();

        /** @type {WebSocket | undefined} */
        this._client = undefined;
    }

    /**
     * Call this function to start the WebSocket server.
     * @param {number} port 
     */
    connect(port) {
        this._wss = new WebSocket.Server({ port: port });
        this._port = port;
    }

    /**
     * Call this function to close the WebSocket server.
     */
    close() {
        if (this._wss !== undefined) {
            this._wss.close();
        }
    }

    generateCode() {
        this._code.generate();
        return this._code.get();
    }

    getCode() {
        return this._code.get();
    }

    /**
     * This function only allows a single connection to the WebSocket server.
     */
    async createConnection() {
        this._wss.on('connection', (ws, req) => {
            // console.log(req.headers)
            // console.log(req.socket.remoteAddress)
            const uuid = crypto.randomUUID();
            this._raspberry_pi_clients.set(uuid, ws);

            ws.send(JSON.stringify({ uuid: uuid }));

            mainWindow.webContents.send('onConnection', JSON.stringify({ uuid: uuid }));

            ws.once('close', () => {
                if (this._wss === undefined) {
                    return;
                }
                mainWindow.webContents.send('onDisconnection', JSON.stringify({ uuid: uuid }));
            });

            ws.on('message', async (message) => {
                if (uuid !== this._client.uuid) {
                    return;
                }
                mainWindow.webContents.send('onMessage', message.toString());
            });
        });
    }
}

module.exports = { websocket_server: new WebSocketServer() };