const Store = require('electron-store')

const schema = {
    defaults: {
        behaviour_tree_list: []
    }
}

function getStore() {
    const store = new Store(schema);
    return store;
}

module.exports = {
    getStore: getStore,
}