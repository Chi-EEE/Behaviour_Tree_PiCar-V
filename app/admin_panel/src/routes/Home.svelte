<script>
  import CodeBox from "../lib/CodeBox.svelte";
  import NavigationBar from "../lib/NavigationBar.svelte";
  import RaspberryPiConnectBanner from "../lib/RaspberryPiConnectBanner.svelte";

  import { websocket_server_connection_state } from "../store/websocket_store";
</script>

<main>
  <NavigationBar />
  {#if $websocket_server_connection_state}
    <RaspberryPiConnectBanner />
  {/if}
  <div class="py-6">
    <div class="w-full h-full" style="background-color:rgb(230,230,230)">
      <CodeBox />
    </div>
  </div>
</main>
