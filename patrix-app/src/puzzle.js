import { settings } from "./settings";

export async function startPuzzle() {
    try {
        const [edgeResponse, colorResponse] = await Promise.all([
            fetch("http://127.0.0.1:8000/puzzle-edge"),
            fetch("http://127.0.0.1:8000/puzzle-color"),
        ]);

        if (!edgeResponse.ok || !colorResponse.ok) {
            throw new Error("request failed");
        }

        const edgeData = await edgeResponse.arrayBuffer();
        const colorData = await colorResponse.arrayBuffer();

        renderPuzzle(edgeData, colorData);
    } catch (error) {
        console.error(error);
    }
}

export function renderPuzzle(edgeData, colorData) {
    // TODO: it won't be 2 bytes each
    const edgeDataBytes = new Uint16Array(edgeData);
    const colorDataBytes = new Uint16Array(colorData);

    renderCanvas(edgeDataBytes, "canvas1");
    renderCanvas(colorDataBytes, "canvas2");
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

            settings.listenToPauseState((paused) => {
                if (paused) s.noLoop();
                else s.loop();
            });

            let b = 0;
            let c = setInterval(() => {
                // stop once we've read all of the bytes
                if (b >= bytes.length) {
                    clearInterval(c);
                    return;
                }

                if (count !== 0) return;

                batch = [];

                if (settings.paused) return;

                for (let x = 0; x < settings.getBatchSize(); x++) {
                    batch.push({
                        x: bytes[b] * PIECE_SIZE,
                        y: bytes[b + 1] * PIECE_SIZE,
                    });
                    b += 2;
                }

                count = batch.length;
                // 1s/60calls = 0.0167 seconds per call
            }, 16.6667);

            s.background(0);
        };

        s.draw = () => {
            // FIXME: why does it look so weird on my monitor's screen?
            for (let item of batch) {
                // if (item.isDrawn) continue;
                s.fill(255);
                s.rect(item.x, item.y, PIECE_SIZE, PIECE_SIZE);
                count = Math.max(count - 1, 0);
            }
        };
    }, canvasId);
}
