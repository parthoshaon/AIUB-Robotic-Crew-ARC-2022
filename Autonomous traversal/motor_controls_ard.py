import connection
ard = connection.arduino_conn()

def forward():
    ard.write('w'.encode('utf-8'))

def backward():
    ard.write('s'.encode('utf-8'))

def left_rotation():
    ard.write('a'.encode('utf-8'))

def right_rotation():
    ard.write('d'.encode('utf-8'))

def halt():
    ard.write('h'.encode('utf-8'))

def halt_motorfunctions():
    ard.write('v'.encode('utf-8'))
    ard.write('b'.encode('utf-8'))

def speed(x):
    ard.write(x.encode('utf-8'))

def arm_instructions(x,y):
    ard.write(x.encode('utf-8'))
    print(x+" : "+y)
