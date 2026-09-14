import pytest
import subprocess
from utils.config_loader import load_devices
from utils.network_utils import ping_performance

devices = load_devices()
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
            ".\\network_engine.exe",
            "3",
            "100",
            "127.0.0.1",
            "8080",
        ],
        capture_output=True,
        text=True
    )

    print(result.stdout)

    assert result.returncode == 0
    assert "Result: PASS" in result.stdout
