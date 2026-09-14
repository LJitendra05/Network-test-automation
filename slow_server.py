import socket
import time
HOST = "127.0.0.1"
PORT = 8080
server = socket.socket(
    socket.AF_INET,
    socket.SOCK_STREAM
)
server.bind((HOST, PORT))
server.listen(1)
print(f"Slow server listening on {HOST}:{PORT}")
client, address = server.accept()
print(f"Client connected: {address}")
data = client.recv(4096)
print(f"Received {len(data)} bytes")
print("Waiting without responding...")
time.sleep(10)
client.close()
server.close()