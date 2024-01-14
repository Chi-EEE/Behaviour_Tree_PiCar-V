<script lang="ts">
	import LidarStream from "../lib/LidarStream.svelte";
	import UrlPattern from "url-pattern";

	const room_pattern = new UrlPattern("#/room/:room");
	let room_name = room_pattern.match(window.location.hash).room;

	const websocket: WebSocket = new WebSocket(`ws://${location.host}/ws/room?request=create&room_name=${room_name}`);

	let message = "";
	function sendMessage() {
		websocket.send(JSON.stringify({ data: message }));
	}
</script>

<LidarStream {room_name} {websocket}/>
<h1>Title: {room_name}</h1>
<input
	on:keydown={(key_event) => {
		if (key_event.key === "Enter") sendMessage();
	}}
	bind:value={message}
/>
<button on:click={sendMessage}>Send</button>
