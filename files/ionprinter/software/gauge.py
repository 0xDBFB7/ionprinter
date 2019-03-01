#python3 script to decode the output from an Inficon BPG400 high vacuum gauge
#or, as I prefer to call it, the BFG
#yittrium oxide filament!

# 0 Length of data string 7 (Set value)
# 1 Page number 5 (For BPG400)
# 2 Status - Status byte
# 3 Error → Error byte
# 4 Measurement high byte 0 … 255 → Calculation of pressure value
# 5 Measurement low byte 0 … 255 → Calculation of pressure value
# 6 Software version 0 … 255 → Software version
# 7 Sensor type 10 (For BPG400)
# 8 Check sum 0 … 255 → Synchronization

import serial
import sys
import time
gauge = serial.Serial(sys.argv[1],9600,timeout=1000)

while(True):
    while(True):
        input_string = list(gauge.read(2)) #watch for two magic bytes
        if(input_string[0] == 7 and input_string[1] == 5):
            input_string += list(gauge.read(9-2)) #append the rest of the data
            if((sum(input_string[1:8]) % 256) == input_string[8]): #compute checksum
                break

    gauge_value = 10.0**((input_string[4] * 256.0 + input_string[5]) / 4000.0 - 12.5)
    if(len(sys.argv) > 2):
        print("mBar: {} Pa: {} Status: {} ({}) Error: {} ({})".format(gauge_value,gauge_value*100000.0,
                                                        input_string[2],bin(input_string[2]),
                                                        input_string[3],bin(input_string[3])))
    else:
        print("{},{}".format(time.time(), gauge_value))
    # print(int.from_bytes(input_string, byteorder='little'))
