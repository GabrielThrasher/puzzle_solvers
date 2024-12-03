import { startPuzzle, renderPuzzle } from "./puzzle";
import { settings } from "./settings";
import { uploadFile } from "./upload";

const startBtn = document.getElementById("start-btn");
const slowBtn = document.getElementById("slow-btn");
const fastBtn = document.getElementById("fast-btn");
const pauseBtn = document.getElementById("pause-btn");
const uploadBtn = document.getElementById("upload-btn");
const speedState = document.getElementById("speed-state");
const uploadFileInput = document.getElementById("upload-file");
const fileName = document.getElementById("file-name");

const file = {
    data: null,
};

document.addEventListener("dragover", (e) => {
    e.preventDefault();
});

document.addEventListener("drop", (e) => {
    file.data = e.dataTransfer.files[0];
    fileName.innerText = file.data.name;

    uploadFile(file, (edgeData, colorData) => {
        const durationContainer = document.getElementById("duration");
        durationContainer.style.display = "none";
        settings.reset();
        renderPuzzle(edgeData, colorData);
    });

    e.preventDefault();
});

startBtn.addEventListener("click", () => {
    const durationContainer = document.getElementById("duration");
    durationContainer.style.display = "none";
    settings.reset();
    startPuzzle();
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

uploadBtn.addEventListener("click", () => {
    uploadFileInput.click();
});

uploadFileInput.addEventListener("change", (e) => {
    file.data = e.target.files[0];
    fileName.innerText = file.data.name;
    uploadFileInput.src = URL.createObjectURL(file.data);
    uploadFileInput.onload = function () {
        URL.revokeObjectURL(output.src); // free memory
    };
});
