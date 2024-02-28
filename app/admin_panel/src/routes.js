import Home from './routes/Home.svelte';
import NotFound from './routes/NotFound.svelte';

export default {
    '/': Home,
    // The catch-all route must always be last
    '*': NotFound
};
