import pygame
import motor_controls
pygame.init()
j = pygame.joystick.Joystick(0)
j.init()
try:
    while True:
        events = pygame.event.get()
        for event in events:
            if event.type == pygame.JOYBUTTONDOWN: #if pressed
                val_pressed = event.button
                if val_pressed == 2:
                    motor_controls.forward()
                elif val_pressed == 0:
                    motor_controls.backward()
                elif val_pressed == 3:
                    motor_controls.left_rotation()
                elif val_pressed == 1:
                    motor_controls.right_rotation()
            elif event.type == pygame.JOYBUTTONUP: #if released
                motor_controls.halt()
except KeyboardInterrupt:
    print("Controller Disconnected")
    print("EXITING NOW")
    j.quit()