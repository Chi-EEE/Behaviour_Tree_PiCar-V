<script>
  import {
    DarkMode,
    Navbar,
    NavLi,
    NavUl,
    NavHamburger,
  } from "flowbite-svelte";

  let btnClass =
    "text-gray-500 dark:text-gray-400 hover:bg-gray-100 dark:hover:bg-gray-700 rounded-lg text-xl p-2"
</script>

<Navbar>
  <NavUl>
    <NavLi>
      <a href="#/">Home</a>
    </NavLi>
    <NavLi>
      <a href="#/websocket">Manage Websockets</a>
    </NavLi>
  </NavUl>
  <NavHamburger class="lg:hidden" />
  <DarkMode {btnClass} />
</Navbar>
