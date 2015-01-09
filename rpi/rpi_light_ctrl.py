#!/usr/bin/env python

from time import sleep
from RF24 import *
from flask import Flask, render_template
import threading

app = Flask(__name__)

#RPi B
# Setup for GPIO 24 CE and CE0 CSN with SPI Speed @ 8Mhz
radio = RF24(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)

pipes = ['1Node', '2Node']

radio.begin()

radio.setChannel(10)

radio.openWritingPipe(pipes[1])

radio.printDetails()

@app.route("/light/")
def light_index():
    return render_template('index.html')

@app.route("/light/<state>")
def turn_lgith(state):
    radio.stopListening()
    if state == 'on':
        radio.write('1')
    elif state == 'off':
        radio.write('0')
    radio.startListening()
    return render_template('index.html')

if __name__ == "__main__":
    app.run(host='0.0.0.0')
