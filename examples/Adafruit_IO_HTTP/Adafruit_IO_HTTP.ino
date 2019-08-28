
#include "SSTuino_Companion.h"

#define SSID          F("SSID GOES HERE")
#define PASSWORD      F("WIFI PASSWORD GOES HERE")
#define IO_USERNAME   "AIO USERNAME GOES HERE"
#define IO_KEY        "AIO KEY GOES HERE"
#define FEED_KEY      "FEED KEY GOES HERE"

SSTuino wifi = SSTuino();

int randNumber;

void setup()
{
  Serial.begin(9600);

  // Open the link between the two devices
  wifi.openLink();

  // Verify that the link is ok between the two devices
  if (!wifi.smokeTest()) {
    Serial.println(F("Unable to establish link with Wi-Fi chip. Halting.")); 
    while (true){};
  }

  // Setup random number generator (as dummy data)
  randomSeed(analogRead(0));

  connectToWifi();
}

void loop()
{
  randNumber = (int)random(0, 255);
  Serial.print("Random number picked: ");
  Serial.println(randNumber);
  transmitData(randNumber);
  delay(5000); // 5 second interval
}

void connectToWifi(void)
{
  // Connects to Wifi and displays connection state
  wifi.connectToWifi(SSID, PASSWORD);
  Serial.println(F("Connecting to Wi-Fi..."));

  delay(10000); // 10 seconds optimal for wifi connection to fully establish

  Status wifiStatus = wifi.getWifiStatus();
  if (wifiStatus != SUCCESSFUL) {
    Serial.println(F("Failed to connect to Wi-Fi"));
    while (true){};
  }
}

void transmitData(int value)
{
  // Setup the connection
  int handle = wifi.setupHTTP(POST, F("https://io.adafruit.com/api/v2/" IO_USERNAME "/feeds/" FEED_KEY "/data"));
  if (handle == -1) {
    Serial.println(F("Unable to setup HTTP connection!"));
    while (true){};
  }

  // Set POST data
  String combinedString = "value=";
  combinedString += value;
  if (!wifi.setHTTPPOSTParameters(handle, combinedString)) {
    Serial.println(F("HTTP unable to set POST parameters!"));
    while (true){};
  }

  // Set headers to contain the key
  if (!wifi.setHTTPHeaders(handle, "Content-Type: multipart/form-data\nX-AIO-Key: " IO_KEY "\n")) {
    Serial.println(F("HTTP unable to set headers!"));
    while (true){};
  }

  // Actually transmit the data
  Serial.println(F("Transmitting data to Adafruit IO via HTTP..."));
  if (!wifi.transmitHTTP(handle)) {
      Serial.println(F("Transmit failed!"));
      while (true){};
  }

  delay(10000);

  // Check if the message was received properly
  Status http = wifi.getHTTPProgress(handle);
  if (http != SUCCESSFUL) {
    Serial.println(F("HTTP request did not succeed!"));
  }
  // Print out the reply in full
  Serial.print(F("Got HTTP status code: "));
  Serial.println(wifi.getHTTPStatusCode(handle));
  Serial.print(F("Got HTTP reply: "));
  Serial.println(wifi.getHTTPReply(handle, CONTENT, false));
  // Purge the previous HTTP data
  wifi.deleteHTTPReply(handle);
}
