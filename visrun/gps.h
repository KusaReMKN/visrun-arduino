#ifndef VISRUN_GPS_H
#define VISRUN_GPS_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>

#define D07_GPSRX	7
#define D08_GPSTX	8

#define GPS_BAUD	9600L

#define PMTK_TEST			("$PMTK000")
#define PMTK_ACK
#define PMTK_SYS_MSG			("$PMTK010,%03d")
#define PMTK_CMD_HOT_START		("$PMTK101")
#define PMTK_CMD_WARN_START		("$PMTK102")
#define PMTK_CMD_COLD_START		("$PMTK103")
#define PMTK_CMD_FULL_COLD_START	("$PMTK104")
#define PMTK_CMD_CLEAR_FLASH_AID	("$PMTK120")
#define PMTK_CMD_STANDBY_MODE		("$PMTK161,%d")
#define PMTK_LOCUS_QUERY_STATUS
#define PMTK_LOCUS_ERASE_FLASH		("$PMTK184,%d")
#define PMTK_LOCUS_STOP_LOGGER		("$PMTK185,%d")
#define PMTK_LOCUS_LOG_NOW		("$PMTK186,%d")
#define PMTK_SET_AL_DEE_CFG
#define PMTK_SET_PERIODIC_MODE
#define PMTK_SET_NMEA_BAUDRATE		("$PMTK251,%ld")
#define PMTK_API_SET_FIX_CTL		("$PMTK300,%d,0,0,0,0")
#define PMTK_API_SET_DGPS_MODE		("$PMTK301,%d")
#define PMTK_API_SET_SBAS_ENABLED	("$PMTK313,%d")
#define PMTK_API_SET_NMEA_OUTPUT	
#define PMTK_API_SET_DATUM		("$PMTK330,%d")
#define PMTK_API_SET_DATUM_ADVANCE
#define PMTK_API_SET_RTC_TIME
#define PMTK_API_SET_SUPPORT_QZSS_NMEA	("$PMTK351,%d")
#define PMTK_API_SET_STOP_QZSS		("$PMTK352,%d")
#define PMTK_API_SET_STATIC_NAV_THD	("$PMTK386,%.1f")
#define PMTK_API_SET_TCXO_DEBUG		("$PMTK389,%d")
#define PMTK_Q_RELEASE			("$PMTK605")
#define PMTK_Q_EPO_INFO			("$PMTK607")
#define PMTK_Q_LOCUS_DATA		("$PMTK622")
#define PMTK_Q_AVAILABLE_SV_EPH
#define PMTK_Q_AVAILABLE_SV_ALM
#define PMTK_DT_RELEASE
#define PMTK_DT_UTC
#define PMTK_DT_POS
#define PMTK_EASY_ENABLE		("$PMTK869,%d,%d")

extern SoftwareSerial	GpsModule;
extern TinyGPSPlus	gps;

void	FlushGps();
void	InitGps();
int	SendGps(const char *fmt, ...);

#endif // !VISRUN_GPS_H
