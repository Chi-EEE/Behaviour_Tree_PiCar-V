<script>
    // If this does not work then try preview
    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";

    import xmlFormat from "xml-formatter";
    import { xml_schema } from "./CodeBox_Constants";
    import { node_hover } from "./CodeBox_Constants";

    import { behaviour_tree_xml_code } from "../store/behaviour_tree_code_store";
    import { selected_raspberry_pi_uuid } from "../store/raspberry_pi_store";

    /** @type {string} */
    let send_behaviour_tree_text = "Send Behaviour Tree";

    /** @type {string} */
    let send_behaviour_tree_color = "#50AA34";

    /** @type {boolean} */
    let send_behaviour_tree_debounce = false;

    /** @type {string} */
    let start_behaviour_tree_text = "Start Behaviour Tree";

    /** @type {string} */
    let start_behaviour_tree_color = "#89AAFF";

    /** @type {boolean} */
    let start_behaviour_tree_debounce = false;

    async function sendBehaviourTree() {
        if (send_behaviour_tree_debounce) {
            return;
        }
        send_behaviour_tree_debounce = true;
        try {
            await api.sendBehaviourTree({
                data: xmlFormat.minify($behaviour_tree_xml_code),
            });
            send_behaviour_tree_text = "Sent Behaviour Tree!";
            send_behaviour_tree_color = "#3457AA";
        } catch (error) {
            send_behaviour_tree_text = "Unable to send Behaviour Tree!";
            send_behaviour_tree_color = "#AA3434";
        }
        setTimeout(() => {
            send_behaviour_tree_text = "Send Behaviour Tree";
            send_behaviour_tree_color = "#50AA34";
            send_behaviour_tree_debounce = false;
        }, 1000);
    }

    async function startBehaviourTree() {
        if (start_behaviour_tree_debounce) {
            return;
        }
        start_behaviour_tree_debounce = true;
        try {
            await api.startBehaviourTree();
            start_behaviour_tree_text = "Started Behaviour Tree!";
            start_behaviour_tree_color = "#4277FF";
        } catch (error) {
            start_behaviour_tree_text = "Unable to start Behaviour Tree!";
            start_behaviour_tree_color = "#AA3434";
        }
        setTimeout(() => {
            start_behaviour_tree_text = "Start Behaviour Tree";
            start_behaviour_tree_color = "#89AAFF";
            start_behaviour_tree_debounce = false;
        }, 1000);
    }

    async function stopBehaviourTree() {
        await api.stopBehaviourTree();
    }
</script>

<div class="h-full m-1">
    <div class="grid grid-cols-3 gap-2">
        <button
            on:mousedown={sendBehaviourTree}
            class="p-2 rounded-lg shadow-lg relative inset-0"
            style="background-color: {send_behaviour_tree_color}; color: white; width: 100%; border: none;{$selected_raspberry_pi_uuid === "" ? "cursor: not-allowed;" : ""}"
            disabled={$selected_raspberry_pi_uuid === ""}
            >{send_behaviour_tree_text}</button
        >
        <button
            on:mousedown={startBehaviourTree}
            class="p-2 rounded-lg shadow-lg relative inset-0"
            style="background-color: {start_behaviour_tree_color}; color: white; width: 100%; border: none; {$selected_raspberry_pi_uuid === "" ? "cursor: not-allowed;" : ""}"
            disabled={$selected_raspberry_pi_uuid === ""}
            >{start_behaviour_tree_text}</button
        >
        <button
            on:mousedown={stopBehaviourTree}
            class="p-2 rounded-lg shadow-lg relative inset-0"
            style="background-color: #FF352F; color: white; width: 100%; border: none; {$selected_raspberry_pi_uuid === "" ? "cursor: not-allowed;" : ""}"
            disabled={$selected_raspberry_pi_uuid === ""}
            >Stop Behaviour Tree</button
        >
    </div>
    <CodeMirror
        class="text-left h-full flex-auto font-mono text-lg font-bold"
        bind:value={$behaviour_tree_xml_code}
        lang={xml(xml_schema)}
        tabSize={4}
        theme={oneDark}
        extensions={[node_hover]}
    />
    <style>
        .cm-editor {
            height: 90% !important;
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
