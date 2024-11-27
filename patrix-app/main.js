import { startPuzzle } from "./puzzle";

const startBtn = document.getElementById("start-btn");
const slowBtn = document.getElementById("slow-btn");
const fastBtn = document.getElementById("fast-btn");
const pauseBtn = document.getElementById("pause-btn");

let hasStarted = false;

startBtn.addEventListener("click", () => {
    if (!hasStarted) {
        startPuzzle();
        hasStarted = true;
    }
});

slowBtn.addEventListener("click", () => {});

fastBtn.addEventListener("click", () => {});

pauseBtn.addEventListener("click", () => {});
