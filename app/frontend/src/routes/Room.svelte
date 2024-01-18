<script lang="ts">
	import ChatBox from "../lib/ChatBox.svelte";
	import LidarStream from "../lib/LidarStream.svelte";
	import UrlPattern from "url-pattern";
	import CodeBox from "../lib/CodeBox.svelte";
	import { Pane, Splitpanes } from 'svelte-splitpanes';


	const room_pattern = new UrlPattern("#/room/:room");
	let room_name = room_pattern.match(window.location.hash).room;

	const websocket: WebSocket = new WebSocket(
		`ws://${location.host}/ws/room?request=create&room_name=${room_name}`,
	);
</script>

<Splitpanes horizontal={false}>
	<Pane>
		<Splitpanes horizontal={true} class="h-screen w-auto">
			<Pane>
				<LidarStream {room_name} {websocket} />
			</Pane>
			<Pane>
				<CodeBox />
			</Pane>
		</Splitpanes>
	</Pane>
	<Pane maxSize={20} snapSize={10}>
		<ChatBox {websocket} />
	</Pane>
</Splitpanes>