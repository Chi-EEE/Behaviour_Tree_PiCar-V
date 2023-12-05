<script lang="ts">
	import { onMount } from "svelte";
	import { page } from "$app/stores";

	import LidarStream from "$lib/LidarStream.svelte";

	import { socketStore } from "./store";

	socketStore.subscribe((value) => {
		console.log(value);
	});

	const room_name = $page.params.slug
	const websocket_url = `ws://${location.host}/ws/room`;

    let websocket = new WebSocket(`${websocket_url}?request=create&room_name=${room_name}`);
    
	websocket.addEventListener("open", (event) => {
        console.log("Websocket opened")
    });

    websocket.addEventListener("message", (event) => {
        const json_data = JSON.parse(event.data);
        switch (json_data["type"]) {
            case "car": {
                points.length = 0;
                for (const point of json_data["points"]) {
                    points.push(
                        new ScanPoint(point["angle"], point["distance"]),
                    );
                }
                break;
            }
            default: {
                console.log("Unknown message type: " + json_data["type"]);
            }
        }
    });
	function handle_keyup(event: KeyboardEvent) {
		console.log(event);
	}

	let data: {
		room_name: string | undefined;
		websocket_url: string | undefined;
	} = {
		room_name: undefined,
		websocket_url: undefined,
	};
	onMount(() => {
		data = {
			,
			,
		};
	});

	let message = "";
</script>

<h1>This is the Room page</h1>
<h2>Title: {data.room_name}</h2>

<input on:keyup={handle_keyup} bind:value={message} />

<h1>Hello {name}!</h1>

{#if data.websocket_url !== undefined}
	<LidarStream
		websocket_url={data.websocket_url}
		room_name={data.room_name}
	/>
{:else}
	<p>Waiting to retrieve the Websocket Url...</p>
{/if}
