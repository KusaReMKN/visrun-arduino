#include <Arduino.h>

#include "gps.h"
#include "pswled.h"
#include "sdcard.h"
#include "sv_rec.h"
#include "visrun.h"

static long		dateValue = -1;
static long		timeValue = -1;
static bool		isOpened  = false;
static File		file;

static inline long
chgdir()
{
	long tmpDate = gps.date.value();
	char dirName[8];

	if (tmpDate == dateValue)
		return 0;
	dateValue = tmpDate;
	snprintf(dirName, sizeof(dirName), "/%06ld", dateValue);
	if (!SD.exists(dirName))
		if (!SD.mkdir(dirName))
			return -1;

	return dateValue;
}

static inline long
chgfile()
{
	long tmpTime = gps.time.value() / 10000;
	char fileName[13];

	if (tmpTime == timeValue)
		return 0;
	if (isOpened) {
		file.println("]");
		file.close();
		isOpened = false;
	}
	timeValue = tmpTime;
	snprintf(fileName, sizeof(fileName), "/%06ld/%04ld",
			dateValue, timeValue);
	file = SD.open(fileName, FILE_WRITE);
	if (file == false)
		return -1;
	isOpened = true;
	file.println("[");

	return timeValue;
}

static inline void
logdata()
{
	char tmpbuf[16];

	file.print("\t[ ");
	snprintf(tmpbuf, sizeof(tmpbuf), "%06lu%08lu",
			gps.date.value(), gps.time.value());
	file.print(tmpbuf);
	file.print(", ");
	snprintf(tmpbuf, sizeof(tmpbuf), "%s%u.%09lu",
			gps.location.rawLat().negative ? "-" : "",
			gps.location.rawLat().deg,
			gps.location.rawLat().billionths);
	file.print(tmpbuf);
	file.print(", ");
	snprintf(tmpbuf, sizeof(tmpbuf), "%s%u.%09lu",
			gps.location.rawLng().negative ? "-" : "",
			gps.location.rawLng().deg,
			gps.location.rawLng().billionths);
	file.print(tmpbuf);
	file.println(" ],");
}

void
Init_SV_REC(enum StateValue PrevState)
{
	digitalWrite(D04_nPON, LOW);
	delay(500);

	SD.begin();
}

void
Main_SV_REC()
{
	while (GpsModule.available())
		gps.encode(GpsModule.read());
	if (gps.date.isUpdated())
		chgdir();
	if (gps.time.isUpdated() && dateValue != -1)
		chgfile();
	if (gps.location.isUpdated() && isOpened)
		logdata();
}

void
Quit_SV_REC(enum StateValue NextState)
{
	if (isOpened) {
		file.println("]");
		file.close();
		isOpened = false;
	}
	delay(500);
}
