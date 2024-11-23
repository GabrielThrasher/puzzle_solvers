import "./App.css";
import { useState } from "react";

function PuzzleCanvas() {
    return (
        <div className="puzzle-canvas">
            <p>Test</p>
        </div>
    );
}

function App() {
    const [isPaused, setIsPaused] = useState(true);
    return (
        <div>
            <h1 id="title">Patrix 🧩</h1>
            <p>
                An edge algorithm or a color algorithm -- which one is faster at
                solving the same 100,000-piece puzzle?
            </p>
            <button id="start-btn">start 🚀</button>
            <div id="visualization-area">
                <PuzzleCanvas />
                <PuzzleCanvas />
            </div>
            <div id="controls-area">
                <div className="control-btn">⏪</div>
                <div
                    id="pause-btn"
                    className="control-btn"
                    onClick={() => setIsPaused((prev) => !prev)}
                >
                    {isPaused ? "▶️" : "️⏸️"}
                </div>
                <div className="control-btn">️⏩</div>
            </div>
        </div>
    );
}

export default App;
