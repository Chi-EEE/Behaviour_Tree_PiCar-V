<script>
    import { Button, Card } from "flowbite-svelte";

    let behaviour_tree_list = [];
</script>

<div class="w-full h-full bg-white dark:bg-gray-800">
    <Button class="w-full my-3" color="green">Save Behaviour Tree</Button>
    <Card
        class="w-full max-w-full h-full max-h-full overflow-scroll my-4 gap-y-2"
    >
        {#each behaviour_tree_list as a}
            <Card class="w-full max-w-full">
                <h3>
                    {a}
                </h3>
                <Button color="blue">Load</Button>
            </Card>
        {/each}
    </Card>
</div>
