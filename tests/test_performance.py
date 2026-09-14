import pytest
import json
import subprocess
from utils.config_loader import load_devices,load_tcp_config
from utils.network_utils import ping_performance

with open("config/tcp_config.json") as f:
    tcp_config = json.load(f)


devices = load_devices()
tcp_config = load_tcp_config()
performance_devices = [
    device for device in devices
    if device["expected_reachable"]
]
@pytest.mark.parametrize(
    "devices",
    performance_devices,
    ids=[device["Name"] for device in performance_devices]
)

def test_performance(devices):
    result=ping_performance(devices["host"],count=5)

    assert result['packet_loss'] <= devices["max_packet_loss"]
    assert result['average'] <= devices["max_latency"]

def test_cpp_tcp_engine():
    result = subprocess.run(
        [
            ".\\cpp\\bin\\network_engine.exe",
            str(tcp_config["test_count"]),
            str(tcp_config["payload_size"]),
            tcp_config["server_ip"],
            str(tcp_config["server_port"]),
        ],
        capture_output=True,
        text=True
    )

    print(result.stdout)

    assert result.returncode == 0

    output = result.stdout

    passed = None
    failed = None
    average_rtt = None

    for line in output.splitlines():

        if line.startswith("Passed:"):
            passed = int(line.split(":")[1].strip())

        elif line.startswith("Failed:"):
            failed = int(line.split(":")[1].strip())

        elif line.startswith("Average RTT:"):
            average_rtt = float(
                line.split(":")[1].split("us")[0].strip()
            )

    assert passed is not None
    assert failed is not None
    assert average_rtt is not None

    assert passed == tcp_config["test_count"]
    assert failed == 0

    assert average_rtt <= tcp_config["max_average_rtt_us"]