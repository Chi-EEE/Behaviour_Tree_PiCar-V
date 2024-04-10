<script>
    import BehaviourTreeHandler from "../lib/BehaviourTreeHandler.svelte";
    import CodeBox from "../lib/CodeBox.svelte";
    import RaspberryPiConnectBanner from "../lib/RaspberryPiConnectBanner.svelte";

    import {
        websocket_server_connection_state,
        frame_buffer,
        lidar,
    } from "../store/websocket_store";

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

    // From: https://stackoverflow.com/a/16245768
    function b64toBlob(b64Data, contentType = "", sliceSize = 512) {
        const byteCharacters = atob(b64Data);
        const byteArrays = [];

        for (
            let offset = 0;
            offset < byteCharacters.length;
            offset += sliceSize
        ) {
            const slice = byteCharacters.slice(offset, offset + sliceSize);

            const byteNumbers = new Array(slice.length);
            for (let i = 0; i < slice.length; i++) {
                byteNumbers[i] = slice.charCodeAt(i);
            }

            const byteArray = new Uint8Array(byteNumbers);
            byteArrays.push(byteArray);
        }

        const blob = new Blob(byteArrays, { type: contentType });
        return blob;
    }

    let live_feed;
    frame_buffer.subscribe((value) => {
        if (live_feed) {
            const blob = b64toBlob(value, "image/jpeg");
            const image_url = URL.createObjectURL(blob);
            live_feed.src = image_url;
        }
    });
</script>

<main style="height: 92vh">
    {#if $websocket_server_connection_state === false}
        <RaspberryPiConnectBanner />
    {/if}
    <SaveBehaviourTreeModal />
    <Splitpanes
        style="height:100%;padding:1em;background-color:rgb(20,20,40)"
        on:resize={onSideResize}
    >
        <Pane size={side_pane_size_0}>
            <Splitpanes horizontal={true} on:resize={onMainResize}>
                <Pane size={main_pane_size_0} minSize={5}>
                    <div
                        class="w-full h-full"
                        style="background-color:rgb(230,230,230)"
                        bind:clientWidth={stream_split_width}
                        bind:clientHeight={stream_split_height}
                    >
                        <!-- svelte-ignore a11y-missing-attribute -->
                        <img class="w-full h-full" bind:this={live_feed} />
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
