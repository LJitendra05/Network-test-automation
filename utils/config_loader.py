import json

def load_devices():
    with open("config/devices.json","r") as f:
        data=json.load(f)

        return data["Devices"]  