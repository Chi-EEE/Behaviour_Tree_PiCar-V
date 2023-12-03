<script lang="ts">
	import { onMount } from "svelte";
	import { page } from "$app/stores";

	import LidarViewer from "$lib/LidarViewer.svelte";

	let data: {
		room_name: string | undefined;
		websocket_url: string | undefined;
	} = {
		room_name: undefined,
		websocket_url: undefined,
	};
	onMount(() => {
		data = {
			room_name: $page.params.slug,
			websocket_url: `ws://${location.host}/ws/room`,

		};
	});
</script>

<h1>This is the Room page</h1>
<h2>Title: {data.room_name}</h2>

{#if data.websocket_url !== undefined}
	<LidarViewer websocket_url={data.websocket_url} room_name={data.room_name} />
{:else}
	<p>Waiting to retrieve the Websocket Url...</p>
{/if}
