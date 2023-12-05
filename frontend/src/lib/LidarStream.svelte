<script lang="ts">
    import { onMount, tick } from "svelte";
    export let websocket_url: string;
    export let room_name: string;

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

</script>

<canvas id="canvas" width="800" height="600" bind:this={canvas}></canvas>
