
#include <stdio.h>
#include "Arduino.h"
#include "RF24.h"

#define CMD_LIGHT_OFF   '0'
#define CMD_LIGHT_ON    '1'

RF24 radio(9, 10);
byte addresses[][6] = {"1Node", "2Node"};

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

    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);

    radio.startListening();

    radio.printDetails();

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

    radio_init();
}

void loop()
{
    static bool button_state = false;

    if (radio.available())
    {
        while (radio.available())
        {
            byte data;
            radio.read(&data, 1);
            if (CMD_LIGHT_OFF == data)
            {
                Serial.println("Recevied command to turn light off.");
                digitalWrite(2, HIGH);
            }
            else if (CMD_LIGHT_ON == data)
            {
                Serial.println("Recevied command to turn light on.");
                digitalWrite(2, LOW);
            }
            else
            {
                Serial.println("Received unknown command.");
            }
        }
    }
}
