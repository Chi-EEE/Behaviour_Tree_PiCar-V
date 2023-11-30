<script lang="ts">
    import { onMount, tick } from "svelte";
    export let websocket_url: string;

    let canvas: HTMLCanvasElement;

    class ScanPoint {
        constructor(angle: number, distance: number) {
            this.angle = angle;
            this.distance = distance;
        }

        readonly angle: number;
        readonly distance: number;
    }

    let points: Array<ScanPoint> = [];

    function draw() {
        const ctx: CanvasRenderingContext2D = canvas.getContext("2d")!!;
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        for (const point of points) {
            const x = canvas.width / 2 + point.distance * Math.cos(point.angle);
            const y =
                canvas.height / 2 + point.distance * Math.sin(point.angle);
            ctx.fillRect(x, y, 1, 1);
        }
    }

    onMount(async () => {
        await tick();
        draw();
    });

    let web_socket = new WebSocket(websocket_url);
    web_socket.addEventListener("open", (event) => {
        console.log("Open");
    });

    web_socket.addEventListener("message", (event) => {
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
</script>

<canvas id="canvas" width="800" height="600" bind:this={canvas}></canvas>
