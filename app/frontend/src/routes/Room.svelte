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

<Splitpanes horizontal={false} style="height:100vh; padding:1em">
	<Pane>
		<Splitpanes horizontal={true}>
			<Pane size={80}>
				<LidarStream {room_name} {websocket} />
			</Pane>
			<Pane size={20} snapSize={20}>
				<CodeBox />
			</Pane>
		</Splitpanes>
	</Pane>
	<Pane maxSize={20} snapSize={10}>
		<ChatBox {websocket} />
	</Pane>
</Splitpanes>