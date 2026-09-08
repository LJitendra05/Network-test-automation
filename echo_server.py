import socket

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind(("127.0.0.1", 8080))
server.listen(1)

print("Echo server listening on 127.0.0.1:8080")

client, address = server.accept()

print("Client connected:", address)

data = client.recv(1024)

print("Received:", data.decode())

client.sendall(data)

client.close()
server.close()