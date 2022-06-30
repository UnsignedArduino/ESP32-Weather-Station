#ifndef _NTP_TIME_H_INCLUDED
#define _NTP_TIME_H_INCLUDED

//====================================================================================
//                                  Libraries
//====================================================================================

// Time library:
// https://github.com/PaulStoffregen/Time
#include <Time.h>

// Time zone correction library:
// https://github.com/JChristensen/Timezone
#include <Timezone.h>

// Libraries built into IDE
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

#include <WiFiUdp.h>

// A UDP instance to let us send and receive packets over UDP
extern WiFiUDP udp;

//====================================================================================
//                                  Settings
//====================================================================================

#ifdef ESP32 // Temporary fix, ESP8266 fails to communicate with some servers...
// Try to use pool url instead so the server IP address is looked up from those available
// (use a pool server in your own country to improve response time and reliability)
//extern const char* ntpServerName;
//extern const char* ntpServerName;
extern const char* ntpServerName;
#else
// Try to use pool url instead so the server IP address is looked up from those available
// (use a pool server in your own country to improve response time and reliability)
// const char* ntpServerName = "time.nist.gov";
extern const char* ntpServerName;
//extern const char* ntpServerName;
#endif

// Try not to use hard-coded IP addresses which might change, you can if you want though...
//extern IPAddress timeServerIP;   // time-c.nist.gov NTP server
//extern IPAddress timeServerIP; // wwv.nist.gov NTP server
extern IPAddress timeServerIP;                     // Use server pool

// Example time zone and DST rules, see Timezone library documents to see how
// to add more time zones https://github.com/JChristensen/Timezone

// Zone reference "UK" United Kingdom (London, Belfast)
extern TimeChangeRule BST;        //British Summer (Daylight saving) Time
extern TimeChangeRule GMT;         //Standard Time
extern Timezone UK;

// Zone reference "euCET" Central European Time (Frankfurt, Paris)
extern TimeChangeRule CEST;     //Central European Summer Time
extern TimeChangeRule  CET;      //Central European Standard Time
extern Timezone euCET;

// Zone reference "ausET" Australia Eastern Time Zone (Sydney, Melbourne)
extern TimeChangeRule aEDT;    //UTC + 11 hours
extern TimeChangeRule aEST;    //UTC + 10 hours
extern Timezone ausET;

// Zone reference "usET US Eastern Time Zone (New York, Detroit)
extern TimeChangeRule usEDT;  //Eastern Daylight Time = UTC - 4 hours
extern TimeChangeRule usEST;   //Eastern Standard Time = UTC - 5 hours
extern Timezone usET;

// Zone reference "usCT" US Central Time Zone (Chicago, Houston)
extern TimeChangeRule usCDT;
extern TimeChangeRule usCST;
extern Timezone usCT;

// Zone reference "usMT" US Mountain Time Zone (Denver, Salt Lake City)
extern TimeChangeRule usMDT;
extern TimeChangeRule usMST;
extern Timezone usMT;

// Zone reference "usAZ" Arizona is US Mountain Time Zone but does not use DST
extern Timezone usAZ;

// Zone reference "usPT" US Pacific Time Zone (Las Vegas, Los Angeles)
extern TimeChangeRule usPDT;
extern TimeChangeRule usPST;
extern Timezone usPT;


//====================================================================================
//                                  Variables
//====================================================================================
extern TimeChangeRule *tz1_Code;   // Pointer to the time change rule, use to get the TZ abbrev, e.g. "GMT"

extern time_t utc;

extern bool timeValid;

extern unsigned int localPort;      // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

extern byte packetBuffer[ NTP_PACKET_SIZE ]; //buffer to hold incoming and outgoing packets

extern uint8_t lastMinute;

extern uint32_t nextSendTime;
extern uint32_t newRecvTime;
extern uint32_t lastRecvTime;

extern uint32_t newTickTime;
extern uint32_t lastTickTime;

extern bool rebooted;

extern uint32_t no_packet_count;

//====================================================================================
//                                    Function prototype
//====================================================================================

void syncTime(void);
void displayTime(void);
void printTime(time_t zone, char *tzCode);
void decodeNTP(void);
void sendNTPpacket(IPAddress& address);

#endif
