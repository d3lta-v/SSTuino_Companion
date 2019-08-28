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
    GET = 'G',
    POST = 'P'
};

enum HTTP_Content {
    CONTENT,
    HEADERS
};

/*
 * Class declaration
 */

class SSTuino {
public:
    SSTuino();
    // void rawInput(String input);
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

    // HTTP operations
    int setupHTTP(HTTP_Operation op, const String& url);
    bool setHTTPPOSTParameters(int handle, const String& data);
    bool setHTTPHeaders(int handle, const String& data);
    bool transmitHTTP(int handle);

    Status getHTTPProgress(int handle);

    int getHTTPStatusCode(int handle);
    String getHTTPReply(int handle, HTTP_Content field, bool deleteReply);
    bool deleteHTTPReply(int handle);
//     int16_t beginDeepSleep(uint16_t sleepTime, bool blocking);

//     int16_t setDHCPEnabled(bool enabled);
//     int16_t setStationName(String& name);
//     int16_t setSoftAPSettings(String& ssid, String& password, uint8_t channel, EncryptionMethod encryptionMethod);
//     int16_t setIP(bool permanent, String ip, String gateway="", String netmask="");

    int16_t wait(char* values, uint16_t timeOut);
private:
    SoftwareSerial _ESP01UART;
    void writeCommandFromPROGMEM(const char* text, int buffersize=8);
    int16_t waitNoOutput(char* values, uint16_t timeOut);
    void rx_empty(void);
    bool recvFind(String target, uint32_t timeout, uint8_t reserve=8);
    String recvString(String target, uint32_t timeout, uint8_t reserve);
    // bool debug;
};


#endif  // End of __SSTuino_Companion__ definition check
