/******************************************************************************
 *                                                                            *
 * NAME: SSTuino_Companion.h                                                  *
 *                                                                            *
 * PURPOSE: Main header file for the Arduino library                          *
 *                                                                            *
 * GLOBAL VARIABLES:                                                          *
 *                                                                            *
 * Variable Type Description                                                  *
 * -------- ---- -----------                                                  *
 *                                                                            *
 *****************************************************************************/

#ifndef __SSTuino_Companion__
#define __SSTuino_Companion__

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

/*
 * Enumerations and structs
 */

struct ReturnedData {
    int8_t linkID = -1;
    String content = "";
};

enum Status {
    SUCCESSFUL = 0,
    UNSUCCESSFUL,
    IN_PROGRESS,
    NOT_ATTEMPTED
};

enum HTTP_Operation {
    GET,
    POST
};

/*
 * Class declaration
 */

class SSTuino {
public:
    SSTuino();
    void rawInput(String input);
    void openLink();
    void slowOpenLink(int delayTime=5000);

    // Basic functionality
    bool smokeTest();
    String getVersion();
    void reset();

    // Wi-fi functionality
    String getWifiHotspots();
    bool wifiInRange(const String& ssid);
    void connectToWifi(const String& ssid, const String& password);
    Status getWifiStatus();
    void disconnectWifi();

    // Network functionality
    String getIP();
//     int16_t beginDeepSleep(uint16_t sleepTime, bool blocking);

//     int16_t setWiFiMode(WiFiMode mode);
//     int16_t listAP();
//     int16_t joinAP(String ssid, String password);
//     int16_t disconnectAP();
//     int16_t getLocalIP();
//     int16_t setDHCPEnabled(bool enabled);
//     int16_t setStationName(String& name);
//     int16_t setSoftAPSettings(String& ssid, String& password, uint8_t channel, EncryptionMethod encryptionMethod);
//     int16_t setIP(bool permanent, String ip, String gateway="", String netmask="");

    int16_t wait(char* values, uint16_t timeOut);
private:
    SoftwareSerial _ESP01UART;
    void writeCommandFromPROGMEM(const char* text);
    int16_t waitNoOutput(char* values, uint16_t timeOut);
    void rx_empty(void);
    bool recvFind(String target, uint32_t timeout, uint8_t reserve=8);
    String recvString(String target, uint32_t timeout, uint8_t reserve);
    // bool debug;
};


#endif  // End of __SSTuino_Companion__ definition check
