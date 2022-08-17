import motor_controls

def main():
    print("....")
    print("system started")
    print("establishing connection with rover")
    motor_controls.halt_motorfunctions()
    print("established connection with rover")
    exec(open("keyboard.py").read())

if __name__ == "__main__":
    main()

