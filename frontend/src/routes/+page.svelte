<script lang="ts">
    import { dev } from "$app/environment";
    import { onMount } from "svelte";
    import { websocket_url } from "./store.js";

    import LidarViewer from "$lib/LidarViewer.svelte";

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

<LidarViewer lidar_websocket_url={$websocket_url}/>
