from fastapi import FastAPI, Request, WebSocket
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
import uvicorn as uvicorn
from lab3ftp import connect_ftp, ftp_check, list_files_ftp

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")
app.mount("/html", StaticFiles(directory="html files"), name="html")

async def send_message(websocket: WebSocket, message: str):
    await websocket.send_text(message)

@app.get("/")
async def read_root():
    return FileResponse("html files/index.html")

templates = Jinja2Templates(directory="templates")

@app.get("/success")
async def read_root(request: Request, ftp_address: str = None, login: str = None, password: str = None):
    files = []
    if connect_ftp(ftp_address, login, password):
        files = list_files_ftp(ftp_address, login, password)
        return templates.TemplateResponse("success.html", {"request": request, "files": files})
    else:
        return "Неверный логин или пароль"
    
@app.get("/error")
async def read_root(request: Request, ftp_address: str = None, login: str = None, password: str = None):
    return "Проверьте существование сервера"

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        if data.startswith("connectFTP"):
            _, ftp_address, login, password = data.split(":")
            print(f"Received FTP Address: {ftp_address}, Login: {login}, Password: {password}")
            
            if ftp_check(ftp_address):
                await send_message(websocket, f"success?ftp_address={ftp_address}&login={login}&password={password}")
            else:
                await send_message(websocket, "error")

if __name__ == "__main__":
   uvicorn.run("main:app", host="127.0.0.1", port=80, reload=True)