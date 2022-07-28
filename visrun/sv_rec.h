#ifndef VISRUN_SV_REC_H
#define VISRUN_SV_REC_H

#include <Arduino.h>

#include "gps.h"
#include "pswled.h"
#include "sdcard.h"
#include "visrun.h"

void	Init_SV_REC(enum StateValue);
void	Main_SV_REC();
void	Quit_SV_REC(enum StateValue);

#endif // !VISRUN_SV_REC_H
