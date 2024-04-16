<script>
    import { Button, Card } from "flowbite-svelte";

    import {
        behaviour_tree_xml_code,
        behaviour_tree_save_modal_code,
        show_behaviour_tree_save_modal,
    } from "../store/behaviour_tree_code_store";

    let behaviour_tree_list = []
    async function main() {
        behaviour_tree_list = await api.getBehaviourTreeList();
    }
    main()
</script>

<div class="w-full h-full bg-white dark:bg-gray-800">
    <Button
        on:click={() => {
            show_behaviour_tree_save_modal.set(true);
            behaviour_tree_save_modal_code.set($behaviour_tree_xml_code);
        }}
        class="w-full my-3"
        color="green">Save Behaviour Tree</Button
    >
    <Card
        class="w-full max-w-full h-full max-h-full overflow-scroll my-4 gap-y-2"
    >
        {#each behaviour_tree_list as behaviour_tree}
            <Card class="w-full max-w-full">
                <h3>
                    {behaviour_tree.name === "" ? "[Empty Name]" : behaviour_tree.name}
                </h3>
                <Button color="blue" on:click={()=>{
                    $behaviour_tree_xml_code = behaviour_tree.code;
                }}>Load</Button>
            </Card>
        {/each}
    </Card>
</div>
