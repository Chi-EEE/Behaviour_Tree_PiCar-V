<script>
    import { Label, Input, Button, Helper } from "flowbite-svelte";

    import {
        websocket_server_connection_state,
        websocket_server_port,
        websocket_server_code,
    } from "../websocket_server_store";

    /** @type {number} */
    let websocketServerPort = $websocket_server_port;

    /** @type {string} */
    let toggleWebsocketServerMessage = "Start Websocket Server";

    /** @type {string} */
    let helperText = "";

    /** @type {string} */
    let websocketServerPortInputColor = "";

    function displayWebsocketServerStatus() {
        if ($websocket_server_connection_state) {
            websocketServerPortInputColor = "green";
            helperText = "Websocket Server is running";
            toggleWebsocketServerMessage = "Stop Websocket Server";
        } else {
            websocketServerPortInputColor = "";
            helperText = "";
            toggleWebsocketServerMessage = "Start Websocket Server";
        }
    }

    websocket_server_connection_state.subscribe(displayWebsocketServerStatus);

    async function toggleWebsocketServer() {
        const websocketServerPortNumber = websocketServerPort * 1;
        if (typeof websocketServerPortNumber !== "number") {
            helperText = "Port must be a number";
            websocketServerPortInputColor = "red";
            return;
        }
        if (!$websocket_server_connection_state) {
            websocket_server_port.set(websocketServerPortNumber);
            const response = await api.startWebSocketServer({
                port: $websocket_server_port,
            });
            if (response.success) {
                websocket_server_connection_state.set(true);
                websocket_server_code.set(response.code);
            } else {
                // Error: Websocket Server is already running
                websocketServerPortInputColor = "red";
                helperText = response.message;
                toggleWebsocketServerMessage = "Start Websocket Server";
            }
        } else {
            api.closeWebSocketServer();
            websocket_server_connection_state.set(false);
        }
    }
</script>

<div class="mb-6">
    <Label for="websocket-server-port" class="block mb-2"
        >Input Port for Websocket Server</Label
    >
    <Input
        id="websocket-server-port"
        size="lg"
        placeholder="Websocket Server Port"
        bind:value={websocketServerPort}
        disabled={$websocket_server_connection_state}
        color={websocketServerPortInputColor}
    />
    <Button class="mt-4" on:click={toggleWebsocketServer}
        >{toggleWebsocketServerMessage}</Button
    >
    <Helper class="mt-2" color={websocketServerPortInputColor}
        >{helperText}</Helper
    >
</div>
