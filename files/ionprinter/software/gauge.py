import serial
import sys
gauge = serial.Serial(sys.argv[1],9600)



while(True):
    input_string = gauge.readline()
    print(input_string)
    print(input_string.encode(my_str))
