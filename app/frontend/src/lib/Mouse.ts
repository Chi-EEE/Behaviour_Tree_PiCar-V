
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