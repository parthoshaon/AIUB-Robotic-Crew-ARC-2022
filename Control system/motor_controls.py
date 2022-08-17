import connection

def forward():
    conn = connection.socket_conn()
    conn.send('w'.encode('utf-8'))
    print("forward")
    conn.close()

def backward():
    conn = connection.socket_conn()
    conn.send('s'.encode('utf-8'))
    print("backward")
    conn.close()

def left_rotation():
    conn = connection.socket_conn()
    conn.send('a'.encode('utf-8'))
    print("left")
    conn.close()

def right_rotation():
    conn = connection.socket_conn()
    conn.send('d'.encode('utf-8'))
    print("right")
    conn.close()

def halt():
    conn = connection.socket_conn()
    conn.send('b'.encode('utf-8'))
    print("halt")
    conn.close()


def halt_motorfunctions():
    conn = connection.socket_conn()
    conn.send('b'.encode('utf-8'))
    conn.send('v'.encode('utf-8'))
    print("halt")
    conn.close()

def rover_speed(x):
    conn = connection.socket_conn()
    conn.send(x.encode('utf-8'))
    print("speed : "+x)
    conn.close()

def arm_instructions(x,y):
    conn = connection.socket_conn()
    conn.send(x.encode('utf-8'))
    print(x+" : "+y)
    conn.close()
