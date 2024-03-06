<script>
  import CodeBox from "../lib/CodeBox.svelte";
  import NavigationBar from "../lib/NavigationBar.svelte";
  import RaspberryPiConnectBanner from "../lib/RaspberryPiConnectBanner.svelte";
  import { Pane, Splitpanes } from "svelte-splitpanes";
</script>

<main>
  <NavigationBar />
  <RaspberryPiConnectBanner />
  <div class="py-6">
    <div class="w-full h-full" style="background-color:rgb(230,230,230)">
      <CodeBox />
    </div>
  </div>
</main>
