<script>
    import { Button, Spinner, Tooltip } from "flowbite-svelte";
    import { copy } from "svelte-copy";

    import { websocket_server_port } from "../store/websocket_store";

    if (api.getLocalIPList === undefined) {
        api.getLocalIPList = async function () {
            return new Promise((resolve) => {
                resolve([]);
            });
        };
    }

    export let getIPList = updateIPList();

    function refreshIPList() {
        getIPList = updateIPList();
    }

    function updateIPList() {
        return api.getLocalIPList().then((local_ip_list) => {
            let ip_list = [];
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
            return ip_list;
        });
    }
</script>

<div>
    <h2 class="text-2xl font-bold text-gray-900 dark:text-white">Local IP List</h2>
    <br />
    <Button class="mb-3" id="refresh_ip_address" on:click={refreshIPList}>Refresh IP List</Button>
    <Tooltip triggeredBy="#refresh_ip_address">Remember to determine the appropriate IP Address based on the Raspberry Pi's network connection.</Tooltip>
    <br />
    <div>
        <!-- svelte-ignore missing-declaration -->
        {#await getIPList}
            <p>Loading IP List</p>
            <Spinner />
        {:then ip_list}
            <ul>
                {#each ip_list as ip}
                    <figure class="p-6 border-neutral-100 border-b border-t">
                        <h3
                            class="mb-2 text-lg font-bold tracking-tight text-gray-900 dark:text-white"
                        >
                            {ip.group_name}
                        </h3>
                        <p
                            class="font-normal text-gray-700 dark:text-gray-400 leading-tight"
                        >
                            {ip.address}<b>:{$websocket_server_port}</b>
                        </p>
                        <button
                            use:copy={`${ip.address}:${$websocket_server_port}`}
                            class="mt-4 p-2"
                        >
                            Copy IP
                        </button>
                    </figure>
                {/each}
            </ul>
        {/await}
    </div>    
</div>