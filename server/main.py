from fastapi import FastAPI
from datetime import datetime
import hex
import json

app = FastAPI()


@app.get("/wifi/data/get")
async def get_wifi_data(mid: str):
    with open(f"{mid}.json", "r", encoding="UTF-8") as f:
        content = f.read()
    data = json.loads(content)
    return data


@app.get("/wifi/data/send")
async def send_wifi_data(ssid: str, password: str, mid: str):
    ssid_decode = hex.hex_to_text(ssid)
    password_decode = hex.hex_to_text(password)
    mid_decode = hex.hex_to_text(mid)
    with open(f"{mid}.json", "w", encoding="UTF-8") as f:
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
        json_str = json.dumps(data, indent=4)
        f.write(json_str)
    return {
        "status": "success",
        "code": 0,
        "message": "Data sent successfully",
        "ssid": ssid,
        "password": password,
        "mid": mid
    }


@app.get("/wifi/data/reset")
async def reset_wifi_data(mid: str):
    mid_decode = hex.hex_to_text(mid)
    with open(f"{mid}.json", "w", encoding="UTF-8") as f:
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
    return {"status": "success", "message": "Data reset successfully"}
