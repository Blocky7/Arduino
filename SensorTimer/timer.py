import time
import serial
from pynput.keyboard import Key, Controller

ser = serial.Serial('COM7', 9600) #1410 #1420
time.sleep(2) # wait for the serial connection to initialize
keyboard = Controller()
prev = ""
while True:
    bytesToRead = ser.inWaiting()
    message = ser.readline(bytesToRead).decode()
    if(len(message) > 0 and message[0] != " "):
        if message[0].isalpha():
            count = 0
            s = ""
            for a in prev:
                if count == 3:
                    break
                elif count >= 1:
                    count += 1
                if a == '.':
                    count = 1
                    continue
                keyboard.press(a)
                keyboard.release(a)
                time.sleep(0.050)
            keyboard.press(Key.enter)
            keyboard.release(Key.enter)
        prev = message

    
    