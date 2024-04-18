<script>
    import { lidar_toggle } from "../../store/home_store";
    import { frame_buffer, lidar } from "../../store/websocket_store";

    const lidar_wall_colour = "#0061FF";
    const offset_angle = 45;

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

    /**
     * @param {string} color Hex value format: #ffffff or ffffff
     * @param {number} decimal lighten or darken decimal value, example 0.5 to lighten by 50% or 1.5 to darken by 50%.
     * @returns {string} Hex value of the new color
     */
    function shadeColor(color, decimal) {
        const base = color.startsWith("#") ? 1 : 0;

        let r = parseInt(color.substring(base, 3), 16);
        let g = parseInt(color.substring(base + 2, 5), 16);
        let b = parseInt(color.substring(base + 4, 7), 16);

        r = Math.round(r / decimal);
        g = Math.round(g / decimal);
        b = Math.round(b / decimal);

        r = r < 255 ? r : 255;
        g = g < 255 ? g : 255;
        b = b < 255 ? b : 255;

        const rr =
            r.toString(16).length === 1 ? `0${r.toString(16)}` : r.toString(16);
        const gg =
            g.toString(16).length === 1 ? `0${g.toString(16)}` : g.toString(16);
        const bb =
            b.toString(16).length === 1 ? `0${b.toString(16)}` : b.toString(16);

        return `#${rr}${gg}${bb}`;
    }

    /**
     * @param {number} num
     */
    function round0_25(num) {
        return (Math.round(num * 4) / 4).toFixed(2);
    }

    /**
     * @param {number} x
     * @param {number} lidar_canvas_midpoint_y
     * @param {number} height
     */
    function drawLidarWall(x, lidar_canvas_midpoint_y, distance) {
        const height = 20000 / distance;
        lidar_context_.strokeStyle = shadeColor(
            lidar_wall_colour,
            distance / 15.15151515151515 / 33,
        );
        const path = new Path2D();
        path.moveTo(x, lidar_canvas_midpoint_y + height);
        path.lineTo(x, lidar_canvas_midpoint_y - height);
        lidar_context_.stroke(path);
    }

    function subscribeLidarDrawing() {
        lidar_context_.lineWidth = 6;
        return lidar.subscribe((/** @type {Array<Point>} */ points) => {
            lidar_context_.clearRect(
                0,
                0,
                lidar_canvas_.width,
                lidar_canvas_.height,
            );
            if (points.length === 0) {
                return;
            }
            const space_between_angles = lidar_canvas_.width / 90;
            const lidar_canvas_midpoint_y = lidar_canvas_.height / 2;
            for (let point of points) {
                const angle = point.angle;
                const distance = point.distance;
                if (angle < offset_angle || angle > offset_angle + 90) {
                    continue;
                }
                const x = (angle - offset_angle) * space_between_angles;
                drawLidarWall(x, lidar_canvas_midpoint_y, distance);
            }
            for (let i = 0; i < 90; i += 0.25) {
                if (
                    points.find(
                        (point) => round0_25(point.angle) === i + offset_angle,
                    )
                ) {
                    continue;
                }
                const angle = i + offset_angle;

                // Using i to calculate x, since we are already going to subtract 45 from angle
                const x = i * space_between_angles;

                // Get the closest points to the angle
                let closest_before_point = null;
                let closest_after_point = null;
                for (const point of points) {
                    if (point.angle < angle) {
                        if (
                            closest_before_point === null ||
                            angle - point.angle <
                                angle - closest_before_point.angle
                        ) {
                            closest_before_point = point;
                        }
                    } else if (point.angle > angle) {
                        if (
                            closest_after_point === null ||
                            point.angle - angle <
                                closest_after_point.angle - angle
                        ) {
                            closest_after_point = point;
                        }
                    }
                }

                let distance = 0;
                if (
                    closest_before_point !== null &&
                    closest_after_point !== null
                ) {
                    distance =
                        (closest_before_point.distance +
                            closest_after_point.distance) /
                        2;
                } else if (closest_before_point !== null) {
                    distance = closest_before_point.distance;
                } else if (closest_after_point !== null) {
                    distance = closest_after_point.distance;
                }
                drawLidarWall(x, lidar_canvas_midpoint_y, distance);
            }
        });
    }

    let lidar_draw_disconnect = null;
    function handleLidar(/** @type {HTMLCanvasElement} */ lidar_canvas) {
        lidar_canvas_ = lidar_canvas;
        lidar_context_ = lidar_canvas.getContext("2d");
        lidar_draw_disconnect = subscribeLidarDrawing();
    }

    let lidar_toggle_icon = "favicon.png";
    function toggleLidar() {
        if (
            lidar_draw_disconnect === null ||
            lidar_canvas_ === null ||
            lidar_context_ === null
        ) {
            return;
        }
        lidar_toggle.set(!$lidar_toggle);
        if ($lidar_toggle) {
            lidar_toggle_icon = "favicon.png";
            lidar_draw_disconnect = subscribeLidarDrawing();
        } else {
            lidar_toggle_icon = "greyed_favicon.png";
            lidar_draw_disconnect();
            lidar_context_.clearRect(
                0,
                0,
                lidar_canvas_.width,
                lidar_canvas_.height,
            );
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
    <button class="absolute top-0 right-0" on:click={toggleLidar}>
        <img
            width="60"
            height="60"
            src={lidar_toggle_icon}
            alt="Button to toggle Lidar"
        />
    </button>
</div>
