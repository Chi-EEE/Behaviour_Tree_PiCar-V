
<script>
    import { Spinner } from "flowbite-svelte";
    import { copy } from "svelte-copy";
    
    import { websocket_server_port } from "../websocket_server_store";
        
    let ip_list = [];

    if (api.getLocalIPList === undefined) {
        api.getLocalIPList = async function () {
            return new Promise((resolve) => {
                resolve();
            });
        };
    }

    const getIPList = updateIPList();

    function updateIPList() {
        return api.getLocalIPList().then((local_ip_list) => {
            ip_list.length = 0;
            console.log(local_ip_list);
            for (const [ip_group_name, ip_group] of Object.entries(
                local_ip_list,
            )) {
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
            console.log(ip_list);
        });
    }    
</script>
<div {...$$restProps}>
    <!-- svelte-ignore missing-declaration -->
    {#await getIPList}
        <p>Loading IP List</p>
        <Spinner />
    {:then _}
        <ul>
            {#each ip_list as ip}
                <figure class="p-6 border-neutral-100 border-b border-t">
                    <h2
                        class="mb-2 text-2xl font-bold tracking-tight text-gray-900 dark:text-white"
                    >
                        {ip.group_name}
                    </h2>
                    <p
                        class="font-normal text-gray-700 dark:text-gray-400 leading-tight"
                    >
                        {ip.address}<b>:{$websocket_server_port}</b>
                    </p>
                    <button use:copy={`${ip.address}:${$websocket_server_port}`} class="mt-4"> Copy IP </button>
                </figure>
            {/each}
        </ul>
    {/await}
    
</div>