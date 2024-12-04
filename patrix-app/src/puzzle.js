import { settings } from "./settings";

const defaultPuzzles = {
    edge: null,
    color: null,
};

const times = {
    edge: null,
    color: null,
};

async function fetchPuzzle() {
    try {
        const [edgeResponse, colorResponse] = await Promise.all([
            fetch("http://127.0.0.1:8000/default-puzzle-edge"),
            fetch("http://127.0.0.1:8000/default-puzzle-color"),
        ]);

        if (!edgeResponse.ok || !colorResponse.ok) {
            throw new Error("request failed");
        }

        const edgeData = await edgeResponse.arrayBuffer();
        const colorData = await colorResponse.arrayBuffer();

        defaultPuzzles.edge = new Uint16Array(edgeData);
        defaultPuzzles.color = new Uint16Array(colorData);
    } catch (error) {
        console.error(error);
    }
}

export async function startPuzzle() {
    if (!defaultPuzzles.color && !defaultPuzzles.edge) {
        await fetchPuzzle();
    }

    renderPuzzle(defaultPuzzles.edge, defaultPuzzles.color);
}

export function renderPuzzle(edgeData, colorData) {
    times.edge = edgeData[edgeData.length - 1];
    times.color = colorData[colorData.length - 1];

    renderCanvas(edgeData, "canvas1");
    renderCanvas(colorData, "canvas2");
}

const WIDTH = 634;
const HEIGHT = 634;
const PIECE_SIZE = 2;

function renderCanvas(bytes, canvasId) {
    new p5((s) => {
        let batch = [];
        let count = batch.length;

        s.setup = () => {
            s.createCanvas(WIDTH, HEIGHT);

            let b = 0;
            let c = setInterval(() => {
                // stop once we've read all of the bytes
                if (b >= bytes.length) {
                    puzzleFinishedHanlder();
                    clearInterval(c);
                    return;
                }

                if (count !== 0) return;

                batch = [];

                if (settings.paused) return;

                for (let x = 0; x < settings.getBatchSize(); x++) {
                    // top left
                    batch.push({
                        y: bytes[b] * PIECE_SIZE,
                        x: bytes[b + 1] * PIECE_SIZE,
                        color: {
                            red: bytes[b + 2],
                            green: bytes[b + 3],
                            blue: bytes[b + 4],
                        },
                    });

                    // top right
                    batch.push({
                        y: bytes[b] * PIECE_SIZE,
                        x: bytes[b + 1] * PIECE_SIZE + 1,
                        color: {
                            red: bytes[b + 5],
                            green: bytes[b + 6],
                            blue: bytes[b + 7],
                        },
                    });

                    // bottom right
                    batch.push({
                        y: bytes[b] * PIECE_SIZE + 1,
                        x: bytes[b + 1] * PIECE_SIZE + 1,
                        color: {
                            red: bytes[b + 8],
                            green: bytes[b + 9],
                            blue: bytes[b + 10],
                        },
                    });

                    // bottom left
                    batch.push({
                        y: bytes[b] * PIECE_SIZE + 1,
                        x: bytes[b + 1] * PIECE_SIZE,
                        color: {
                            red: bytes[b + 11],
                            green: bytes[b + 12],
                            blue: bytes[b + 13],
                        },
                    });

                    b += 14;
                }

                count = batch.length;
                // 1s/60calls = 0.0167 seconds per call
            }, 1 / 60);

            settings.listenToPauseState((paused) => {
                if (paused) s.noLoop();
                else s.loop();
            });

            settings.listenToReset(() => {
                s.remove();
                console.log("hi");
                clearInterval(c);
                settings.resetListeners = [];
            });

            s.background(0);
        };

        s.draw = () => {
            for (let item of batch) {
                s.noStroke();
                s.fill(item.color.red, item.color.green, item.color.blue);
                s.rect(item.x, item.y, 1, 1);
                count = Math.max(count - 1, 0);
            }
        };
    }, canvasId);
}

function puzzleFinishedHanlder() {
    const durationContainer = document.getElementById("duration");
    const edgeDuration = document.getElementById("edge-duration-txt");
    const colorDuration = document.getElementById("color-duration-txt");

    durationContainer.style.display = "flex";
    edgeDuration.innerText = `${times.edge / 1000} seconds`;
    colorDuration.innerText = `${times.color / 1000} seconds`;

    settings.started = false;
}
