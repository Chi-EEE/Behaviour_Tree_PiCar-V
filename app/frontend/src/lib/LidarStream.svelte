<script>
    import { onMount } from "svelte";

    /** @type {string} */
    export let room_name;

    /** @type {WebSocket} */
    export let websocket;

    /** @type {number} */
    export let parentWidth;

    /** @type {number} */
    export let parentHeight;

    const DEFAULT_SUNFOUNDER_CAR_MAX_WIDTH = 200; // 30 cm

    class ScanPoint {
        /**
         * @param {number} angle
         * @param {number} distance
         */
        constructor(angle, distance) {
            this.angle = angle;
            this.distance = distance;
        }
    }

    /**
     * @type {Array<ScanPoint>}
     */
    let points = [];

    websocket.addEventListener(
        "message",
        /**
         * @param {MessageEvent<any>} event
         */
        (event) => {
            const json_data = JSON.parse(event.data);
            if (json_data.type == "car") {
                points.length = 0;
                console.log(json_data["data"].length);
                for (const point of json_data["data"]) {
                    points.push(new ScanPoint(point.angle, point.distance));
                }
                draw();
            }
        },
    );

    /** @type {HTMLCanvasElement} */
    let canvas;

    /** @type {CanvasRenderingContext2D} */
    let context;

    /** @type {number} */
    let zoom = 0.3;

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
        let _context = canvas.getContext("2d");
        if (!_context) {
            throw new Error("Could not get context");
        }
        context = _context;
        clear();
        drawCursor();
    });

    /** @type {number} */
    let information_tab_height = 0;

    $: {
        if (canvas) {
            draw();
            const width = parentWidth < 0 ? 0 : parentWidth;
            const height =
                parentHeight - information_tab_height < 0
                    ? 0
                    : parentHeight - information_tab_height;
            const targetAspectRatio = 16 / 9;
            const currentAspectRatio = width / height;

            if (currentAspectRatio > targetAspectRatio) {
                canvas.width = height * targetAspectRatio;
                canvas.height = height;
            } else {
                canvas.width = width;
                canvas.height = width / targetAspectRatio;
            }
        }
    }
</script>

<canvas
    id="canvas"
    class="border-2 border-black align-middle"
    style="margin-right: auto;margin-left: auto;display: block;"
    bind:this={canvas}
/>
{#if parentHeight - information_tab_height > 0}
    <div bind:clientHeight={information_tab_height}>
        <h1>Title: {room_name}</h1>
    </div>
{/if}
