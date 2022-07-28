#include <Arduino.h>

#include "gps.h"
#include "pswled.h"

#include "sv_off.h"
#include "sv_nop.h"
#include "sv_rec.h"
#include "sv_blt.h"

#include "visrun.h"

volatile enum StateValue StateValue = SV_OFF;

static void (* const InitState[])(enum StateValue) = {
	Init_SV_OFF,
	Init_SV_NOP,
	Init_SV_REC,
	Init_SV_BLT,
};
static void (* const MainState[])() = {
	Main_SV_OFF,
	Main_SV_NOP,
	Main_SV_REC,
	Main_SV_BLT,
};
static void (* const QuitState[])(enum StateValue) = {
	Quit_SV_OFF,
	Quit_SV_NOP,
	Quit_SV_REC,
	Quit_SV_BLT,
};

static void
common()
{
	static char queue[8];
	static int qpos = 0;
	register int i, stat;

	while (Serial.available())
		queue[qpos++ & 7] = Serial.read();
	qpos &= 7;
	stat = 0;
	for (i = 0; i < 16 && stat < 3; i++)
		if (queue[i & 7] == 'Z')
			stat++;
	if (stat == 3) {
		for (i = 0; i < 8; i++)
			queue[i] = 0;
		qpos = 0;
		if (StateValue == SV_NOP)
			StateValue = SV_BLT;
	}
}

void
setup()
{
	InitPswLed();
	InitGps();
	Serial.begin(115200);
	while (!Serial);
}

void
loop()
{
	static enum StateValue PrevState = SV_XXX;
	const enum StateValue CurrentState = StateValue;

	if (PrevState != CurrentState) {
		if (PrevState != SV_XXX)
			QuitState[PrevState](CurrentState);
		InitState[CurrentState](PrevState);
		PrevState = CurrentState;
	}
	MainState[CurrentState]();
	common();
}
