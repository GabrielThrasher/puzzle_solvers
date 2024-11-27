async function init() {
    await startPuzzle();
}

async function startPuzzle() {
    try {
        const [response1, response2] = await Promise.all([
            fetch("http://127.0.0.1:8000/puzzle-edge"),
            fetch("http://127.0.0.1:8000/puzzle-color"),
        ]);

        if (!response1.ok || !response2.ok) {
            throw new Error("request failed");
        }

        const data = await response1.arrayBuffer();
        const data3 = await response2.arrayBuffer();
        const bytes = new Uint8Array(data);
        const bytes2 = new Uint8Array(data3);
        renderCanvas(bytes);
        renderCanvas2(bytes2);
    } catch (error) {
        console.error(error);
    }
}

init();

/**
 * TODO:
 * - fetch binary file that contain all of the steps for each algorithms
 * - for everyline in that file, draw the puzzle piece in its corresponding position
 * - scale down the puzzle?
 * - pause/play and controlling speed
 */
function renderCanvas(bytes) {
    let canvas1 = new p5((s) => {
        let items = [];
        let size = 2;
        let b = 0;

        s.setup = () => {
            s.createCanvas(600, 600);
            let c = setInterval(() => {
                if (b >= bytes.length) {
                    clearInterval(c);
                    return;
                }
                items.push({
                    x: bytes[b] * size,
                    y: bytes[b + 1] * size,
                    size,
                    isDrawn: false,
                });
                b += 2;
            }, 5);
            s.background(0);
            s.fill(255);
        };

        s.draw = () => {
            // s.background(0);
            // s.fill(255);
            for (let item of items) {
                if (item.isDrawn) continue;
                s.fill(255);
                s.rect(item.x, item.y, item.size, item.size);
                item.isDrawn = true;
            }
        };
    }, "canvas1");
}

function renderCanvas2(bytes) {
    new p5((s) => {
        let items = [];
        let size = 2;
        let b = 0;

        s.setup = () => {
            s.createCanvas(600, 600);
            let c = setInterval(() => {
                if (b >= bytes.length) {
                    clearInterval(c);
                    return;
                }
                items.push({
                    x: bytes[b] * size,
                    y: bytes[b + 1] * size,
                    size,
                    isDrawn: false,
                });
                b += 2;
            }, 5);
            s.background(0);
            s.fill(255);
        };

        s.draw = () => {
            for (let item of items) {
                if (item.isDrawn) continue;
                s.fill(255);
                s.rect(item.x, item.y, item.size, item.size);
                item.isDrawn = true;
            }
        };
    }, "canvas2");
}