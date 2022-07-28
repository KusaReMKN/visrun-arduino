#include <Arduino.h>
#include <MsTimer2.h>

#include "pswled.h"
#include "visrun.h"

enum PressType { PT_IGNORE, PT_SHORT, PT_LONG, PT_MAX, PT_XXX = -1 };

static inline enum PressType
GetPressType()
{
	static int TimerCount = 0;
	static int PressStart = -1;
	static bool isPressing = false;
	register int PressingCount = 0;
	register enum PressType PressType = PT_IGNORE;

#define MAX_TIMERCOUNT	10000
#define CalcPressingCount(s, e) \
	(((e) - (s) + MAX_TIMERCOUNT) % MAX_TIMERCOUNT)
	if (PressStart != -1)
		PressingCount = CalcPressingCount(PressStart, TimerCount);
	TimerCount = (TimerCount + 1) % MAX_TIMERCOUNT;

#define LONG_PRESS	30
#define SHORT_PRESS	5
	if (digitalRead(D02_nPSW) == LOW) {
		if (!isPressing) {
			PressStart = TimerCount;
			isPressing = true;
		} else {
			if (PressingCount > LONG_PRESS) {
				PressType = PT_LONG;
				PressStart = -1;
			}
		}
	} else {
		if (isPressing) {
			if (PressingCount > LONG_PRESS)
				PressType = PT_SHORT;
			else if (PressingCount > SHORT_PRESS)
				PressType = PT_SHORT;
		}
		PressStart = -1;
		isPressing = false;
	}

	return PressType;
}

static inline void
UpdateStateValue()
{
	switch (GetPressType()) {
	case PT_SHORT:
		switch (StateValue) {
		case SV_NOP:
			StateValue = SV_REC;
			break;
		case SV_REC:
			StateValue = SV_NOP;
			break;
		default:
			(void)0;
		}
		break;
	case PT_LONG:
		switch (StateValue) {
		case SV_OFF:
			StateValue = SV_NOP;
			break;
		case SV_NOP:
		case SV_REC:
			StateValue = SV_OFF;
		default:
			(void)0;
		}
		break;
	default:
		(void)0;
	}
}

static inline void
BlinkLed()
{
	static int Count = 0;

	Count = (Count + 1) % 10;
	switch (StateValue) {
	case SV_NOP:
		digitalWrite(D03_nLED, LOW);
		break;
	case SV_REC:
		digitalWrite(D03_nLED, Count < 5 ? LOW : HIGH);
		break;
	case SV_BLT:
		digitalWrite(D03_nLED, Count < 5 && Count & 1 ? LOW : HIGH);
		break;
	default:
		digitalWrite(D03_nLED, HIGH);
	}
}

static void
TimerHandler()
{
	UpdateStateValue();
	BlinkLed();
}

void
InitPswLed()
{
	pinMode(D02_nPSW, INPUT_PULLUP);
	digitalWrite(D02_nPSW, HIGH);

	pinMode(D03_nLED, OUTPUT);
	digitalWrite(D03_nLED, HIGH);

	pinMode(D04_nPON, OUTPUT);
	digitalWrite(D04_nPON, HIGH);

#define TIMER_INTERVAL	100
	MsTimer2::set(TIMER_INTERVAL, TimerHandler);
	MsTimer2::start();
}
