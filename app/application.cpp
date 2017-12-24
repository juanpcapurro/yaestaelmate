#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "pinout.h"

Timer procTimer;
bool state = true;

void blink()
{
//	digitalWrite(LED_RED, state);
//	digitalWrite(LED_BLUE, !state);
	state = !state;
    Serial.printf("led state: %i \n", state);
}

void init()
{
    Serial.begin(SERIAL_BAUD_RATE);
//	pinMode(LED_RED, OUTPUT);
//	pinMode(LED_BLUE, OUTPUT);
	procTimer.initializeMs(500, blink).start();
}
