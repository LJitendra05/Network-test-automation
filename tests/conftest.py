import subprocess
import time
import pytest


@pytest.fixture
def echo_server():
    server = subprocess.Popen(
        ["python", "echo_server.py"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    time.sleep(1)

    yield server

    server.terminate()
    server.wait()