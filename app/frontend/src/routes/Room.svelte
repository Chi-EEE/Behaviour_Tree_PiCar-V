<script lang="ts">
	import ChatBox from "../lib/ChatBox.svelte";
	import LidarStream from "../lib/LidarStream.svelte";
	import UrlPattern from "url-pattern";
	import CodeBox from "../lib/CodeBox.svelte";

	const room_pattern = new UrlPattern("#/room/:room");
	let room_name = room_pattern.match(window.location.hash).room;

	const websocket: WebSocket = new WebSocket(
		`ws://${location.host}/ws/room?request=create&room_name=${room_name}`,
	);
</script>

<LidarStream {room_name} {websocket} />
<ChatBox {websocket} />
<CodeBox />