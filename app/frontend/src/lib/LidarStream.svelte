<script lang="ts">
    import { onMount } from "svelte";
    import { get } from "svelte/store";

    import { websocket_store } from "./WebsocketStore";
    import { detectLeftButton } from "./Mouse";
    import DrawButton from "./DrawButton.svelte";

    const DEFAULT_SUNFOUNDER_CAR_MAX_WIDTH = 200; // 30 cm

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

    websocket!!.addEventListener("message", (event: MessageEvent<any>) => {
        const json_data = JSON.parse(event.data);
        if (json_data.type == "car") {
            points.length = 0;
            console.log(json_data["data"].length);
            for (const point of json_data["data"]) {
                points.push(new ScanPoint(point.angle, point.distance));
            }
            draw();
        }
    });

    let canvas: HTMLCanvasElement;
    let context: CanvasRenderingContext2D;

    let zoom: number = 1;
    $: sunfounder_car_width = DEFAULT_SUNFOUNDER_CAR_MAX_WIDTH * zoom;

    function drawCursor() {
        context.fillStyle = "black";
        const width = 4;
        const height = 4;
        context.fillRect(
            canvas.width / 2 - width / 2,
            canvas.height / 2 - height / 2,
            width,
            height,
        );
        context.beginPath();
        context.ellipse(
            canvas.width / 2,
            canvas.height / 2,
            sunfounder_car_width / 2,
            sunfounder_car_width,
            0,
            0,
            2 * Math.PI,
        );
        context.stroke();
    }

    function clear() {
        context.clearRect(0, 0, canvas.width, canvas.height);
        context.fillStyle = "white";
        context.fillRect(0, 0, canvas.width, canvas.height);
    }

    function draw() {
        console.log("Drawing");
        clear();
        drawCursor();
        context.fillStyle = "blue";
        for (const point of points) {
            const angle = point.angle;
            const distance = point.distance * zoom;
            const angleInRadians = angle * (3.14159265 / 180.0);
            const x = distance * Math.cos(angleInRadians);
            const y = distance * Math.sin(angleInRadians);
            context.fillRect(x + canvas.width / 2, y + canvas.height / 2, 2, 2);
        }
    }

    onMount(async () => {
        context = canvas.getContext("2d")!!;
        clear();
        drawCursor();
    });

    let mouseDown: boolean = false;

    // https://stackoverflow.com/a/33063222
    function getMousePos(event: MouseEvent) {
        var rect = canvas.getBoundingClientRect();
        return {
            x:
                ((event.clientX - rect.left) / (rect.right - rect.left)) *
                canvas.width,
            y:
                ((event.clientY - rect.top) / (rect.bottom - rect.top)) *
                canvas.height,
        };
    }

    function onMouseDown(event: MouseEvent) {
        if (detectLeftButton(event)) {
            mouseDown = true;
        }
    }

    function onMouseUp(event: MouseEvent) {
        if (detectLeftButton(event)) {
            mouseDown = false;
        }
    }

    function onMouseMove(event: MouseEvent) {
        if (!mouseDown) {
            return;
        }
        const { x, y } = getMousePos(event);
        // drawPoints.push({ x, y });
        // context.fillStyle = "black";
        // context.fillRect(x, y, 4, 4);
    }
</script>

<canvas
    id="canvas"
    width="800"
    height="600"
    style="border-style:solid"
    on:mousedown={onMouseDown}
    on:mouseup={onMouseUp}
    on:mousemove={onMouseMove}
    bind:this={canvas}
/>
<DrawButton {canvas} />
