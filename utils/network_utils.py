import subprocess
import logging

# logging.basicConfig(
#     filename="network_test.log",
#     level=logging.INFO,
#     format="%(asctime)s -%(levelname)s - %(message)s"
# )

logger=logging.getLogger(__name__)
logger.setLevel(logging.INFO)

file_handler=logging.FileHandler("network_test.log")
formatting=logging.Formatter("%(asctime)s -%(levelname)s - %(message)s")
file_handler.setFormatter(formatting)
logger.addHandler(file_handler)

def ping_host( host):
    logger.info(f"Starting ping test for {host}")

    results=subprocess.run(
        ["ping","-n","1",host],
        capture_output=True,
        text=True)
    
    if results.returncode == 0:
        logger.info(f"{host} is reachable")
        return True
    logger.info(f"{host} is not reachable")
    return False

    def ping_performance(host,count=5):
        result=subprocess.run(
            ["ping","-n",str(count),host],
            capture_output=True,
            text=True
        )
        output=result.stdout

        return output