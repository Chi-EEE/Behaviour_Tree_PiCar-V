<script>
    import { Button, Modal } from "flowbite-svelte";

    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";

    import { xml_schema } from "./CodeBox_Constants";
    import { node_hover } from "./CodeBox_Constants";

    import { show_behaviour_tree_save_modal, behaviour_tree_save_modal_code } from "../store/behaviour_tree_code_store";
</script>

<Modal
    title="Save Behaviour Tree"
    bind:open={$show_behaviour_tree_save_modal}
    autoclose
>
    <p class="text-base leading-relaxed text-gray-500 dark:text-gray-400">
        Would you like to save this behaviour tree?
    </p>

    <CodeMirror
        class="text-left h-full flex-auto font-mono text-lg font-bold"
        bind:value={$behaviour_tree_save_modal_code}
        lang={xml(xml_schema)}
        tabSize={4}
        theme={oneDark}
        extensions={[node_hover]}
    />
    <svelte:fragment slot="footer">
        <Button on:click={() => alert('Handle "success"')}>Yes</Button>
        <Button color="alternative">No</Button>
    </svelte:fragment>
</Modal>
