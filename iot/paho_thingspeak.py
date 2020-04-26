import paho.mqtt.client as mqtt

client = mqtt.Client()
client.connect("mqtt.thingspeak.com",1883,60)

channelId = "1046955"
apiKey = "R2GFC1Q7A5Y999I0" #Write
#apiKey = "3CIKRBES3MDG9W7J" #Read
client.publish("channels/%s/publish/%s" % (channelId, apiKey),
               "field1=10&field2=543")
client.loop_forever(2)
