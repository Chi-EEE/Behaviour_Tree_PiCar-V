<script>
    import { Card } from "flowbite-svelte";

    import LocalServerSettings from "./lib/LocalServerSettings.svelte";
    import IPList from "./lib/IPList.svelte";
    import NavigationBar from "./lib/NavigationBar.svelte";
</script>

<main>
    <NavigationBar />
    <div class="grid grid-cols-2">
        <div>
            <LocalServerSettings />
        </div>
        <div class="flex justify-end">
            <Card class="p-6">
                <h1>Local IP List</h1>
                <br />
                <IPList />
            </Card>
        </div>
    </div>
</main>

<style>
    main {
        text-align: center;
        padding: 1em;
        max-width: 240px;
        margin: 0 auto;
    }

    @media (min-width: 640px) {
        main {
            max-width: none;
        }
    }
</style>
