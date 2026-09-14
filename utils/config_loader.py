import json


def load_devices():
    with open("config/devices.json", "r") as f:
        data = json.load(f)

    return data["Devices"]


def load_tcp_config():
    with open("config/tcp_config.json", "r") as f:
        return json.load(f)