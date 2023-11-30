<script>
	import { onMount } from 'svelte';
	import { page } from '$app/stores';

    function run_websocket() {
        let webSocket = new WebSocket(`ws://localhost:8848/chat/${page.params.name}`);
        webSocket.addEventListener("open", (event) => {
            console.log("Open");
        });
        webSocket.addEventListener("message", (event) => {
            console.log(event);
        });
    }

	let data = {};
	onMount(() => {
		console.log('page is mounted');
		console.log(`url is ${$page.url}`);
		data = {
			title: $page.params.name,
			content: $page.params.name
		};
	});
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

<h1>This is the Room page</h1>
<h2>Title: {data.title}</h2>
<p>{data.content}</p>
