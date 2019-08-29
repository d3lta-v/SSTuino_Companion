/******************************************************************************
 *                                                                            *
 * FILE NAME: SSTuino_Companion.c                                             *
 *                                                                            *
 * PURPOSE: Contains the main implementation of the Arduino library           *
 *                                                                            *
 * FILE REFERENCES:                                                           *
 *                                                                            *
 * Name I/O Description                                                       *
 * ---- --- -----------                                                       *
 * none                                                                       *
 *                                                                            *
 * EXTERNAL VARIABLES:                                                        *
 * Source: .h                                                                 *
 *                                                                            *
 * Name Type I/O Description                                                  *
 * ---- ---- --- -----------                                                  *
 *                                                                            *
 * EXTERNAL REFERENCES:                                                       *
 *                                                                            *
 * Name Description                                                           *
 * ---- -----------                                                           *
 *                                                                            *
 * ABNORMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: none          *
 *                                                                            *
 * ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS: none                               *
 *                                                                            *
 * NOTES: none                                                                *
 *                                                                            *
 * REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES: N/A                     *
 *                                                                            *
 * ALGORITHM (PDL)                                                            *
 *                                                                            *
 *****************************************************************************/

#include "SSTuino_Companion.h"

/*
 * Constants stored in program memory
 */

// Basic constant strings
const char NEWLINE[] = "\r\n";              // Newline is not in PROGMEM due to frequent use
const char DELIMITER[] PROGMEM = "\x1f";

// Basic commands
const char NOOPERATION[] PROGMEM = "nop\r\n";
const char VERSION[] PROGMEM = "ver\r\n";
const char RESET[] PROGMEM = "rst\r\n";

// Wi-Fi commands
const char CONNECTAP[] PROGMEM = "cap ";
const char LISTAP[] PROGMEM = "lap\r\n";
const char STATUSAP[] PROGMEM = "sap\r\n";
const char DISCONNECTAP[] PROGMEM = "dap\r\n";

// Networking commands
const char GETIP[] PROGMEM = "gip\r\n";

// HTTP commands
const char INITHTTP[] PROGMEM = "ihr ";
const char POSTPARAMSHTTP[] PROGMEM = "phr ";
const char HEADERSHTTP[] PROGMEM = "hhr ";
const char TRANSMITHTTP[] PROGMEM = "thr ";
const char STATUSHTTP[] PROGMEM = "shr ";
const char GETRESPONSEHTTP[] PROGMEM = "ghr ";
const char DELETERESPONSEHTTP[] PROGMEM = "dhr ";

// MQTT commands
const char MQTTCONFIGURE[] PROGMEM = "mcg ";
const char MQTTISCONNECTED[] PROGMEM = "mic\r\n";
const char MQTTSUB[] PROGMEM = "msb ";
const char MQTTUNSUB[] PROGMEM = "mus ";\
const char MQTTNEWDATA[] PROGMEM = "mnd ";
const char MQTTGETSUBDATA[] PROGMEM = "mgs ";
const char MQTTPUBLISH[] PROGMEM = "mpb ";

/******************************************************************************
 * Constructor                                                                *
 *****************************************************************************/

SSTuino::SSTuino() : _ESP01UART(2, 4) {
}

/******************************************************************************
 * Public functions                                                           *
 *****************************************************************************/

/* ---------------------------- Basic functions ---------------------------- */

/*!
 * @brief Opens the serial link from the SSTuino to the ESP-01 module
 */
void SSTuino::openLink() {
    _ESP01UART.begin(9600);
    rx_empty();
}

/*!
 * @brief Opens the serial link from the SSTuino to the ESP-01 module, and imposes an artificial delay
 *
 * @param delayTime The time to wait before opening the serial port. Defaults to 5 seconds.
 */
void SSTuino::slowOpenLink(int delayTime /* =5000 */) {
    delay(delayTime);
    _ESP01UART.begin(9600);
    rx_empty();
}

/*!
 * @brief A simple smoke test to see if the module responds
 *
 * @return true if the test is successful and false if the test times out
 */
bool SSTuino::smokeTest() {
    rx_empty();
    writeCommandFromPROGMEM(NOOPERATION);
    int16_t result = wait("\r\n", 1000);
    if (result == 0) return true;
    else return false;
}

String SSTuino::getVersion() {
    rx_empty();
    writeCommandFromPROGMEM(VERSION);
    return recvString(NEWLINE, 1000, 16);
}


/* ---------------------------- Wi-Fi functions ---------------------------- */

String SSTuino::getWifiHotspots() {
    rx_empty();
    writeCommandFromPROGMEM(LISTAP);
    return recvString(NEWLINE, 10000, 64);
}

bool SSTuino::wifiInRange(const String& ssid) {
    rx_empty();
    writeCommandFromPROGMEM(LISTAP);
    return recvFind(ssid, 10000, 64);
}

void SSTuino::connectToWifi(const String& ssid, const String& password) {
    rx_empty();
    writeCommandFromPROGMEM(CONNECTAP);
    _ESP01UART.print(ssid);
    writeCommandFromPROGMEM(DELIMITER);
    _ESP01UART.print(password);
    _ESP01UART.print(NEWLINE);
}

Status SSTuino::getWifiStatus() {
    rx_empty();
    writeCommandFromPROGMEM(STATUSAP);
    int16_t result = wait("S;U;P;N", 1000);
    if (result < 0) return UNSUCCESSFUL;
    return (Status)result;
}

void SSTuino::disconnectWifi() {
    rx_empty();
    writeCommandFromPROGMEM(DISCONNECTAP);
}

/* --------------------------- Network functions --------------------------- */

String SSTuino::getIP() {
    rx_empty();
    writeCommandFromPROGMEM(GETIP);
    return recvString(NEWLINE, 1000, 32);
}

/* ---------------------------- HTTP operations ---------------------------- */

int SSTuino::setupHTTP(HTTP_Operation op, const String& url) {
    rx_empty();
    writeCommandFromPROGMEM(INITHTTP);
    _ESP01UART.print((char)op);
    writeCommandFromPROGMEM(DELIMITER);
    _ESP01UART.print(url);
    _ESP01UART.print(NEWLINE);
    String data = recvString(NEWLINE, 1000, 8);
    if (data.charAt(0) == 'U') return -1; // -1 indicates that the function failed
    //TODO: can consider performing robust validation for whether it is an integer
    return data.toInt();
}

bool SSTuino::setHTTPPOSTParameters(int handle, const String& data) {
    rx_empty();
    writeCommandFromPROGMEM(POSTPARAMSHTTP);
    _ESP01UART.print(handle);
    writeCommandFromPROGMEM(DELIMITER);
    _ESP01UART.print(data);
    _ESP01UART.print(NEWLINE);
    int16_t result = wait("S;U;short;long", 1000);
    if (result == 0) return true;
    else return false;
}

bool SSTuino::setHTTPHeaders(int handle, const String& data) {
    rx_empty();
    writeCommandFromPROGMEM(HEADERSHTTP);
    _ESP01UART.print(handle);
    writeCommandFromPROGMEM(DELIMITER);
    _ESP01UART.print(data);
    _ESP01UART.print(NEWLINE);
    int16_t result = wait("S;U;short;long", 1000);
    if (result == 0) return true;
    else return false;
}

bool SSTuino::transmitHTTP(int handle) {
    rx_empty();
    writeCommandFromPROGMEM(TRANSMITHTTP);
    _ESP01UART.print(handle);
    _ESP01UART.print(NEWLINE);
    int16_t result = wait("S;U;short;long", 1000);
    if (result == 0) return true;
    else return false;
}

Status SSTuino::getHTTPProgress(int handle) {
    // WARNING: commands may not respond when ESP8266 CPU is overloaded such as during cryptographic operations!
    // potential 1202
    rx_empty();
    writeCommandFromPROGMEM(STATUSHTTP);
    _ESP01UART.print(handle);
    _ESP01UART.print(NEWLINE);
    int16_t result = wait("S;U;P;N", 1000);
    switch (result)
    {
    case 0:
        return SUCCESSFUL;
    case 1:
        return UNSUCCESSFUL;
    case 2:
        return IN_PROGRESS;
    case 3:
        return NOT_ATTEMPTED;
    default:
        return UNSUCCESSFUL;
    }
}

/* ------------------------------------------------------------------------- */

int SSTuino::getHTTPStatusCode(int handle) {
    rx_empty();
    writeCommandFromPROGMEM(GETRESPONSEHTTP);
    _ESP01UART.print(handle);
    writeCommandFromPROGMEM(DELIMITER);
    _ESP01UART.print('S');
    writeCommandFromPROGMEM(DELIMITER);
    _ESP01UART.print('F');
    _ESP01UART.print(NEWLINE);
    String data = recvString(F("\r\n"), 1000, 8);
    if (data.charAt(0) == 'U') return -1; // -1 indicates that the function failed
    //TODO: can consider performing robust validation for whether it is an integer
    return data.toInt();
}

String SSTuino::getHTTPReply(int handle, HTTP_Content field, bool deleteReply) {
    rx_empty();
    writeCommandFromPROGMEM(GETRESPONSEHTTP);
    _ESP01UART.print(handle);
    writeCommandFromPROGMEM(DELIMITER);
    if (field == HEADERS) _ESP01UART.print('H');
    if (field == CONTENT) _ESP01UART.print('C');
    writeCommandFromPROGMEM(DELIMITER);
    deleteReply ? _ESP01UART.print('T') : _ESP01UART.print('F');
    _ESP01UART.print(NEWLINE);
    return recvString(F("\r\n"), 2000, 64);
}

bool SSTuino::deleteHTTPReply(int handle) {
    rx_empty();
    writeCommandFromPROGMEM(DELETERESPONSEHTTP);
    _ESP01UART.print(handle);
    _ESP01UART.print(NEWLINE);
    int16_t result = wait("S;U;short;long", 1000);
    if (result == 0) return true;
    else return false;
}

/* ------------------------------------------------------------------------- */

/******************************************************************************
 * Private functions                                                          *
 *****************************************************************************/

/* --------------------------- Helper  functions --------------------------- */

/*!
 * @brief Flushes serial receive buffer to ensure no characters remaining in the buffer
 */
void SSTuino::rx_empty(void) 
{
    while(_ESP01UART.available() > 0) {
        _ESP01UART.read();
    }
}

String SSTuino::recvString(String target, uint32_t timeout, uint8_t reserve=8)
{
    String data((char *)0);
    data.reserve(reserve);
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(_ESP01UART.available() > 0) {
            a = _ESP01UART.read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target) != -1) {
            break;
        }
    }
    return data;
}

bool SSTuino::recvFind(String target, uint32_t timeout, uint8_t reserve /* =8 */)
{
    String data_tmp((char *)0);
    data_tmp.reserve(reserve);
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        return true;
    }
    return false;
}

/*!
 * @brief Writes a command from PROGMEM to the ESP8266's serial ports
 *
 * @param text The constant from PROGMEM to write to the ESP8266 module
 */
void SSTuino::writeCommandFromPROGMEM(const char* text, int buffersize /* =8 */) {
    char buf[buffersize] = {'\0'};           // WARNING: THIS BUFFER ONLY GOES UP TO 8 CHARS!
    strcpy_P(buf, (char *) text);
    _ESP01UART.print(buf);
}

/*!
 * @brief Waits for a certain serial input. This also prints out the serial data before hitting the correct case
 * if debug mode is enabled.
 *
 * @param values Semicolon delimited expected C-style string
 * @param timeOut Timeout in milliseconds
 * @return -1 if timed out, 0, 1, 2, ... if the data matches one of the values in the values string
 */
int16_t SSTuino::wait(const char* values, uint16_t timeOut) {
    if(!values)
        return -1;
    uint16_t length = strlen(values);
    char InputBuffer[length + 1];
    strcpy(InputBuffer, values);
    char CompareBuffer[length + 1];
    memset(CompareBuffer, 0, sizeof(CompareBuffer));
    uint16_t tokenQuantity = 1;
    for (uint16_t n = 0; n < length; n++) {
        if (InputBuffer[n] == ';')
            tokenQuantity++;
    }
    char* inputTokens[tokenQuantity];
    memset(inputTokens, 0, sizeof(inputTokens));
    char* compareTokens[tokenQuantity];
    memset(compareTokens, 0, sizeof(compareTokens));
    inputTokens[0] = InputBuffer;
    compareTokens[0] = CompareBuffer;
    uint16_t TokenPosition = 1;
    for (uint16_t n = 0; n < length; n++) {
        if (InputBuffer[n] == ';') {
            InputBuffer[n] = 0;
            inputTokens[TokenPosition] = &InputBuffer[n + 1];
            compareTokens[TokenPosition] = &CompareBuffer[n + 1];
            TokenPosition++;
        }
    }
    uint32_t timer = millis();
    char c;
    while (millis() - timer < timeOut) {
        while (_ESP01UART.available()) {
            c = _ESP01UART.read();
            for (uint16_t n = 0; n < tokenQuantity; n++) {
                length = strlen(compareTokens[n]);
                if (c == inputTokens[n][length])
                    compareTokens[n][length] = c;
                else if (length > 0)
                    memset(compareTokens[n], 0, length);
                if (!strcmp(inputTokens[n], compareTokens[n]))
                    return n;
            }
        }
    }
    return -1;
}
