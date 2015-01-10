#!/usr/bin/env python

from time import sleep
from RF24 import *


class Light:
    def __init__(self):
        self.radio = RF24(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)
        pipes = ['1Node', '2Node']

        self.radio.begin()
        self.radio.setAutoAck(1)
        self.radio.enableAckPayload()

        self.radio.setChannel(10)

        # Set fixed minimum fixed payload size to improve range
        self.radio.payloadSize = 2

        self.radio.openWritingPipe(pipes[1])
        self.radio.openReadingPipe(1, pipes[0])

        self.radio.printDetails()

    def turn(self, on):
        send_data = '1' if on else '0'
        ack_data = ''
        result = False
        if self.radio.write(send_data):
            print "Successfully send data: " + send_data
            while self.radio.available():
                ack_data = self.radio.read(1)
                print "Received ack: " + ack_data
                result = True
        else:
            print "Error sending data: " + send_data
        return result

    def getStatus(self):
        if self.radio.write('2'):
            print "Successfully send data 2"
            while self.radio.available():
                ack_data = self.radio.read(1)
                print "Received ack: " + ack_data
                return ack_data[:1] == '1'
        else:
            print "Error sending data"


if __name__ == "__main__":
    light = Light()
    while True:
        turn = raw_input("Turn light (on/off/get): ")
        if turn == 'on':
            light.turn(True)
        elif turn == 'off':
            light.turn(False)
        elif turn == 'get':
            print "Ligh is: " + ('on' if light.getStatus() else 'off')
        else:
            print "Unknown option."
