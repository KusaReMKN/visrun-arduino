#include <stdarg.h>

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>

#include "gps.h"

SoftwareSerial	GpsModule(D07_GPSRX, D08_GPSTX);
TinyGPSPlus	gps;

void
FlushGps()
{
	while (GpsModule.read() != -1);
}

void
InitGps()
{
	GpsModule.begin(GPS_BAUD);
	delay(100UL);	// may not be necessary?
}

int
SendGps(const char *fmt, ...)
{
	register int i, cksum;
	register va_list ap;
	char buf[100], tail[6];

	if (*fmt != '$')
		return -1;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	cksum = 0;
	for (i = 1; buf[i] != '\0'; i++)
		cksum ^= buf[i];
	snprintf(tail, sizeof(tail), "*%02X\r\n", cksum & 0xFF);

	GpsModule.print(strcat(buf, tail));

	return 0;
}
