#ifndef VISRUN_SV_OFF_H
#define VISRUN_SV_OFF_H

#include <Arduino.h>

#include "pswled.h"
#include "visrun.h"

void	Init_SV_OFF(enum StateValue);
void	Main_SV_OFF();
void	Quit_SV_OFF(enum StateValue);

#endif // !VISRUN_SV_OFF_H
