<script>
    import { Label, Input, Button, Helper } from "flowbite-svelte";

    import { local_server_port } from "../local_server_store";
    
    let localServerPort = $local_server_port;

    /** @type {boolean} */
    let localServerEnabled = false;

    /** @type {string} */
    let toggleLocalServerMessage = "Start Local Server";

    function isNumeric(value) {
        return /^-?\d+$/.test(value);
    }

    /** @type {string} */
    let helperText = "";

    /** @type {string} */
    let localServerPortInputColor = "";

    function toggleLocalServer() {
        if (!isNumeric(localServerPort)) {
            helperText = "Port must be a number";
            localServerPortInputColor = "red";
            return;
        }
        localServerEnabled = !localServerEnabled;
        if (localServerEnabled) {
            localServerPortInputColor = "green";
            helperText = "Local Server is running";
            toggleLocalServerMessage = "Stop Local Server";
        } else {
            localServerPortInputColor = "";
            helperText = "";
            toggleLocalServerMessage = "Start Local Server";
            local_server_port.set(localServerPort);
        }
    }
</script>

<div class="mb-6">
    <Label for="local-server-port" class="block mb-2"
        >Input Port for Local Server</Label
    >
    <Input
        id="local-server-port"
        size="lg"
        placeholder="Local Server Port"
        bind:value={localServerPort}
        disabled={localServerEnabled}
        color={localServerPortInputColor}
    />
    {#if helperText}
        <Helper class="mt-2" color={localServerPortInputColor}
            >{helperText}</Helper
        >
    {/if}
    <Button class="mt-4" on:click={toggleLocalServer}
        >{toggleLocalServerMessage}</Button
    >
</div>
