import socket
import struct

HOST = "127.0.0.1"
PORT = 8080

server = socket.socket(
    socket.AF_INET,
    socket.SOCK_STREAM
)

server.bind((HOST, PORT))
server.listen(1)

print(f"Echo server listening on {HOST}:{PORT}")

while True:

    client, address = server.accept()

    print(f"Client connected: {address}")

    try:

        while True:

            header = client.recv(4)

            if not header:
                break

            message_size = struct.unpack(
                "!I",
                header
            )[0]

            data = b""

            while len(data) < message_size:

                chunk = client.recv(
                    message_size - len(data)
                )

                if not chunk:
                    break

                data += chunk

            if len(data) != message_size:
                print("Incomplete message")
                break

            print(
                f"Received: {data.decode(errors='replace')}"
            )

            client.sendall(header)
            client.sendall(data)

    finally:

        client.close()