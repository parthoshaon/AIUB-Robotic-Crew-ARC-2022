import socket
import serial
from netifaces import interfaces, ifaddresses, AF_INET

hostname = socket.getfqdn()
port = 5544
print('connecting')
print('conncected to ',hostname)
for ifaceName in interfaces():
    addresses = [i['addr'] for i in ifaddresses(ifaceName).setdefault(AF_INET, [{'addr':'NILL'}] )]
    print(ifaceName, ', '.join(addresses))

ipv4 = addresses[0]

def socket_conn():
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sk.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sk.bind((ipv4,port))
    return sk

def arduino_conn():
    ard_conn = serial.Serial("/dev/ttyACM0", 9600)
    return ard_conn
