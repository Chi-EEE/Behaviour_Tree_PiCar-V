<script>
  import { Renderer, Container, Ticker, Text } from "svelte-pixi";
  import { Button, Card } from "flowbite-svelte";
  import { ArrowRightOutline } from "flowbite-svelte-icons";

  let renderer;
  let stage;

  import NavigationBar from "../lib/NavigationBar.svelte";
  import DraggableCircle from "../lib/DraggableCircle.svelte";
</script>

<main>
  <NavigationBar />
  <div class="py-6">
    <Card
      class="w-screen px-3 flex items-start justify-start"
      style="max-width:100%;"
    >
      <img src="RaspberryPi.png" class="item-body w-12" />
      <h2 class="m-1">Raspberry Pi not connected to Admin Panel</h2>
      <p class="m-2">
        Currently, the Raspberry Pi is not connected to the Admin Panel's
        websocket server. Please view the Websocket Tab or the button below for
        instructions on how to connect the Raspberry Pi to the Admin Panel.
      </p>
      <Button href="#/websocket" class="w-fit">
        View Instructions
        <ArrowRightOutline />
      </Button>
    </Card>
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
