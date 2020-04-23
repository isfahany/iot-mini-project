import paho.mqtt.client as mqtt
import time
import thingspeak
from ast import literal_eval

MQTT_BROKER =
MQTT_PORT =
MQTT_TOPIC =
THINGSPEAK_CHANNEL =
THINGSPEAK_API = 
UPDATE_INTERVAL = 

client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT)
client.loop_start()

while True:
    thing = thingspeak.Channel(THINGSPEAK_CHANNEL, THINGSPEAK_API)
    n = literal_eval(thing.get_field_last(field='1'))
    client.publish(MQTT_TOPIC, n)
    time.sleep(UPDATE_INTERVAL)
