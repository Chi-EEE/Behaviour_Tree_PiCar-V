<script>
    import { Button, Input, Modal } from "flowbite-svelte";

    import CodeMirror from "svelte-codemirror-editor";
    import { xml } from "@codemirror/lang-xml";
    import { oneDark } from "@codemirror/theme-one-dark";

    import { xml_schema } from "./CodeBox_Constants";
    import { node_hover } from "./CodeBox_Constants";

    import {
        show_behaviour_tree_save_modal,
        behaviour_tree_save_modal_code,
        behaviour_tree_list,
    } from "../store/behaviour_tree_store";

    let behaviour_tree_name = "";
</script>

<Modal
    title="Save Behaviour Tree"
    bind:open={$show_behaviour_tree_save_modal}
    autoclose
>
    <p class="text-base leading-relaxed text-gray-500 dark:text-gray-400">
        Would you like to save this behaviour tree?
    </p>

    <Input let:props>
        <input
            type="text"
            {...props}
            placeholder="Behaviour Tree Name:"
            bind:value={behaviour_tree_name}
            required
        />
    </Input>
    <CodeMirror
        class="text-left h-full flex-auto font-mono text-lg font-bold"
        bind:value={$behaviour_tree_save_modal_code}
        lang={xml(xml_schema)}
        tabSize={4}
        theme={oneDark}
        extensions={[node_hover]}
    />
    <svelte:fragment slot="footer">
        <Button
            on:click={() => {
                const behaviour_tree_save_state = {
                    name: behaviour_tree_name,
                    code: $behaviour_tree_save_modal_code,
                };
                api.saveBehaviourTree(behaviour_tree_save_state);
                behaviour_tree_list.set([
                    ...$behaviour_tree_list,
                    behaviour_tree_save_state,
                ]);
            }}>Yes</Button
        >
        <Button color="alternative">No</Button>
    </svelte:fragment>
</Modal>
