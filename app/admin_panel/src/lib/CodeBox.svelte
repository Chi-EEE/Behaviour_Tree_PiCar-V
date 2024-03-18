<script>
    // If this does not work then try preview
    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";

    import xmlFormat from "xml-formatter";
    import { xml_schema } from "./CodeBox_Constants";
    import { node_hover } from "./CodeBox_Constants";

    /** @type {string} */
    let xml_code = "";

    /** @type {string} */
    let send_behaviour_tree_text = "Send Behaviour Tree";

    /** @type {string} */
    let send_behaviour_tree_color = "#50AA34";

    /** @type {boolean} */
    let debounce = false;
    async function sendXMLCode() {
        if (debounce) {
            return;
        }
        debounce = true;
        try {
            await api.sendBehaviourTree(
                JSON.stringify({
                    data: xmlFormat.minify(xml_code),
                }),
            );
            send_behaviour_tree_text = "Sent Behaviour Tree!";
            send_behaviour_tree_color = "#3457AA";
        } catch (error) {
            send_behaviour_tree_text = "Unable to send Behaviour Tree!";
            send_behaviour_tree_color = "#AA3434";
        }
        setTimeout(() => {
            send_behaviour_tree_text = "Send Behaviour Tree";
            send_behaviour_tree_color = "#50AA34";
            debounce = false;
        }, 1000);
    }

    async function startBehaviourTree() {
        await api.startBehaviourTree();
    }
</script>

<div class="h-full">
    <div class="grid grid-cols-2 gap-2">
        <button
            on:mousedown={sendXMLCode}
            class="p-2 rounded-lg shadow-lg relative inset-0"
            style="background-color: {send_behaviour_tree_color}; color: white; width: 100%; border: none;"
            >{send_behaviour_tree_text}</button
        >
        <button
            on:mousedown={startBehaviourTree}
            class="p-2 rounded-lg shadow-lg relative inset-0"
            style="background-color: blue; color: white; width: 100%; border: none;"
            >Start Behaviour Tree</button
        >
    </div>
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
            font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas,
                "Liberation Mono", "Courier New" !important;
        }
        .cm-scroller {
            height: 100% !important;
        }
    </style>
</div>
