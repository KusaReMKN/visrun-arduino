#include <Arduino.h>

#include "pswled.h"
#include "sv_off.h"
#include "visrun.h"

void
Init_SV_OFF(enum StateValue PrevState)
{
	digitalWrite(D04_nPON, HIGH);
	digitalWrite(D08_GPSTX, LOW);
}

void
Main_SV_OFF()
{
	(void)0;
}

void
Quit_SV_OFF(enum StateValue NextState)
{
	digitalWrite(D04_nPON, LOW);
}
