
#include <stdio.h>
#include "Arduino.h"
#include "RF24.h"

#define CMD_LIGHT_OFF   '0'
#define CMD_LIGHT_ON    '1'

RF24 radio(9, 10);
byte addresses[][6] = {"1Node", "2Node"};
static bool light_on;

static FILE uartout = {0};

static int uart_putchar(char c, FILE* stream)
{
    Serial.write(c);

    return 0;
}

static void radio_init()
{
    radio.begin();
    
    radio.setChannel(10);
    
    radio.setPALevel(RF24_PA_MAX);
    radio.setAutoAck(1);
    radio.enableAckPayload();

    // Set fixed payload size to 1 char + \0 to improve range
    radio.setPayloadSize(2);

    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);

    radio.startListening();

    radio.printDetails();
    Serial.print("Payload size: ");
    Serial.println(radio.getPayloadSize());

    delay(1000);
}

void setup()
{
    Serial.begin(57600);
    Serial.println("Starting...");
    fdev_setup_stream(&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uartout;
    
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    light_on = true;

    radio_init();
}

void loop()
{
    static bool button_state = false;

    byte pipeNumber;

    while (radio.available(&pipeNumber))
    {
        byte recv_data;
        radio.read(&recv_data, 1);
        if (CMD_LIGHT_OFF == recv_data)
        {
            digitalWrite(2, HIGH);
            light_on = false;
        }
        else if (CMD_LIGHT_ON == recv_data)
        {
            digitalWrite(2, LOW);
            light_on = true;
        }

        byte ack_data = (light_on) ? CMD_LIGHT_ON : CMD_LIGHT_OFF;
        radio.writeAckPayload(pipeNumber, &ack_data, 1);

        if (CMD_LIGHT_OFF == recv_data)
        {
            Serial.println("Recevied command to turn light off.");
        }
        else if (CMD_LIGHT_ON == recv_data)
        {
            Serial.println("Recevied command to turn light on.");
        }
        else
        {
            Serial.println("Received unknown command.");
        }
    }
}
