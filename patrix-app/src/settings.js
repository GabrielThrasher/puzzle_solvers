export const settings = {
    started: false,
    paused: false,
    speed: 1,
    pauseStateListeners: [],
    resetListeners: [],

    start() {
        this.started = true;
    },

    reset() {
        for (let listener of this.resetListeners) {
            listener();
        }

        this.paused = false;
        this.pauseStateListeners = [];
    },

    getBatchSize() {
        if (this.speed == 1.5) return 200;
        else if (this.speed === 2) return 300;
        else return 100;
    },

    increaseSpeed() {
        this.speed = Math.min(this.speed + 0.5, 2);
    },

    decreaseSpeed() {
        this.speed = Math.max(this.speed - 0.5, 1);
    },

    togglePause() {
        this.paused = !this.paused;
        for (let listener of this.pauseStateListeners) {
            listener(this.paused);
        }
    },

    listenToPauseState(cb) {
        this.pauseStateListeners.push(cb);
    },

    listenToReset(cb) {
        this.resetListeners.push(cb);
    },
};
