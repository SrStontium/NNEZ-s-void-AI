# 获取WIFI的服务器API

## 安装
```
pip install -r requirements.txt
pip install uvicorn
```
## 运行
```
uvicorn main:app --reload
```

## 接口

| 接口     | 用途     | 方式     | 参数     |
| -------- | -------- | -------- |-------- |
| /wifi/data/get | 获取wifi数据 | GET | str:mid mid：为设备id的十六进制 |
| /wifi/data/send | 提供wifi数据 | GET | str:mid,password,ssid ssid：WIFI名称的十六进制，password：密码的十六进制，mid：设备ID的十六进制 |
| /wifi/data/reset | 初始化wifi数据 | GET | str:mid mid：设备id的十六进制 |

## WebUI

```py
http://<example.com>/wifi/send/ui #发送数据的UI
```