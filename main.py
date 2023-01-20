# Importing libraries
import serial # For writing to Arduino serial
from pylsl import StreamInlet, resolve_stream 

ser = serial.Serial('/dev/tty.usbmodem2101', 115200)  # open serial port
print("Connected to ", ser.name)         # check which port was really used
ser.write(b'hello\r')     # write a string

# first resolve an EEG # stream on the lab network
print("looking for an EEG stream...")
streams = resolve_stream('type', 'EEG') 

# create a new inlet to read # from the stream
inlet = StreamInlet(streams[0])
while True:    
# get a new sample (you can also omit the timestamp part if you're 
# not interested in it)    
  sample, timestamp = inlet.pull_sample()    
  # print(timestamp, sample)
  tp9 = sample[0]
  af7 = sample[1]
  af8 = sample[2]
  tp10 = sample[3]
  right_aux = sample[4]
  
  ser.write((f"{tp9}, {af7}, {af8}, {tp10}").encode())

  # ser.write((f"{tp9}\n").encode())
  str = ser.readline()
  print(str)

ser.close()             # close port