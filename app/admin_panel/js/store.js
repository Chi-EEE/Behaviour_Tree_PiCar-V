const Store = require('electron-store')

const schema = {
    defaults: {
        behaviour_tree_list: []
    }
}

/** @returns {Store} */
function getStore() {
    const store = new Store(schema);
    return store;
}

module.exports = {
    getStore: getStore,
}