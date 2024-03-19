<script>
    import BehaviourTreeHandler from "../lib/BehaviourTreeHandler.svelte";
    import CodeBox from "../lib/CodeBox.svelte";
    import RaspberryPiConnectBanner from "../lib/RaspberryPiConnectBanner.svelte";

    import { websocket_server_connection_state } from "../store/websocket_store";
    import {
        main_pane_size_0_store,
        main_pane_size_1_store,
        side_pane_size_0_store,
        side_pane_size_1_store,
    } from "../store/home_store";

    import { Pane, Splitpanes } from "svelte-splitpanes";
    import SaveBehaviourTreeModal from "../lib/SaveBehaviourTreeModal.svelte";

    /** @type {number} */
    let stream_split_height = 0;

    /** @type {number} */
    let stream_split_width = 0;

    let main_pane_size_0 = $main_pane_size_0_store;
    let main_pane_size_1 = $main_pane_size_1_store;
    let side_pane_size_0 = $side_pane_size_0_store;
    let side_pane_size_1 = $side_pane_size_1_store;

    function onMainResize(event) {
        main_pane_size_0_store.set(event.detail[0].size);
        main_pane_size_1_store.set(event.detail[1].size);
    }

    function onSideResize(event) {
        side_pane_size_0_store.set(event.detail[0].size);
        side_pane_size_1_store.set(event.detail[1].size);
    }
</script>

<main style="height: 92vh">
    {#if $websocket_server_connection_state === false}
        <RaspberryPiConnectBanner />
    {/if}
    <SaveBehaviourTreeModal />
    <Splitpanes style="height:100%;padding:1em;background-color:rgb(20,20,40)" on:resize={onSideResize}>
        <Pane size={side_pane_size_0}>
            <Splitpanes horizontal={true} on:resize={onMainResize}>
                <Pane size={main_pane_size_0} minSize={5}>
                    <div
                        class="w-full h-full"
                        style="background-color:rgb(230,230,230)"
                        bind:clientWidth={stream_split_width}
                        bind:clientHeight={stream_split_height}
                    >
                        <!--  -->
                    </div>
                </Pane>
                <Pane size={main_pane_size_1} minSize={7.5}>
                    <div
                        class="w-full h-full"
                        style="background-color:rgb(230,230,230)"
                    >
                        <CodeBox />
                    </div>
                </Pane>
            </Splitpanes>
        </Pane>
        <Pane size={side_pane_size_1} minSize={13} maxSize={80}>
            <BehaviourTreeHandler />
        </Pane>
    </Splitpanes>
</main>
