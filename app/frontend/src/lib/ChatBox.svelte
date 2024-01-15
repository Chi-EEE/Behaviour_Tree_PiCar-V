<script lang="ts">
    import ChatLog from "./ChatLog.svelte";
    import ChatButton from "./ChatButton.svelte";

    export let websocket: WebSocket;
</script>

<div>
    <ChatLog {websocket} />
    <ChatButton {websocket} />
</div>