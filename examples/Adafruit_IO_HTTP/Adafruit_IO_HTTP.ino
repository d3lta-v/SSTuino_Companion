
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
  Serial.println(F("Opening link to Wi-fi chip..."));
  wifi.openLink();

  // Verify that the link is ok between the two devices
  Serial.print(F("Verifying link ok..."));
  if (wifi.smokeTest()) Serial.println("ok");
  else {
    Serial.println(F("not ok. Halting.")); 
    while (true){};
  }

  // Setup random number generator (as dummy data)
  randomSeed(analogRead(0));

  connectToWifi();
}

void loop()
{
  randNumber = random(0, 255);
  transmitData(randNumber);
  delay(5000); // 5 second interval
}

void connectToWifi(void)
{
  // Connects to Wifi and displays connection state
  wifi.connectToWifi(SSID, PASSWORD);
  Serial.print(F("Connecting to wifi with SSID: "));
  Serial.print(SSID);
  Serial.print(F(", password: "));
  Serial.print(PASSWORD);
  Serial.println(F("..."));

  delay(10000); // 10 seconds optimal for wifi connection to fully establish

  Status wifiStatus = wifi.getWifiStatus();
  Serial.print(F("Wifi status: "));
  switch (wifiStatus)
  {
  case SUCCESSFUL:
    Serial.println(F("connected"));
    break;
  case UNSUCCESSFUL:
    Serial.println(F("failed to connect"));
    while (true){};
    break;
  case IN_PROGRESS:
    Serial.println(F("connecting"));
    while (true){};
    break;
  case NOT_ATTEMPTED:
    Serial.println(F("not connected"));
    while (true){};
    break;
  }
}

void transmitData(int value)
{
  Serial.println(F("Transmitting data to Adafruit IO via HTTP..."));
  int handle = wifi.setupHTTP(POST, F("https://io.adafruit.com/api/v2/" IO_USERNAME "/feeds/" FEED_KEY "/data"));
  if (handle == -1) {
    Serial.println(F("Unable to setup HTTP connection!"));
    while (true){};
  }

  // Set POST data
  if (!wifi.setHTTPPOSTParameters(handle, "value=" + value)) {
    Serial.println(F("HTTP unable to set POST parameters!"));
    while (true){};
  }

  // Set headers to contain the key
  if (!wifi.setHTTPHeaders(handle, "Content-Type: multipart/form-data\nX-AIO-Key: " IO_KEY "\n")) {
    Serial.println(F("HTTP unable to set headers!"));
    while (true){};
  }

  // Actually transmit the data
  if (!wifi.transmitHTTP(handle)) {
      Serial.println(F("Transmit failed!"));
      while (true){};
  }

  // Wait for a good reply
  delay(10000);
  Status http = wifi.getHTTPProgress(handle);
  if (http != SUCCESSFUL) {
    Serial.println(F("HTTP request did not succeed!"));
  } else {
    // Print out reply
    Serial.print(F("Got HTTP status code: "));
    Serial.println(wifi.getHTTPStatusCode(handle));
    Serial.print(F("Got HTTP reply: "));
    Serial.println(wifi.getHTTPReply(handle, CONTENT, false));
  }
  // Purge the previous HTTP data
  wifi.deleteHTTPReply(handle);
}
