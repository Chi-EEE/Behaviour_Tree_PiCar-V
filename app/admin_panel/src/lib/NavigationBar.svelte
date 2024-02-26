<script>
    import {
        DarkMode,
        Navbar,
        NavLi,
        NavUl,
        NavHamburger,
    } from "flowbite-svelte";

    let btnClass =
        "text-gray-500 dark:text-gray-400 hover:bg-gray-100 dark:hover:bg-gray-700 rounded-lg text-xl p-2";
</script>

<Navbar>
    <NavUl>
        <NavLi>
            <a href="/admin_panel">Home</a>
        </NavLi>
        <NavLi>
            <a href="/admin_panel/settings">Settings</a>
        </NavLi>
        <NavLi>
            <a href="/admin_panel/logout">Logout</a>
        </NavLi>
    </NavUl>
    <NavHamburger class="lg:hidden" />
    <DarkMode {btnClass} />
</Navbar>
