import socket
host = '192.168.1.23'
port = 5544

def socket_conn():
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sk.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sk.connect((host,port))
    return sk
