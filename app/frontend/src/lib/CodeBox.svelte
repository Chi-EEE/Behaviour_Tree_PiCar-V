<script>
    // If this does not work then try preview
    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";
    
    import xmlFormat from "xml-formatter";
    import { xml_schema } from "./CodeBox_Constants";
    import { node_hover } from "./CodeBox_Constants";

    /**
     * @type {WebSocket}
     */
    export let websocket;

    /**
     * @type {string}
     */
    let xml_code = "";

    function sendXMLCode() {
        websocket.send(
            JSON.stringify({
                type: "behaviour_tree",
                data: xmlFormat.minify(xml_code),
            }),
        );
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
        .cm-scroller {
            height: 100% !important;
        }
    </style>
    <div
        style="
            position: absolute;
            bottom: 20px;
            right: 30px;
            background-color: #50AA34;
            color: white;
        "
        class="p-2 rounded-lg shadow-lg"
    >
        <button on:mousedown={sendXMLCode}>Send Behaviour Tree!</button>
    </div>
</div>
