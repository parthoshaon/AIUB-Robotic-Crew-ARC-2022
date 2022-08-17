import bluetooth
from pynput import keyboard
import rover_nav
pressed = {}
print(bluetooth.discover_devices(duration=8, lookup_names=True, flush_cache=True, lookup_class=False))
print ("Searching for devices...")
print ("")
#Create an array with all the MAC
#addresses of the detected devices
nearby_devices = bluetooth.discover_devices()
print(nearby_devices)
#Run through all the devices found and list their name
num = 0
print ("Select your device by entering its coresponding number...")
for i in nearby_devices:
	num+=1
	print(num , ": " , bluetooth.lookup_name( i ))

#Allow the user to select their Arduino
#bluetooth module. They must have paired
#it before hand.

selection = input("> ") - 1

print ("You have selected", bluetooth.lookup_name(nearby_devices[selection]))
bd_addr = nearby_devices[selection]
sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
port = 1


def on_press(key): 
    if key not in pressed: # Key was never pressed before
        pressed[key] = False
    
    if not pressed[key]: # Same logic
        pressed[key] = True
        print('Key %s pressed' % key) 
        key = format(key.char)
        if(key == 'w'):
            sock.send(key)
        elif(key == "s"):
            sock.send(key)
        elif(key == "a"):
            sock.send(key)
        elif(key == "d"):
            sock.send(key)
        elif(key == "h"):
            sock.send(key)
        elif(key == "1"):
            sock.send(key)
        elif(key == "2"):
            sock.send(key)
        elif(key == "3"):
            sock.send(key)
        

def on_release(key):  # Same logic
    pressed[key] = False
    print('Key %s released' %key) 
    key = format(key.char)
    if(key == 'w'):
        sock.send("b")
    elif(key == "s"):
        sock.send("b")
    elif(key == "a"):
        sock.send("b")
    elif(key == "d"):
        sock.send("b")

with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()

