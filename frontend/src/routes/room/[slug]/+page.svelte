<script lang="ts">
	import { onMount } from "svelte";
	import { page } from "$app/stores";

	import LidarViewer from "$lib/LidarViewer.svelte";

	let data: {
		title: string | undefined;
		websocket_url: string | undefined;
	} = {
		title: undefined,
		websocket_url: undefined,
	};
	onMount(() => {
		data = {
			title: $page.params.slug,
			websocket_url: `ws://${location.host}/room/${$page.params.slug}`,
		};
	});
</script>

<h1>This is the Room page</h1>
<h2>Title: {data.title}</h2>

{#if data.websocket_url !== undefined}
	<LidarViewer websocket_url={data.websocket_url} />
{:else}
	<p>Waiting to retrieve the Websocket Url...</p>
{/if}
