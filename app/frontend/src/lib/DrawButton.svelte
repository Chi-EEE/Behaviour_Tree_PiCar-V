<script lang="ts">
    import { FPS, LINE_PATH_DRAWING_SPEED } from "./Globals";
    import { detectLeftButton } from "./Mouse";
    import { positionX, positionY, angleInRadians } from "./PathStore";

    export let canvas: HTMLCanvasElement;
    export let zoom: number;

    let mouseDown: boolean = false;

    const distanceToTravel = LINE_PATH_DRAWING_SPEED * zoom;

    function update() {
        const halfCanvasWidth = canvas.width / 2;
        const halfCanvasHeight = canvas.height / 2;

        const xVector: number = Math.cos($angleInRadians);
        const yVector: number = Math.sin($angleInRadians);

        const magnitude: number = Math.sqrt(
            xVector * xVector + yVector * yVector,
        );

        const unitVectorX: number = xVector / magnitude;
        const unitVectorY: number = yVector / magnitude;

        const deltaTimeDistance = distanceToTravel / FPS;

        $positionX = $positionX + unitVectorX * deltaTimeDistance;
        $positionY = $positionY + unitVectorY * deltaTimeDistance;

        const context = canvas.getContext("2d")!!;
        context.fillStyle = "black";
        context.fillRect(
            $positionX + halfCanvasWidth - 2.5,
            $positionY + halfCanvasHeight - 2.5,
            5,
            5,
        );
    }

    // Prevent mouse down from being triggered when the user leaves the window
    window.addEventListener('mouseout', () => mouseDown = false);

    function onMouseDown(event: MouseEvent) {
        if (detectLeftButton(event)) {
            mouseDown = true;
            let intervalId: number;
            intervalId = setInterval(() => {
                if (!mouseDown) {
                    clearInterval(intervalId);
                    return;
                }
                update();
            }, 1000 / FPS);
        }
    }

    function onMouseUp(event: MouseEvent) {
        if (detectLeftButton(event)) {
            mouseDown = false;
        }
    }
</script>

<button on:mousedown={onMouseDown} on:mouseup={onMouseUp}>Draw from direction!</button>
