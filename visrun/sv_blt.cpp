#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "pswled.h"
#include "sdcard.h"
#include "sv_blt.h"
#include "visrun.h"

static char *
GetSerial(char *buf, int len)
{
	register int i;

	for (i = 0; i < len-1; i++) {
		while ((buf[i] = Serial.read()) == -1);
		if (buf[i] == '\r')
			break;
	}
	buf[i] = '\0';
	if (Serial.peek() == '\n')
		(void)Serial.read();

	return buf;
}

static inline void
SendStatus(int code)
{
#define STATUS	F("S\t")
	Serial.print(STATUS);
	Serial.println(code);
}

static inline void
SendTermin(int code)
{
#define TERMIN	F("T\t")
	Serial.print(TERMIN);
	Serial.println(code);
}

static void
dump(char *cmd)
{
	register char *filename;
	register int i, c;
	File file;

	SD.begin();
	for (i = 4; cmd[i] && cmd[i] == ' '; i++);
	filename = cmd[i] ? cmd + i : "";
	if (!(file = SD.open(filename))) {
		SendStatus(1), SendTermin(1);
		return;
	}
	if (file.isDirectory()) {
		SendStatus(2), SendTermin(2);
		file.close();
		return;
	}
	SendStatus(0);
	while ((c = file.peek()) != -1) {
#define CONTENT	F("C\t")
		Serial.print(CONTENT);
		while ((c = file.read()) != -1 && c != '\r')
			Serial.write(c);
		if (c == '\r' && file.peek() == '\n')
			file.read();
		Serial.println();
	}
	SendTermin(0);
	file.close();
}

static void
list(char *cmd)
{
	register char *dirname;
	register int i;
	File dir, elem;

	SD.begin();
	for (i = 4; cmd[i] && cmd[i] == ' '; i++);
	dirname = cmd[i] ? cmd + i : "/";
	if (!(dir = SD.open(dirname))) {
		SendStatus(1), SendTermin(1);
		return;
	}
	if (!dir.isDirectory()) {
		SendStatus(2), SendTermin(2);
		dir.close();
		return;
	}
	SendStatus(0);
	while (elem = dir.openNextFile()) {
		Serial.print(elem.isDirectory() ? F("D\t") : F("F\t"));
		Serial.println(elem.name());
		elem.close();
	}
	SendTermin(0);
	dir.close();
}

static void
quit(char *_)
{
	SendStatus(0);
	StateValue = SV_NOP;
	SendTermin(0);
}

static void
rdir(char *cmd)
{
	register char *filename;
	register int i, ret;

	SD.begin();
	for (i = 4; cmd[i] && cmd[i] == ' '; i++);
	filename = cmd[i] ? cmd + i : "";
	ret = SD.rmdir(filename) != 1;
	SendStatus(ret), SendTermin(ret);
}

static void
remv(char *cmd)
{
	register char *filename;
	register int i, ret;

	SD.begin();
	for (i = 4; cmd[i] && cmd[i] == ' '; i++);
	filename = cmd[i] ? cmd + i : "";
	ret = SD.remove(filename) != 1;
	SendStatus(ret), SendTermin(ret);
}

struct {
	char *cmd;
	void (*func)(char *);
} cmdfunc[] = {
	{ "DUMP", dump },
	{ "LIST", list },
	{ "QUIT", quit },
	{ "RDIR", rdir },
	{ "REMV", remv },
	{ NULL,   NULL },
};

void
Init_SV_BLT(enum StateValue PrevState)
{
	while (Serial.read() != -1);
}

void
Main_SV_BLT()
{
	char buf[32];
	register int i, len;

	while (StateValue == SV_BLT) {

		GetSerial(buf, sizeof(buf));
		for (i = 0; cmdfunc[i].cmd != NULL; i++) {
			len = strlen(cmdfunc[i].cmd);
			if (!strncmp(buf, cmdfunc[i].cmd, len)) {
				cmdfunc[i].func(buf);
				break;
			}
		}
		if (cmdfunc[i].cmd == NULL) {
			Serial.print( F("Unknown Command -- ") );
			Serial.println(buf);
		}
	}
}

void
Quit_SV_BLT(enum StateValue NextState)
{
	(void)0;
}
