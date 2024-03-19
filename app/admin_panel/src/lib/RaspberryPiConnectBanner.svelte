<script>
  import { Banner, Button } from "flowbite-svelte";
  import { ArrowRightOutline } from "flowbite-svelte-icons";
</script>

<Banner
  class="border-2 z-20 flex-auto p-1 fixed bottom-3 rounded-lg bg-white dark:bg-gray-700 border-gray-500 dark:border-gray-600"
  bannerType="info"
  style="left:50%; transform: translateX(-50%);"
>
  <div class="flex flex-col justify-center items-center w-full md:flex-row">
    <img src="RaspberryPi.png" class="w-12 whitespace-nowrap" />
    <h2 class="font-bold px-4 text-gray-500 dark:text-gray-400">
      Not Connected!
    </h2>
    <p class="text-gray-500 dark:text-gray-400">
      Connect the Raspberry Pi to the Admin Panel's websocket server by
      following instructions in the Websocket Tab.
    </p>
    <Button href="#/websocket" class="w-fit ml-3">
      View Instructions
      <ArrowRightOutline />
    </Button>
  </div>
</Banner>
