from fastapi import FastAPI, UploadFile
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
import subprocess
import os


app = FastAPI()
app.add_middleware(CORSMiddleware, allow_origins="*")


@app.post("/upload")
async def upload(file: UploadFile):
    with open(f"patrix/images/{file.filename}", "wb") as file_object:
        file_object.write(await file.read())

    result = subprocess.run(
        ["./patrix/build/Patrix", file.filename],  # type: ignore
        capture_output=True,
        text=True,
    )

    # Check the return code

    if result.returncode == 0:
        print("C++ program executed successfully")
        print("Output:", result.stdout)
    else:
        print("C++ program failed with return code:", result.returncode)
        print("Error:", result.stderr)

    return {"success": True}


@app.get("/puzzle")
def getpuzzle(fileName: str = "default-colorAlgoSteps.bin", puzzleType: str = "color"):
    return FileResponse(f"{fileName}-{puzzleType}AlgoSteps.bin")


@app.get("/default-puzzle-color")
def getcolor():
    file_path = "default-colorAlgoSteps.bin"
    # file_path = "person.jpg-colorAlgoSteps.bin"
    return FileResponse(file_path)


@app.get("/default-puzzle-edge")
def getedge():
    file_path = "default-edgeAlgoSteps.bin"
    # file_path = "person.jpg-edgeAlgoSteps.bin"
    return FileResponse(file_path)
