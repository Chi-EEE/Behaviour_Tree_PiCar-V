<script>
  import { Renderer, Container, Ticker, Text } from "svelte-pixi";

  let renderer;
  let stage;

  import NavigationBar from "../lib/NavigationBar.svelte";
  import DraggableCircle from "../lib/DraggableCircle.svelte";
</script>

<main>
  <NavigationBar />

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
