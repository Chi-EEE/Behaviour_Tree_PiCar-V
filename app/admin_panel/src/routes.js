import Home from './routes/Home.svelte';
import Websocket from './routes/Websocket.svelte';
import NotFound from './routes/NotFound.svelte';

export default {
    '/': Home,
    '/websocket': Websocket,
    // The catch-all route must always be last
    '*': NotFound
};
