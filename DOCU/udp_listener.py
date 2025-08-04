import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 3333))  # Puerto 5000, igual que en el ESP32

print("Esperando datos del ESP32...")
while True:
    data, addr = sock.recvfrom(1024)
    print(f"[ESP32] {data.decode()}")
