import serial
ser = serial.Serial('/dev/ttyAMA0',9600,timeout=1)
ser.open()
ser.write("TTesting!")
try:
  while 1:
    response=ser.readline()
    print response
except KeyboardInterrupt:
  ser.close()
