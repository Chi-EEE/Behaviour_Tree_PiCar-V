<script lang="ts">
    import { dev } from "$app/environment";
    import { onMount } from "svelte";
    import { websocket_url } from "./store.js";

    import LidarViewerDev from "$lib/LidarViewerDev.svelte";
    import LidarViewer from "$lib/LidarViewer.svelte";

    // websocket_url.subscribe((value) => {
    //     console.log(value);
    //     try {
    //         let webSocket = new WebSocket(value);
    //         webSocket.addEventListener("open", (event) => {
    //             console.log("Open");
    //         });
    //         webSocket.addEventListener("message", (event) => {
    //             console.log(event);
    //         });
    //     } catch (e) {
    //         console.log(e);
    //     }
    // });
    // function set_websocket_url(event: Event & { target: HTMLInputElement }) {
    //     websocket_url.set(event.target.value);
    // }
    onMount(() => {
        if (dev) {
            websocket_url.set("ws://localhost:8848/chat");
        } else {
            websocket_url.set("ws://" + location.hostname + "/chat");
        }
    });
</script>

<h1>Welcome to SvelteKit</h1>
<p>
    Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation
</p>

{#if dev}
    <LidarViewerDev lidar_websocket_url={"ws://localhost:8848/chat"}></LidarViewerDev>
{:else}
    <!-- <LidarViewer lidar_websocket_url={$websocket_url}>
    
</LidarViewer> -->
{/if}
