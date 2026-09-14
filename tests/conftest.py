import subprocess
import time
import pytest


@pytest.fixture
def echo_server():
    server = subprocess.Popen(
        ["python", "-u", "echo_server.py"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    deadline = time.time() + 2

    while time.time() < deadline:
        if server.poll() is not None:
            stderr = server.stderr.read()
            pytest.fail(
                f"Echo server exited during startup:\n{stderr}"
            )

        line = server.stdout.readline()

        if "Echo server listening on 127.0.0.1:8080" in line:
            break

    else:
        server.terminate()
        server.wait()
        pytest.fail("Echo server did not become ready")

    yield server

    server.terminate()
    server.wait()