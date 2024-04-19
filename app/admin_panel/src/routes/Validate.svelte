<script>
    import BehaviourTreeHandler from "../lib/BehaviourTreeHandler.svelte";

    import {
        side_pane_size_0_store,
        side_pane_size_1_store,
    } from "../store/home_store";

    import { Pane, Splitpanes } from "svelte-splitpanes";
    import SaveBehaviourTreeModal from "../lib/SaveBehaviourTreeModal.svelte";
    import CodeBox from "../lib/validate/CodeBox.svelte";

    let side_pane_size_0 = $side_pane_size_0_store;
    let side_pane_size_1 = $side_pane_size_1_store;

    function onSideResize(event) {
        side_pane_size_0_store.set(event.detail[0].size);
        side_pane_size_1_store.set(event.detail[1].size);
    }

</script>

<main style="height: 92vh">
    <SaveBehaviourTreeModal />
    <Splitpanes
        style="height:100%;padding:1em;background-color:rgb(20,20,40)"
        on:resize={onSideResize}
    >
        <Pane size={side_pane_size_0}>
            <CodeBox />
        </Pane>
        <Pane size={side_pane_size_1} minSize={17} maxSize={80}>
            <BehaviourTreeHandler />
        </Pane>
    </Splitpanes>
</main>
