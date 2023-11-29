<script lang="ts">
    export let lidar_websocket_url: string;
    let webSocket = new WebSocket(lidar_websocket_url);
    webSocket.addEventListener("open", (event) => {
        console.log("Open");
    });
    webSocket.addEventListener("message", (event) => {
        console.log(event);
    });
</script>

<canvas id="canvas" width="800" height="600"> </canvas>
