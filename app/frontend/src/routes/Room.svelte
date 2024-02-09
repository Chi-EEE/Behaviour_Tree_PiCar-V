<script lang="ts">
    import LidarStream from "../lib/LidarStream.svelte";
    import UrlPattern from "url-pattern";
    import CodeBox from "../lib/CodeBox.svelte";
    import { Pane, Splitpanes } from "svelte-splitpanes";

    const room_pattern = new UrlPattern("#/room/:room");
    let room_name = room_pattern.match(window.location.hash).room;

    const websocket: WebSocket = new WebSocket(
        `ws://${location.host}/ws/room?request=create&room_name=${room_name}`,
    );

    let stream_split_height: number = 0;
    let stream_split_width: number = 0;
</script>

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
