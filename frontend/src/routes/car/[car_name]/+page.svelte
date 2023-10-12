<script>
    import { onMount } from 'svelte';

	export let data;
	let socket;
	onMount(() => {
		socket = new WebSocket("ws://localhost:8000/ws/chat/Room1/?nickname=Nick")
		socket.addEventListener("open", ()=> {
			console.log("Opened")
		})
		socket.addEventListener("message", (event) => {
			console.log("Message from server ", event.data);
		});
	})
</script>

<h1>{data.car_name}</h1>
