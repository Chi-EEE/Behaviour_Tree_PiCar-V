<script lang="ts">
	import { onMount } from "svelte";
	import { get } from "svelte/store";

	import { page } from "$app/stores";

	import LidarStream from "$lib/LidarStream.svelte";

	import { websocket_store, websocket_url_store } from "$lib/WebsocketStore";

	const room_name = $page.params.slug;

	websocket_url_store.set(
		`ws://${location.host}/ws/room?request=create&room_name=${room_name}`,
	);
	websocket_store.subscribe((websocket) => {
		websocket!!.addEventListener("open", (event) => {
			console.log("Websocket opened");
		});

		websocket!!.addEventListener("message", (event: MessageEvent<any>) => {
			const json_data = JSON.parse(event.data);
			console.log(json_data);
		});
	});

	let message = "";
	function sendMessage() {
		const websocket = get(websocket_store);
		websocket!!.send(JSON.stringify({ data: message }));
	}

	let data: {
		room_name: string | undefined;
	} = {
		room_name: room_name,
	};
	onMount(() => {
		return {
			data,
		};
	});
</script>

<h1>This is the Room page</h1>
<h2>Title: {data.room_name}</h2>

<input on:keydown={(key_event) => {
	if (key_event.key === "Enter")
		sendMessage()
	}} bind:value={message} />
<button on:click={sendMessage}>Send</button>

<h1>Hello {data.room_name}!</h1>

{#if $websocket_url_store !== undefined}
	<LidarStream />
{:else}
	<p>Waiting to retrieve the Websocket Url...</p>
{/if}
