#!/usr/bin/python
# 

import paho.mqtt.client as paho
import string
import random
from time import sleep
import time
import threading
import serial
#import sys

#sys.stdout = open('log2.log', 'w')

#mqtt setup 
host = "localhost"
topic = "vote"

#setup serial
s = serial.Serial(port = "/dev/tty.usbserial-A601ZJZN", baudrate=19200)
s.flushInput()
s.close()
s.open()

#generate uniq client id
def id_generator(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for x in range(size))

#print out connection status
def on_connect(mosq, obj, rc):
    print 'on_connect:'
    if rc == 0:
        print "Connected"
        mqttc.subscribe(topic, 0)
    else:
        print rc

#On recipt of a message - print it
def on_message(mosq, obj, msg):
    msgTopic = msg.topic
    msgPayload = msg.payload
    print "topic: " + msgTopic
    print "message: " + msgPayload
    s.write(msgPayload)

#on subscribe
def on_subscribe(mosq, obj, mid, qos_list):
    print("Subscribe with mid "+str(mid)+" received.")



#create a broker
mqttc = paho.Client(id_generator())

#define the callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe

#connect
print 'connecting...'
mqttc.connect(host, 1883)

#wait a bit for connection
#sleep(2)

#subscribe to topic test
#mqttc.subscribe(topic, 0)

def loop_thread(mqttclient):
    print "im in da thread"
    while 1:
        mqttclient.loop(timeout = 0)

threading.Thread(target = loop_thread, args = (mqttc,)).start()

#keep connected to broker
#while 1:
    #mqttc.loop()    
