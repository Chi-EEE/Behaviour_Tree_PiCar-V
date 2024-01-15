<script lang="ts">
    export let websocket: WebSocket;

    let message = "";
    function sendMessage() {
        websocket.send(JSON.stringify({ data: message }));
    }
</script>

<input
    class="border-black border-2 col-span-3"
    on:keydown={(key_event) => {
        if (key_event.key === "Enter") sendMessage();
    }}
    bind:value={message}
/>
<button on:click={sendMessage}>Send</button>