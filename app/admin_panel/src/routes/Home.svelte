<script>
  import CodeBox from "../lib/CodeBox.svelte";
  import NavigationBar from "../lib/NavigationBar.svelte";
  import RaspberryPiConnectBanner from "../lib/RaspberryPiConnectBanner.svelte";

  import { connected } from "../websocket_server_store";
</script>

<main>
  <NavigationBar />
  {#if $connected}
    <RaspberryPiConnectBanner />
  {/if}
  <div class="py-6">
    <div class="w-full h-full" style="background-color:rgb(230,230,230)">
      <CodeBox />
    </div>
  </div>
</main>
