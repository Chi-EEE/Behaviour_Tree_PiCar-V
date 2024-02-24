<script>
    import { Button } from "bits-ui";

    export let name;

    let ip_list = [];

    const updateIPList = api.getLocalIPList().then((local_ip_list) =>{
        ip_list.length = 0;
        console.log(local_ip_list)
        for (const [ip_group_name, ip_group] of Object.entries(local_ip_list)) {
            for (const ip of ip_group) {
                if (ip.family === "IPv4") {
                    ip_list.push({
                        group_name: ip_group_name,
                        address: ip.address,
                        netmask: ip.netmask,
                        mac: ip.mac,
                        internal: ip.internal,
                        cidr: ip.cidr,
                    });
                }
            }
        }
        console.log(ip_list)
    });
</script>

<main>
    <!-- svelte-ignore missing-declaration -->
    {#await updateIPList}
        <p>...loading</p>
    {:then _}
    {#each ip_list as ip}
    <div>
        <p>{ip.group_name}</p>
        <p>{ip.address}</p>
        <!-- <p>{ip.netmask}</p>
        <p>{ip.mac}</p>
        <p>{ip.internal}</p>
        <p>{ip.cidr}</p> -->
    </div>
    {/each}
    {/await}
    <h1>Hello {name}!</h1>
    <p>
        Visit the <a href="https://svelte.dev/tutorial">Svelte tutorial</a> to learn
        how to build Svelte apps.
    </p>
</main>

<style>
    main {
        text-align: center;
        padding: 1em;
        max-width: 240px;
        margin: 0 auto;
    }

    h1 {
        color: #ff3e00;
        text-transform: uppercase;
        font-size: 4em;
        font-weight: 100;
    }

    @media (min-width: 640px) {
        main {
            max-width: none;
        }
    }
</style>
