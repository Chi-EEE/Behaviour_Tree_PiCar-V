<script>
    import { Card } from "flowbite-svelte";

    import NavigationBar from "../lib/NavigationBar.svelte";

    import IPList from "../lib/IPList.svelte";

    import WebsocketServerConfig from "../lib/WebsocketServerConfig.svelte";
    import RaspberryPiSelector from "../lib/RaspberryPiSelector.svelte";
</script>

<main>
    <NavigationBar />
    <div class="grid grid-cols-6">
        <div class="col-span-5 p-3">
            <WebsocketServerConfig />
            <hr />
            <br />
            <RaspberryPiSelector />
        </div>
        <div class="col-start-6">
            <Card class="p-6">
                <IPList />
            </Card>
        </div>
    </div>
</main>
