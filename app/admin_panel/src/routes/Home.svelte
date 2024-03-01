<script>
    import { Banner, Button } from "flowbite-svelte";
    import { ArrowRightOutline } from "flowbite-svelte-icons";

    import NavigationBar from "../lib/NavigationBar.svelte";
</script>

<main>
    <NavigationBar />
    <div class="py-6">
        <Banner
            class="border-2 rounded-lg dark:bg-gray-700 border-gray-500 dark:border-gray-600"
            position="absolute"
            bannerType="signup"
        >
            <div class="flex flex-col justify-center items-center w-full md:flex-row">
                <img
                    src="RaspberryPi.png"
                    class="w-12 whitespace-nowrap"
                />
                <h2 class="font-bold px-4 text-gray-500 dark:text-gray-400">
                    Not Connected!
                </h2>
                <p class="text-gray-500 dark:text-gray-400">
                    Connect the Raspberry Pi to the Admin Panel's websocket
                    server by following instructions in the Websocket Tab or
                    using the button.
                </p>
                <Button href="#/websocket" class="w-fit ml-3">
                    View Instructions
                    <ArrowRightOutline />
                </Button>
            </div>
        </Banner>
    </div>
</main>
