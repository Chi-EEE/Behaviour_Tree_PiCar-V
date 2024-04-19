<script>
    import NavigationBar from "./lib/NavigationBar.svelte";
    import Router from "svelte-spa-router";
    import routes from "./routes";
</script>

<NavigationBar />
<main>
    <Router {routes} />
</main>

<style>
    main {
        text-align: center;
        max-width: 240px;
        margin: 0 auto;
        height: 92vh;
    }

    @media (min-width: 640px) {
        main {
            max-width: none;
        }
    }
</style>
