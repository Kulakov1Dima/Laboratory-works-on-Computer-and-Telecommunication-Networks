from fastapi import FastAPI, WebSocket
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
import uvicorn as uvicorn
from lab3ftp import ftp_check

app = FastAPI()

app.mount("/html", StaticFiles(directory="html files"), name="html")

async def send_message(websocket: WebSocket, message: str):
    await websocket.send_text(message)

@app.get("/")
async def read_root():
    return FileResponse("html files/index.html")

@app.get("/success")
async def read_root():
    return FileResponse("html files/success.html")

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        if data.startswith("connectFTP"):
            _, ftp_address, login, password = data.split(":")
            print(f"Received FTP Address: {ftp_address}, Login: {login}, Password: {password}")
            
            if ftp_check(ftp_address):
                await send_message(websocket, f"success?login={login}&password={password}")
            else:
                await send_message(websocket, "error")

if __name__ == "__main__":
   uvicorn.run("main:app", host="127.0.0.1", port=80, reload=True)