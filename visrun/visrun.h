#ifndef VISRUN_H
#define VISRUN_H

#include <Arduino.h>

#include "gps.h"
#include "pswled.h"

enum StateValue { SV_OFF, SV_NOP, SV_REC, SV_BLT, SV_MAX, SV_XXX = -1 };

extern volatile enum StateValue StateValue;

#endif // !VISRUN_H
