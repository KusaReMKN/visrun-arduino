#ifndef VISRUN_SV_NOP_H
#define VISRUN_SV_NOP_H

#include <Arduino.h>

#include "gps.h"
#include "pswled.h"
#include "visrun.h"

void	Init_SV_NOP(enum StateValue);
void	Main_SV_NOP();
void	Quit_SV_NOP(enum StateValue);

#endif // !VISRUN_SV_NOP_H
