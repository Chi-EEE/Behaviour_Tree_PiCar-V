<script lang="ts">
    import { onMount } from "svelte";
    import { WebSocket } from "ws";
    onMount(() => {
        let websocket_url = "ws://localhost:8848/lidar";
        let webSocket = new WebSocket(websocket_url);
        webSocket.addEventListener("open", (event) => {
            console.log("Open");
        });
        webSocket.addEventListener("message", (event) => {
           let message = JSON.parse(event.data.toString());
           message["type"]
        });
    })
</script>

<canvas id="canvas" width="800" height="600"> </canvas>
