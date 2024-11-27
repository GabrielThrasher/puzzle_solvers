import { startPuzzle } from "./puzzle";
import { settings } from "./settings";

const startBtn = document.getElementById("start-btn");
const slowBtn = document.getElementById("slow-btn");
const fastBtn = document.getElementById("fast-btn");
const pauseBtn = document.getElementById("pause-btn");
const speedState = document.getElementById("speed-state");

startBtn.addEventListener("click", () => {
    if (!settings.started) {
        startPuzzle();
        settings.started = true;
    }
});

slowBtn.addEventListener("click", () => {
    settings.decreaseSpeed();

    let speed = "Normal";
    if (settings.speed !== 1) {
        speed = `${settings.speed}x`;
    }
    speedState.innerText = speed;
});

fastBtn.addEventListener("click", () => {
    settings.increaseSpeed();

    let speed = "Normal";
    if (settings.speed !== 1) {
        speed = `${settings.speed}x`;
    }
    speedState.innerText = speed;
});

pauseBtn.addEventListener("click", () => {
    settings.togglePause();
    if (settings.paused) {
        pauseBtn.innerText = "▶️";
    } else {
        pauseBtn.innerText = "️⏸️";
    }
});
