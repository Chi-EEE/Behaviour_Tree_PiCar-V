<script>
    import IPList from "../lib/IPList.svelte";

    import WebsocketServerConfig from "../lib/WebsocketServerConfig.svelte";
    import RaspberryPiSelector from "../lib/RaspberryPiSelector.svelte";
</script>

<main>
    <div class="grid grid-cols-6">
        <div class="col-span-5 p-3">
            <WebsocketServerConfig />
            <hr />
            <br />
            <RaspberryPiSelector />
        </div>
        <div class="col-start-6">
            <IPList />
        </div>
    </div>
</main>
<!-- ``` -->