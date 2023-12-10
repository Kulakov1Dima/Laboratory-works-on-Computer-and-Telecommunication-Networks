from fastapi import FastAPI, WebSocket
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
import uvicorn as uvicorn 
from auth import send_email

app = FastAPI()
app.mount("/html", StaticFiles(directory="html files"), name="html")

emailform = "yandex.ru"

@app.get("/")
async def read_root():
    return FileResponse("html files/index.html")

@app.get("/error")
async def read_root():
    return "Проверьте правильность данных"

@app.get("/mail")
async def read_root():
    return "Письмо отправлено"

@app.get("/smtp")
async def read_root():
    return FileResponse("html files/auth.html")

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        if data.startswith("connectSmtp"):
            _, email, password, senderEmail, subject, message, file_path, port = data.split("+")
            emailform =  email.split("@")[1]
            await websocket.send_text(emailform)
            listto = senderEmail.split(",")
            for to in listto:
                await websocket.send_text(send_email(f'smtp.{emailform}', port, email, password, to, subject, message, file_path))
            break

if __name__ == "__main__":
    uvicorn.run("main:app", host="127.0.0.1", port=80, reload=True)