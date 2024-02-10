import Home from './routes/Home.svelte';
import Room from './routes/Room.svelte';
import NotFound from './routes/NotFound.svelte';

export default {
    '/': Home,
    '/room/:room': Room,
    // The catch-all route must always be last
    '*': NotFound
};
