#ifndef VISRUN_SV_BLT_H
#define VISRUN_SV_BLT_H

#include <Arduino.h>

#include "pswled.h"
#include "visrun.h"

void	Init_SV_BLT(enum StateValue);
void	Main_SV_BLT();
void	Quit_SV_BLT(enum StateValue);

#endif // !VISRUN_SV_BLT_H
