<script>
  import { Renderer, Container, Ticker, Text } from "svelte-pixi";
  import { Banner, Button, Card } from "flowbite-svelte";
  import { ArrowRightOutline } from "flowbite-svelte-icons";

  let renderer;
  let stage;

  import NavigationBar from "../lib/NavigationBar.svelte";
  import DraggableCircle from "../lib/DraggableCircle.svelte";
</script>

<main>
  <NavigationBar />
  <div class="py-6">
    <Banner class="border-4 content-center items-center" position="absolute" bannerType="cta">
      <img src="RaspberryPi.png" class="item-body m-2 w-12" />
      <h2 class="px-4">Not Connected!</h2>
      <p>
        Connect the Raspberry Pi to the Admin Panel's websocket server by
        following instructions in the Websocket Tab or using the button.
      </p>
      <Button href="#/websocket" class="w-fit ml-3">
        View Instructions
        <ArrowRightOutline />
      </Button>
    </Banner>
  </div>
  <Renderer
    bind:instance={renderer}
    bind:stage
    width={400}
    height={400}
    antialias
  >
    <!-- use our own ticker to control the render loop -->
    <Ticker
      on:tick={() => {
        renderer.render(stage);
      }}
    >
      <!--
    the root container is referred to as the "stage", child components
    can access it with getStage()
  -->
      <Container bind:instance={stage}>
        <Text
          x={200}
          y={300}
          text="Click and drag"
          style={{ fill: "white" }}
          anchor={0.5}
        />
        <DraggableCircle x={200} y={200} />
      </Container>
    </Ticker>
  </Renderer>
</main>
