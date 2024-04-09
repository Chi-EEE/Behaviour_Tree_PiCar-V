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

/**
 * @typedef {Object} RaspberryPi
 * @property {string} uuid -- Universally Unique Identifier
 * @property {WebSocket} ws -- WebSocket connection
 */

class WebSocketServer {
    constructor() {
        /** @type {WebSocket.Server | undefined} */
        this._wss = undefined;

        /** @type {number | undefined} */
        this._port = undefined;

        /** @type {Code} */
        this._code = new Code();

        /** @type {Map<string, WebSocket>} */
        this._raspberry_pi_map = new Map();

        /** @type {RaspberryPi | undefined} */
        this._selected_raspberry_pi = undefined;
    }

    /**
     * Call this function to start the WebSocket server.
     * @param {number} port 
     */
    connect(port) {
        this._wss = new WebSocket.Server({ port: port });
        this._port = port;
    }

    isConnected() {
        return this._wss !== undefined;
    }

    /**
     * Call this function to close the WebSocket server.
     */
    close() {
        if (this._wss !== undefined) {
            for (const ws of this._raspberry_pi_map.values()) {
                ws.close();
            }
            this._wss.close();
            this._wss = undefined;
        }
    }

    /**
     * 
     * @returns {number}
     */
    generateCode() {
        this._code.generate();
        return this._code.get();
    }

    /**
     * 
     * @returns {number}
     */
    getCode() {
        return this._code.get();
    }

    /**
     * 
     * @returns {number}
     */
    getPort() {
        return this._port;
    }

    /**
     * 
     * @returns {Map<string, WebSocket>}
     */
    getRaspberryPiMap() {
        return this._raspberry_pi_map;
    }

    /**
     * 
     * @returns {RaspberryPi | undefined}
     */
    getSelectedRaspberryPi() {
        return this._selected_raspberry_pi;
    }

    /**
     * Select a Raspberry Pi to send / receive messages from.
     * @param {string} uuid 
     * @returns 
     */
    selectRaspberryPi(uuid) {
        if (this._raspberry_pi_map.has(uuid)) {
            this._selected_raspberry_pi = { uuid: uuid, ws: this._raspberry_pi_map.get(uuid) };
            return { success: true, message: `Connected to Raspberry Pi with UUID: ${uuid}` };
        } else {
            return { success: false, message: `Raspberry Pi with UUID: ${uuid} not found` };
        }
    }

    async startConnections() {
        this._wss.on('connection', (ws, req) => {
            const uuid = crypto.randomUUID();
            this._raspberry_pi_map.set(uuid, ws);

            ws.send(JSON.stringify({ uuid: uuid }));

            global.mainWindow.webContents.send('onConnection', JSON.stringify({ uuid: uuid }));

            ws.once('close', () => {
                if (this._wss === undefined) {
                    return;
                }
                this._raspberry_pi_map.delete(uuid);
                global.mainWindow.webContents.send('onDisconnection', JSON.stringify({ uuid: uuid }));
            });

            ws.on('message', async (message) => {
                if (uuid !== this.getSelectedRaspberryPi()?.uuid) {
                    return;
                }
                global.mainWindow.webContents.send('onMessage', message.toString());
            });
        });
    }

    /**
     * @param {string} behaviour_tree
     */
    sendBehaviourTree(behaviour_tree) {
        if (this._wss === undefined) {
            return;
        }
        if (this._selected_raspberry_pi === undefined) {
            return;
        }
        this._selected_raspberry_pi.ws.send(JSON.stringify({
            type: 'command',
            command: 'behaviour_tree',
            action: 'set',
            data: behaviour_tree
        }));
    }

    startBehaviourTree() {
        if (this._wss === undefined) {
            return;
        }
        if (this._selected_raspberry_pi === undefined) {
            return;
        }
        this._selected_raspberry_pi.ws.send(JSON.stringify({
            type: 'command',
            command: 'behaviour_tree',
            action: 'start',
        }));
    }

    stopBehaviourTree() {
        if (this._wss === undefined) {
            return;
        }
        if (this._selected_raspberry_pi === undefined) {
            return;
        }
        this._selected_raspberry_pi.ws.send(JSON.stringify({
            type: 'command',
            command: 'behaviour_tree',
            action: 'stop',
        }));
    }
}

module.exports = { websocket_server: new WebSocketServer() };