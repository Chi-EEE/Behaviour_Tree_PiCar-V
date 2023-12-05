<script lang="ts">
    import { get } from "svelte/store";

    import { websocket_store } from "$lib/WebsocketStore";

    class ScanPoint {
        constructor(angle: number, distance: number) {
            this.angle = angle;
            this.distance = distance;
        }

        readonly angle: number;
        readonly distance: number;
    }

    let points: Array<ScanPoint> = [];

    const websocket = get(websocket_store);

    websocket.addEventListener("message", (event: MessageEvent<any>) => {
        const json_data = JSON.parse(event.data);
        if (json_data.type == "car") {
            points.length = 0;
            for (const point of json_data.points) {
                points.push(new ScanPoint(point.angle, point.distance));
            }
            draw();
        }
    })

    let canvas: HTMLCanvasElement;
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

    // onMount(async () => {
    //     await tick();
    //     draw();
    // });
</script>

<canvas id="canvas" width="800" height="600" bind:this={canvas}></canvas>
