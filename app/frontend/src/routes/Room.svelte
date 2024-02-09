<script>
    import LidarStream from "../lib/LidarStream.svelte";
    import UrlPattern from "url-pattern";
    import CodeBox from "../lib/CodeBox.svelte";
    import { Pane, Splitpanes } from "svelte-splitpanes";

    const room_pattern = new UrlPattern("#/room/:room");
    let room_name = room_pattern.match(window.location.hash).room;

    /**
     * @type {WebSocket}
     */
    const websocket = new WebSocket(
        `ws://${location.host}/ws/room?request=create&room_name=${room_name}`,
    );

    /**
     * @type {number}
     */
    let stream_split_height = 0;

    /**
     * @type {number}
     */
    let stream_split_width = 0;
</script>

<div
    style="
    position: absolute;
    top: 20px;
    left: 30px;
    background-color: #BC2731;
    color: white;
"
    class="z-50 p-2 rounded-lg shadow-lg"
>
    <button>Exit Room</button>
</div>
<Splitpanes
    horizontal={true}
    style="height:100vh;padding:1em;background-color:rgb(20,20,40)"
>
    <Pane size={80} minSize={10} snapSize={20}>
        <div
            class="w-full h-full"
            style="background-color:rgb(230,230,230)"
            bind:clientWidth={stream_split_width}
            bind:clientHeight={stream_split_height}
        >
            <LidarStream
                {room_name}
                {websocket}
                parentWidth={stream_split_width}
                parentHeight={stream_split_height}
            />
        </div>
    </Pane>
    <Pane size={20} minSize={10} snapSize={20}>
        <div class="w-full h-full" style="background-color:rgb(230,230,230)">
            <CodeBox {websocket} />
        </div>
    </Pane>
</Splitpanes>
