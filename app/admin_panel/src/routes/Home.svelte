<script>
    import BehaviourTreeHandler from "../lib/BehaviourTreeHandler.svelte";
    import CodeBox from "../lib/CodeBox.svelte";
    import NavigationBar from "../lib/NavigationBar.svelte";
    import RaspberryPiConnectBanner from "../lib/RaspberryPiConnectBanner.svelte";

    import { websocket_server_connection_state } from "../store/websocket_store";

    import { Pane, Splitpanes } from "svelte-splitpanes";

    /** @type {number} */
    let stream_split_height = 0;

    /** @type {number} */
    let stream_split_width = 0;
</script>

<main>
    <NavigationBar />
    {#if $websocket_server_connection_state}
        <RaspberryPiConnectBanner />
    {/if}
    <Splitpanes style="height:100vh;padding:1em;background-color:rgb(20,20,40)">
        <Pane>
            <Splitpanes horizontal={true}>
                <Pane size={80} minSize={5}>
                    <div
                        class="w-full h-full"
                        style="background-color:rgb(230,230,230)"
                        bind:clientWidth={stream_split_width}
                        bind:clientHeight={stream_split_height}
                    >
                        <!--  -->
                    </div>
                </Pane>
                <Pane size={20} minSize={5}>
                    <div
                        class="w-full h-full"
                        style="background-color:rgb(230,230,230)"
                    >
                        <CodeBox />
                    </div>
                </Pane>
            </Splitpanes>
        </Pane>
        <Pane size={13} minSize={13} maxSize={80}>
            <BehaviourTreeHandler />
        </Pane>
    </Splitpanes>
</main>
