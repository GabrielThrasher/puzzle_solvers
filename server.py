from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse


app = FastAPI()
app.add_middleware(CORSMiddleware, allow_origins="*")


@app.get("/puzzle-color")
def getcolor():
    file_path = "example.bin"
    return FileResponse(file_path)


@app.get("/puzzle-edge")
def getedge():
    file_path = "example2.bin"
    return FileResponse(file_path)
