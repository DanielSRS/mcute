const storage = {
  setItem: (key, item) => {
    storage[key] = item;
  },
  getItem: key => storage[key],
  removeItem: key => {
    delete storage[key];
  },
};

global.localStorage = storage;

export default storage;
