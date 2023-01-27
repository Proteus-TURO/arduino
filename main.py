import serial
import time

ser = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)

while(True):
    ser.write('on'.encode('utf-8'))
    time.sleep(1)
    ser.write('off'.encode('utf-8'))
    time.sleep(1)

