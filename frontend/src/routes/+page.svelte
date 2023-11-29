<script lang="ts">
    import { websocket_url } from './store.js';
   
    websocket_url.subscribe((value) => {
        console.log(value);
        try {
            let webSocket = new WebSocket(value);
            webSocket.addEventListener("open", (event) => {
                console.log("Open");
            });
            webSocket.addEventListener("message", (event) => {
                console.log(event);
            });
        } catch (e) {
            console.log(e);
        }
    });
    function set_websocket_url(event: Event & { target: HTMLInputElement }) {
        websocket_url.set(event.target.value);
    }
</script>

<h1>Welcome to SvelteKit</h1>
<p>
    Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation
</p>
<input on:input={set_websocket_url} />
