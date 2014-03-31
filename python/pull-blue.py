#!/usr/bin/env python
#
# simple script to repeatedly publish an MQTT message
#
# uses the Python MQTT client from the Paho project
# http://eclipse.org/paho
#
# Andy Piper @andypiper http://andypiper.co.uk
# 
# 2011/09/15 first version 
# 2012/05/28 updated to use new pure python client from mosquitto 
# 2014/02/03 updated to use the Paho client 
#
# pip install paho-mqtt
# python blast.py

import paho.mqtt.client as paho
import os
import time
import sys
import random


random.seed()

# Create a client
mypid = os.getpid()
client_uniq = "pubclient_"+str(mypid)
mqttc = paho.Client(client_uniq, True) #clean session

# connect to broker
broker = "localhost"
port = 1883
mqttc.connect(broker, port, 60)

msg = "blue"

# remain connected and publish
while mqttc.loop() == 0:
  mqttc.publish("vote", "!" + msg + "~", 0, False) #qos=0, retain=n
  print "published: %s" % msg
  time.sleep(random.uniform(0.1, 1))
  pass
