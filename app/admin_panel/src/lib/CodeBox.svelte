<script>
    // If this does not work then try preview
    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";

    import { node_hover, xml_schema } from "./CodeBox_Constants";

    import { behaviour_tree_xml_code } from "../store/behaviour_tree_code_store";
    import CodeMessagingBar from "./CodeMessagingBar.svelte";
</script>

<div class="h-full m-1">
    <CodeMessagingBar/>
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
