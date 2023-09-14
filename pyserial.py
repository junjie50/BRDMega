import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)
time.sleep(2)

if ser.isOpen():
    print("Port Open")

cmd = input("Serial cmd: ")
while cmd != 'quit':
	ser.write(b't')
	time.sleep(0.05)
	cmd = input("Serial cmd: ")
