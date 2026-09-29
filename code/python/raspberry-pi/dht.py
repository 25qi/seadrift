import sys
import Adafruit_DHT
#import time
dht11 = 11
DHT_PIN = 4
while True:
    h, t = Adafruit_DHT.read_retry(dht11, DHT_PIN)
    print(h, t)
    # time.sleep(1)
