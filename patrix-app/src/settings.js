const _settings = {
    started: false,
    paused: false,
    speed: 1,
    pauseStateListeners: [],

    getBatchSize() {
        if (this.speed == 1.5) return 50;
        else if (this.speed === 2) return 100;
        else return 40;
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
};

const settingsHandler = {};

export const settings = new Proxy(_settings, settingsHandler);
