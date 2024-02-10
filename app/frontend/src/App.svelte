<script>
    import Router from "svelte-spa-router";
    import routes from "./routes";
</script>

<main>
    <Router {routes} />
</main>

<style>
    main {
        text-align: center;
        max-width: 240px;
        margin: 0 auto;
    }

    main {
        height: 100vh;
        background-color: rgb(20, 20, 40);
    }

    @media (min-width: 640px) {
        main {
            max-width: none;
        }
    }
</style>
