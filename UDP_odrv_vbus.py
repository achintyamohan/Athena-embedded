import socket
import struct

UDP_IP = "169.254.187.33"
CLIENT_IP = "169.254.174.143"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(60)

    if (struct.unpack("<i", data)[0] == 420):
        #print("got heartbeat:", int.from_bytes(data, byteorder='little', signed=True))
        break
    else:
        continue
        #print(struct.unpack("<i", data)[0])

sock.sendto(struct.pack("<i", 10), (CLIENT_IP, UDP_PORT))

while True:
    data2, addr = sock.recvfrom(60) # buffer size is 1024 bytes

    data2_upk = struct.unpack("<f", data2)[0]
    if (data2_upk > 10.0):
        print("got vbus voltage", data2_upk)
        break