<script>
    import { Button, Card } from "flowbite-svelte";

    import {
        raspberry_pi_uuid_list,
        selected_raspberry_pi_uuid,
    } from "../store/raspberry_pi_store";

    /** @type {string} */
    let selected_uuid = "";

    async function connectToRaspberryPi() {
        if (selected_uuid === "") {
            return;
        }
        console.log(selected_uuid);
        const selection_result = await api.selectRaspberryPi({
            uuid: selected_uuid,
        });
        console.log(selection_result);
        if (selection_result.success) {
            selected_raspberry_pi_uuid.set(selected_uuid);
        } else {
            console.log(selection_result.message);
        }
    }
</script>

<div>
    <h1 class="text-gray-900 dark:text-white text-2xl font-bold">
        Raspberry Pi UUID List
    </h1>
    <div class="grid-cols-3 grid my-2">
        <div
            class="content-center col-start-1 col-span-2 grid grid-cols-2 gap-2 p-3 border-2 rounded border-gray-200 dark:border-gray-700"
        >
            {#each $raspberry_pi_uuid_list as uuid}
                <Card>
                    <Button
                        on:click={() => {
                            selected_uuid = uuid;
                        }}
                    >
                        {uuid}
                    </Button>
                </Card>
            {/each}
        </div>
        <Card class="col-start-3 w-full ml-2">
            <h2 class="text-gray-900 dark:text-white text-2xl font-bold mb-2">
                Connect to Raspberry Pi UUID
            </h2>
            <Button on:click={connectToRaspberryPi}>
                {#if selected_uuid === ""}
                    Select a Raspberry Pi UUID
                {:else}
                    Connect to
                    <br />
                    [{selected_uuid}]
                {/if}
            </Button>
        </Card>
    </div>
</div>
