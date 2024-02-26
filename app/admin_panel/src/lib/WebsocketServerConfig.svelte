<script>
    import { Label, Input, Button, Helper } from "flowbite-svelte";

    import { websocket_server_port } from "../websocket_server_store";

    let websocketServerPort = $websocket_server_port;

    /** @type {boolean} */
    let websocketServerEnabled = false;

    /** @type {string} */
    let toggleWebsocketServerMessage = "Start Websocket Server";

    function isNumeric(value) {
        return /^-?\d+$/.test(value);
    }

    /** @type {string} */
    let helperText = "";

    /** @type {string} */
    let websocketServerPortInputColor = "";

    function toggleWebsocketServer() {
        if (!isNumeric(websocketServerPort)) {
            helperText = "Port must be a number";
            websocketServerPortInputColor = "red";
            return;
        }
        websocketServerEnabled = !websocketServerEnabled;
        if (websocketServerEnabled) {
            websocketServerPortInputColor = "green";
            helperText = "Websocket Server is running";
            toggleWebsocketServerMessage = "Stop Websocket Server";
        } else {
            websocketServerPortInputColor = "";
            helperText = "";
            toggleWebsocketServerMessage = "Start Websocket Server";
            websocket_server_port.set(websocketServerPort);
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
        disabled={websocketServerEnabled}
        color={websocketServerPortInputColor}
    />
    <Button class="mt-4" on:click={toggleWebsocketServer}
        >{toggleWebsocketServerMessage}</Button
    >
    <Helper class="mt-2" color={websocketServerPortInputColor}>{helperText}</Helper>
</div>
