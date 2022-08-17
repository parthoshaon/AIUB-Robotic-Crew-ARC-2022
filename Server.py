import connection
def main():
    print("....")
    print("server started at",connection.ipv4)

    while True:
        try:
            c = connection.socket_conn()
            c.listen(1)
            conn, client_addr = c.accept()
            print("connected with: ",client_addr)
            signal = conn.recv(512).decode('utf-8')
            connection.serial0.write(signal.encode('utf-8'))
            print(signal)
            c.close()
        except:
            print("stoping all motor functions")
            ard = connection.arduino_conn()
            halt_drive_motors = 'b'
            ard.write(halt_drive_motors.encode('utf-8'))
            halt_arm_motors = 'v'
            ard.write(halt_arm_motors.encode('utf-8'))
            break

if __name__ == "__main__":
    main()

