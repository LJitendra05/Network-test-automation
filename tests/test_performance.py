import pytest
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