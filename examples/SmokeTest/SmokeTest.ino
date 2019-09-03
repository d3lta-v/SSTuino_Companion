/*
  Smoke Test

  An extremely simple test to verify that the SSTuino is communicating
  correctly with the ESP8266-01 Wi-Fi module.

  This example code is in the public domain.

  https://d3lta-v.github.io/SSTuino/
*/

#include "SSTuino_Companion.h"

SSTuino wifi = SSTuino();

void setup(void)
{
  Serial.begin(9600);

  Serial.println(F("Opening link to Wi-fi chip..."));

  wifi.openLink();

  Serial.print(F("Verifying link ok..."));

  if (wifi.smokeTest()) Serial.println(F("ok"));
  else Serial.println(F("not ok"));

  Serial.print(F("Checking if firmware version matches..."));
  if (wifi.verifyVersion()) Serial.println(F("yes"));
  else Serial.println(F("no"));

  Serial.println(F("setup end"));
}

void loop(void)
{
}
