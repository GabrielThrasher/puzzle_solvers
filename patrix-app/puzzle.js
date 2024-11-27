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
        let items = [];

        s.setup = () => {
            s.createCanvas(WIDTH, HEIGHT);

            let b = 0;
            let c = setInterval(() => {
                // stop once we've read all of the bytes
                if (b >= bytes.length) {
                    clearInterval(c);
                    return;
                }

                // TODO: Change batchSize based on current speed
                const batchSize = 30;
                for (let x = 0; x < batchSize; x++) {
                    items.push({
                        x: bytes[b] * PIECE_SIZE,
                        y: bytes[b + 1] * PIECE_SIZE,
                        isDrawn: false,
                    });
                    b += 2;
                }
            }, 1);

            s.background(0);
            s.fill(255);
        };

        s.draw = () => {
            // TODO: is it possible to draw in batches instead of looping through all the items?
            for (let item of items) {
                if (item.isDrawn) continue;
                s.fill(255);
                s.rect(item.x, item.y, PIECE_SIZE, PIECE_SIZE);
                item.isDrawn = true;
            }
        };
    }, canvasId);
}
