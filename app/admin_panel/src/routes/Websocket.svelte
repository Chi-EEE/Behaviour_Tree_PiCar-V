<script>
    import { Button, Card } from "flowbite-svelte";

    import WebsocketServerConfig from "../lib/WebsocketServerConfig.svelte";
    import IPList from "../lib/IPList.svelte";
    import NavigationBar from "../lib/NavigationBar.svelte";

    import { raspberry_pi_uuid_list } from "../websocket_server_store";
</script>

<main>
    <NavigationBar />
    <div class="grid grid-cols-6">
        <div class="col-span-5 p-3">
            <WebsocketServerConfig />
            <hr />
            <br />
            <h1 class="text-gray-900 dark:text-white text-2xl font-bold">Raspberry Pi UUID List</h1>
            {#each $raspberry_pi_uuid_list as uuid}
                <Card>
                    <Button
                        on:click={() => {
                            api.selectRaspberryPi({uuid: uuid}).then((response)=>{
                                console.log(response);
                            });
                        }}
                    >
                        {uuid}
                    </Button>
                </Card>
            {/each}
        </div>
        <div class="col-start-6">
            <Card class="p-6">
                <IPList />
            </Card>
        </div>
    </div>
</main>
