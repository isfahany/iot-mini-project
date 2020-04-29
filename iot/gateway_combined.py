from gattlib import GATTRequester
import paho.mqtt.client as mqtt
from time import sleep

try:
    req = GATTRequester("3C:71:BF:FD:08:96")
    
    client = mqtt.Client()
    client.connect("mqtt.thingspeak.com",1883,60)

    channelId = "1046955"
    apiKey = "R2GFC1Q7A5Y999I0" #Write
    #apiKey = "3CIKRBES3MDG9W7J" #Read

    while True:
        data = req.read_by_uuid("00002A37-0000-1000-8000-00805F9B34FB")[0]
        #data = req.read_by_handle(0x180F)[0]
        
        value_byte = int(data[1])
        
        print("bytes received:", end=' ')
        print(value_byte, end=' ')
        if(value_byte < 100):
            send_to_thingspeak = 0
            print("0. Darah rendah")
        elif(value_byte < 120):
            send_to_thingspeak = 1
            print("1. Normal")
        else:
            send_to_thingspeak = 2
            print("2. Darah tinggi")
        client.publish("channels/%s/publish/%s" % (channelId, apiKey),
               "field1="+ str(send_to_thingspeak))
        client.loop(2)
        sleep(20)


except KeyboardInterrupt:
    exit(1)
