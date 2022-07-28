#include <Arduino.h>

#include "gps.h"
#include "pswled.h"
#include "sv_nop.h"
#include "visrun.h"

void
Init_SV_NOP(enum StateValue PrevState)
{
	digitalWrite(D08_GPSTX, HIGH);
	digitalWrite(D04_nPON, LOW);
	delay(500);
	if (PrevState == SV_OFF) {
		SendGps(PMTK_CMD_FULL_COLD_START);
		SendGps(PMTK_API_SET_DATUM, 0);
		SendGps(PMTK_API_SET_STATIC_NAV_THD, 0.5);
	}
}

void
Main_SV_NOP()
{
	(void)0;
}

void
Quit_SV_NOP(enum StateValue NextState)
{
	FlushGps();
	if (NextState == SV_OFF)
		digitalWrite(D08_GPSTX, LOW);
}
