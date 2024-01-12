<script lang="ts">
    import { detectLeftButton } from "./Mouse";

    export let canvas: HTMLCanvasElement;

    const fps: number = 60;
    let mouseDown: boolean = false;

    const distanceToTravel = 33.333333333333333333333333333333;

    let x: number = 0;
    let y: number = 0;

    let angle: number = 270;
    $: angleInRadians = angle * (3.14159265 / 180.0);

    function update() {
        const halfCanvasWidth = canvas.width / 2;
        const halfCanvasHeight = canvas.height / 2;

        const xVector: number = Math.cos(angleInRadians);
        const yVector: number = Math.sin(angleInRadians);

        const magnitude: number = Math.sqrt(
            xVector * xVector + yVector * yVector,
        );

        const unitVectorX: number = xVector / magnitude;
        const unitVectorY: number = yVector / magnitude;

        const deltaTimeDistance = distanceToTravel / 60;

        x = x + unitVectorX * deltaTimeDistance;
        y = y + unitVectorY * deltaTimeDistance;

        const context = canvas.getContext("2d")!!;
        context.fillStyle = "black";
        context.fillRect(
            x + halfCanvasWidth - 2.5,
            y + halfCanvasHeight - 2.5,
            5,
            5,
        );
    }

    window.addEventListener('mouseout', () => mouseDown = false);

    function onMouseDown(event: MouseEvent) {
        if (detectLeftButton(event)) {
            mouseDown = true;
            let nIntervId: number;
            nIntervId = setInterval(() => {
                if (!mouseDown) {
                    clearInterval(nIntervId);
                    return;
                }
                update();
            }, 1000 / fps);
        }
    }
    function onMouseUp(event: MouseEvent) {
        if (detectLeftButton(event)) {
            mouseDown = false;
        }
    }
</script>

<button on:mousedown={onMouseDown} on:mouseup={onMouseUp}>Draw from direction!</button>
