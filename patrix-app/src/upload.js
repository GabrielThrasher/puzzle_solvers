export async function uploadFile(file, cb) {
    if (!file.data) return;
    const formData = new FormData();
    formData.append("file", file.data);

    console.log(file.data);

    document.getElementById("loading").style.display = "block";

    // upload file to server
    try {
        const response = await fetch("http://127.0.0.1:8000/upload", {
            method: "POST",
            body: formData,
        });

        if (!response.ok) {
            throw new Error("File upload failed");
        }

        let data = await response.json();

        if (data.success) {
            try {
                const [edgeResponse, colorResponse] = await Promise.all([
                    fetch(
                        `http://127.0.0.1:8000/puzzle?fileName=${file.data.name}&puzzleType=edge`
                    ),
                    fetch(
                        `http://127.0.0.1:8000/puzzle?fileName=${file.data.name}&puzzleType=color`
                    ),
                ]);

                if (!edgeResponse.ok || !colorResponse.ok) {
                    throw new Error("request failed");
                }

                const edgeData = await edgeResponse.arrayBuffer();
                const colorData = await colorResponse.arrayBuffer();

                document.getElementById("loading").style.display = "none";
                cb(new Uint16Array(edgeData), new Uint16Array(colorData));
            } catch (error) {
                console.error(error);
            }
        } else {
            throw new Error("cant upload file");
        }
    } catch (error) {
        console.error("Error uploading file:", error);
    }
}
