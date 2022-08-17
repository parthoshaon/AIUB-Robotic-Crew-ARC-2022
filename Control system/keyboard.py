from pynput import keyboard
import motor_controls
pressed = {}

def on_press(key): 
    if key not in pressed: # Key was never pressed before
        pressed[key] = False
    
    if not pressed[key]: # Same logic
        pressed[key] = True
        if(hasattr(key,'char')):
             key = format(key.char)
        print('Key %s pressed' % key) 
        if(key == 'w'):
            motor_controls.forward()
        elif(key == "s"):
            motor_controls.backward()
        elif(key == "a"):
            motor_controls.left_rotation()
        elif(key == "d"):
            motor_controls.right_rotation()
        elif(key == "b"):
            motor_controls.halt()
        elif(key == "1"):
            motor_controls.rover_speed(key)
        elif(key == "2"):
            motor_controls.rover_speed(key)
        elif(key == "3"):
            motor_controls.rover_speed(key)
        elif(key == "r"):
            motor_controls.arm_instructions(key,"base_left")
        elif(key == "f"):
            motor_controls.arm_instructions(key,"base_right")
        elif(key == "t"):
            motor_controls.arm_instructions(key,"first_actuator_up")
        elif(key == "g"):
            motor_controls.arm_instructions(key,"first_actuator_down")
        elif(key == "y"):
            motor_controls.arm_instructions(key,"second_actuator_up")
        elif(key == "h"):
            motor_controls.arm_instructions(key,"second_actuator_down")
        elif(key == "u"):
            motor_controls.arm_instructions(key,"third_actuator_up")
        elif(key == "j"):
            motor_controls.arm_instructions(key,"third_actuator_down")
        elif(key == "i"):
            motor_controls.arm_instructions(key,"claw_close")
        elif(key == "k"):
            motor_controls.arm_instructions(key,"claw_open")
        elif(key == "o"):
            motor_controls.arm_instructions(key,"claw_right_rotate")
        elif(key == "l"):
            motor_controls.arm_instructions(key,"claw_left_rotate")
        elif(key == "v"):
            motor_controls.arm_instructions(key,"arm halted")
def on_release(key):  # Same logic
    pressed[key] = False
    print('Key %s released' %key) 
    if(hasattr(key,'char')):
             key = format(key.char)
    if(key == 'w'):
        motor_controls.halt()
    elif(key == "s"):
        motor_controls.halt()
    elif(key == "a"):
        motor_controls.halt()
    elif(key == "d"):
        motor_controls.halt()
    elif(key == "r"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "f"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "t"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "g"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "y"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "h"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "u"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "j"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "i"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "k"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "o"):
        motor_controls.arm_instructions('v',"arm halted")
    elif(key == "l"):
        motor_controls.arm_instructions('v',"arm halted")

with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
