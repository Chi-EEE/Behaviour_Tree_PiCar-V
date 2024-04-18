<script>
    import { frame_buffer, lidar } from "../../store/websocket_store";

    /**
     * @typedef {{
     *    angle: number,
     *   distance: number
     * }} Point
    */
   
    // From: https://stackoverflow.com/a/16245768
    /**
     *
     * @param {string} b64Data
     * @param {string} contentType
     * @param {number} sliceSize
     */
    function b64toBlob(b64Data, contentType = "", sliceSize = 512) {
        const byteCharacters = atob(b64Data);
        const byteArrays = [];

        for (
            let offset = 0;
            offset < byteCharacters.length;
            offset += sliceSize
        ) {
            const slice = byteCharacters.slice(offset, offset + sliceSize);

            const byteNumbers = new Array(slice.length);
            for (let i = 0; i < slice.length; i++) {
                byteNumbers[i] = slice.charCodeAt(i);
            }

            const byteArray = new Uint8Array(byteNumbers);
            byteArrays.push(byteArray);
        }

        const blob = new Blob(byteArrays, { type: contentType });
        return blob;
    }
    let zoom = 0.3;

    function handleLiveFeed(/** @type {HTMLImageElement} */ live_feed) {
        frame_buffer.subscribe((value) => {
            const blob = b64toBlob(value, "image/jpeg");
            const image_url = URL.createObjectURL(blob);
            live_feed.src = image_url;
        });
    }

    /** @type {HTMLCanvasElement}*/
    let lidar_canvas_ = null;

    /** @type {CanvasRenderingContext2D} */
    let lidar_context_ = null;

    let lidar_draw_disconnect = null;
    function handleLidar(/** @type {HTMLCanvasElement} */ lidar_canvas) {
        lidar_canvas_ = lidar_canvas;
        lidar_context_ = lidar_canvas.getContext("2d");
        lidar_draw_disconnect = lidar.subscribe((/** @type {Array<Point>} */ points) => {
            lidar_context_.clearRect(0, 0, lidar_canvas.width, lidar_canvas.height);
            for (const point of points) {
                const angle = point.angle;
                const distance = point.distance * zoom;
                const angleInRadians = angle * (3.14159265 / 180.0);
                const x = distance * Math.cos(angleInRadians);
                const y = distance * Math.sin(angleInRadians);
                lidar_context_.fillRect(
                    x + lidar_canvas.width / 2,
                    y + lidar_canvas.height / 2,
                    2,
                    2,
                );
            }
        });
        
    }

    let lidar_toggle = true;
    function toggleLidar() {
        if (lidar_draw_disconnect === null || lidar_canvas_ === null || lidar_context_ === null) {
            return;
        }
        lidar_toggle = !lidar_toggle;
        if (lidar_toggle) {
            lidar_draw_disconnect = lidar.subscribe((/** @type {Array<Point>} */ points) => {
                lidar_context_.clearRect(0, 0, lidar_canvas_.width, lidar_canvas_.height);
                for (const point of points) {
                    const angle = point.angle;
                    const distance = point.distance * zoom;
                    const angleInRadians = angle * (3.14159265 / 180.0);
                    const x = distance * Math.cos(angleInRadians);
                    const y = distance * Math.sin(angleInRadians);
                    lidar_context_.fillRect(
                        x + lidar_canvas_.width / 2,
                        y + lidar_canvas_.height / 2,
                        2,
                        2,
                    );
                }
            });
        } else {
            lidar_draw_disconnect();
            lidar_context_.clearRect(0, 0, lidar_canvas_.width, lidar_canvas_.height);
        }
    }
</script>

<div class="relative w-full h-full">
    <!-- svelte-ignore a11y-missing-attribute -->
    <img
        class="absolute top-0 left-0 right-0 bottom-0 w-full h-full"
        src="Warning.png"
        use:handleLiveFeed
    />
    <canvas
        class="absolute top-0 left-0 right-0 bottom-0 w-full h-full"
        use:handleLidar
    />
    <button 
    class="absolute top-0 right-0"
    on:click={toggleLidar}>
        <img alt="Button to toggle Lidar"/>
    </button>
</div>
