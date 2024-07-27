from fastapi import FastAPI
from datetime import datetime
import hex
import json
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse



app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # 允许的源列表
    allow_credentials=True,  # 是否允许携带 cookies
    allow_methods=["*"],  # 允许的方法列表
    allow_headers=["*"],  # 允许的头部列表
)


@app.get("/wifi/data")
async def get_wifi_data(mid: str):
    with open(f"./data/{mid}.json", "r", encoding="UTF-8") as f:
        content = f.read()
    data = json.loads(content)
    return data


@app.get("/wifi/send")
async def send_wifi_data(ssid: str, password: str, mid: str):
    ssid_decode = hex.hex_to_text(ssid)
    password_decode = hex.hex_to_text(password)
    mid_decode = hex.hex_to_text(mid)
    with open(f"./data/{mid}.json", "w", encoding="UTF-8") as f:
        data = {
            "wifi": {
                "ssid": ssid_decode,
                "password": password_decode,
            },
            "info": {
                "status": "changed",
                "code": 114,
                "mid": mid_decode,
                "time": datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            }
        }
        json_str = json.dumps(data, indent=4,ensure_ascii=False)
        f.write(json_str)
    return {
        "status": "success",
        "code": 0,
        "message": "Data sent successfully",
        "ssid": ssid,
        "password": password,
        "mid": mid
    }

@app.get("/wifi/send/ui")
async def send_wifi_data_gui():
    return FileResponse("./webui/send_wifi_data.html")


@app.get("/wifi/reset")
async def reset_wifi_data(mid: str):
    mid_decode = hex.hex_to_text(mid)
    with open(f"./data/{mid}.json", "w", encoding="UTF-8") as f:
        data = {
            "wifi": {
                "ssid": "NNEZGBCZ",
                "password": "GenshinImpac",
            },
            "info": {
                "status": "initial",
                "code": 514,
                "mid": mid_decode,
                "time": datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            }
        }
        json_str = json.dumps(data, indent=4)
        f.write(json_str)
    return {"status": "success", "message": "Data reset successfully","code":0}
