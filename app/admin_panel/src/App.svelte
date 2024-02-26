<script>
    import { Card } from "flowbite-svelte";

    import WebsocketServerConfig from "./lib/WebsocketServerConfig.svelte";
    import IPList from "./lib/IPList.svelte";
    import NavigationBar from "./lib/NavigationBar.svelte";
</script>

<main>
    <NavigationBar />
    <div class="grid grid-cols-6">
        <div class="col-span-5 p-2">
            <WebsocketServerConfig />
            <hr />
            <br />
            <h1>The Raspberry Pi is not connected to the Websocket Server</h1>
        </div>
        <div class="col-start-6">
            <Card class="p-6">
                <h1>Local IP List</h1>
                <br />
                <IPList />
            </Card>
        </div>
    </div>
</main>

<style>
    main {
        text-align: center;
        padding: 1em;
        max-width: 240px;
        margin: 0 auto;
    }

    @media (min-width: 640px) {
        main {
            max-width: none;
        }
    }
</style>
