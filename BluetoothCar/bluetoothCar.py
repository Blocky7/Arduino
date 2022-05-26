import serial
import time
import tkinter as tk
from functools import partial

# Define the serial port and baud rate.
# Ensure the 'COM#' corresponds to what was seen in the Windows Device Manager
#/dev/cu.HC-06-SPPDev
ser = serial.Serial('/dev/cu.HC-06-SPPDev', 9600)
time.sleep(2) # wait for the serial connection to initialize
window = tk.Tk()
window.title("just work already")
window.geometry("200x200")

def go(direction):
    print(direction)
    if direction == "up":
        time.sleep(0.1) 
        ser.write(b'U') 
    elif direction == "down":
        time.sleep(0.1)
        ser.write(b'D')
    elif direction == "right":
        time.sleep(0.1)
        ser.write(b'R')
    elif direction == "left":
        time.sleep(0.1)
        ser.write(b'L')
    elif direction == "stop":
        time.sleep(0.1)
        ser.write(b'S')
    elif direction == "fire":
        time.sleep(0.1)
        ser.write(b'F')

btnUp = tk.Button(window, text="FORWARD")
btnUp.configure(height=4,width=8)
btnUp.place_configure(relx=0.5,rely=0.1,anchor="center")
btnUp.bind('<ButtonPress-1>',lambda event, direction="up": go(direction))
btnUp.bind('<ButtonRelease-1>',lambda event, direction="stop": go(direction))

btnDown = tk.Button(window, text="BACKWARD")
btnDown.configure(height=4,width=8)
btnDown.place_configure(relx=0.5,rely=0.9,anchor="center")
btnDown.bind('<ButtonPress-1>',lambda event, direction="down": go(direction))
btnDown.bind('<ButtonRelease-1>',lambda event, direction="stop": go(direction))

btnRight = tk.Button(window, text="RIGHT")
btnRight.configure(height=4,width=8)
btnRight.place_configure(relx=0.9,rely=0.5,anchor="center")
btnRight.bind('<ButtonPress-1>',lambda event, direction="right": go(direction))
btnRight.bind('<ButtonRelease-1>',lambda event, direction="stop": go(direction))

btnLeft = tk.Button(window, text="LEFT")
btnLeft.configure(height=4,width=8)
btnLeft.place_configure(relx=0.1,rely=0.5,anchor="center")
btnLeft.bind('<ButtonPress-1>',lambda event, direction="left": go(direction))
btnLeft.bind('<ButtonRelease-1>',lambda event, direction="stop": go(direction))

window.mainloop()
