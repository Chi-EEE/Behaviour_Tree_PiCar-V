
// https://stackoverflow.com/a/12737882
export function detectLeftButton(event: MouseEvent) {
    if (event.metaKey || event.ctrlKey || event.altKey || event.shiftKey) {
        return false;
    } else if (event["which"] !== undefined) {
        return event["which"] === 1;
    } else if (event["buttons"] !== undefined) {
        return event["buttons"] === 1;
    } else {
        return event["button"] == 1 || event["type"] == "click";
    }
}

// https://stackoverflow.com/a/33063222
export function getMousePos(event: MouseEvent) {
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