<script>
    import { Card } from "flowbite-svelte";

    import WebsocketServerConfig from "../lib/WebsocketServerConfig.svelte";
    import IPList from "../lib/IPList.svelte";
    import NavigationBar from "../lib/NavigationBar.svelte";
</script>

<main>
    <NavigationBar />
    <div class="grid grid-cols-6">
        <div class="col-span-5 p-3">
            <WebsocketServerConfig />
            <hr />
            <br />
            <h1>The Raspberry Pi is not connected to the Websocket Server</h1>
        </div>
        <div class="col-start-6">
            <Card class="p-6">
                <IPList />
            </Card>
        </div>
    </div>
</main>