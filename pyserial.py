import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

if ser.isOpen():
    print("Port Open")

cmd = input("Serial cmd: ")
while cmd != 'quit':
	print(ser.write(bytes(int(cmd))))
	time.sleep(0.05)
	cmd = input("Serial cmd: ")
