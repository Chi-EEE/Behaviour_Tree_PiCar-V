<script>
    // If this does not work then try preview
    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";

    import xmlFormat from "xml-formatter";
    import { xml_schema } from "./CodeBox_Constants";
    import { node_hover } from "./CodeBox_Constants";

    /** @type {WebSocket}  */
    export let websocket;

    /** @type {string} */
    let xml_code = "";

    /** @type {string} */
    let send_behaviour_tree_cursor = "pointer";

    /** @type {string} */
    let send_behaviour_tree_text = "Send Behaviour Tree";

    /** @type {string} */
    let send_behaviour_tree_color = "#50AA34";

    /** @type {boolean} */
    let debounce = false;
    function sendXMLCode() {
        if (debounce) {
            return;
        }
        debounce = true;
        try {
            websocket.send(
                JSON.stringify({
                    type: "behaviour_tree",
                    data: xmlFormat.minify(xml_code),
                }),
            );
            send_behaviour_tree_cursor = "not-allowed";
            send_behaviour_tree_text = "Sent Behaviour Tree!";
            send_behaviour_tree_color = "#3457AA";
        } catch (error) {
            send_behaviour_tree_cursor = "not-allowed";
            send_behaviour_tree_text = "Unable to send Behaviour Tree!";
            send_behaviour_tree_color = "#AA3434";
        }
        setTimeout(() => {
            send_behaviour_tree_cursor = "pointer";
            send_behaviour_tree_text = "Send Behaviour Tree";
            send_behaviour_tree_color = "#50AA34";
            debounce = false;
        }, 1000);
    }
</script>

<div class="h-full">
    <CodeMirror
        class="text-left h-full flex-auto font-mono text-lg font-bold"
        bind:value={xml_code}
        lang={xml(xml_schema)}
        tabSize={4}
        theme={oneDark}
        extensions={[node_hover]}
    />
    <style>
        .cm-editor {
            height: 100% !important;
        }
        .cm-editor * {
            font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New" !important;
        }
        .cm-scroller {
            height: 100% !important;
        }
    </style>
    <div
        style="
            position: absolute;
            bottom: 20px;
            right: 40px;
            background-color: {send_behaviour_tree_color};
            color: white;
        "
        class="p-2 rounded-lg shadow-lg"
    >
        <button on:mousedown={sendXMLCode} style="cursor:{send_behaviour_tree_cursor}">{send_behaviour_tree_text}</button>
    </div>
</div>
