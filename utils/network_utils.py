import subprocess
import logging
import platform
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
    logger.info(f"Starting performance test for {host}") 

    if platform.system() == "Windows":
        command = ["ping", "-n", str(count), host]
    else:
        command = ["ping", "-c", str(count), host]
    result=subprocess.run(
        command,
        capture_output=True,
        text=True
    )
    output=result.stdout
    lines = output.splitlines()
    packet_line = next(
        line for line in lines if "Packets:" in line)
    timing_line = next(
        line for line in lines if "Minimum =" in line)

    packet_parts = packet_line.split(",")
    sent = int(packet_parts[0].split("=")[1].strip())
    received = int(packet_parts[1].split("=")[1].strip())
    lost = int(packet_parts[2].split("=")[1].split("(")[0].strip())
    timing_parts = timing_line.split(",")
    minimum = int(timing_parts[0].split("=")[1].replace("ms", "").strip())
    maximum = int(timing_parts[1].split("=")[1].replace("ms", "").strip())
    average = int(timing_parts[2].split("=")[1].replace("ms", "").strip())
    packet_loss = (lost / sent) * 100

    logger.info(
        f"{host}: sent={sent}, received={received}, "
        f"lost={lost}, packet_loss={packet_loss}%"
    )
    logger.info(
        f"{host}: min={minimum}ms, max={maximum}ms, "
        f"average={average}ms"
    )
    
    return {
        "host": host,
        "sent": sent,
        "received": received,
        "lost": lost,
        "packet_loss": packet_loss,
        "minimum": minimum,
        "maximum": maximum,
        "average": average
    }