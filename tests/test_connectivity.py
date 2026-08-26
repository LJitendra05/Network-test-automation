import pytest
from utils.network_utils import ping_host
from utils.config_loader import load_devices

devices=load_devices()

@pytest.mark.parametrize(
        "device",
        devices,
        ids=[device["Name"] for device in devices])
def test_host_connectivity(device):
    result=ping_host(device["host"])
    assert result is device["expected_reachable"]