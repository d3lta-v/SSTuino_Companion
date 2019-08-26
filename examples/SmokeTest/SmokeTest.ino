/**
 * @example ConnectWiFi.ino
 * @brief The ConnectWiFi demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.03
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "SSTuino_Companion.h"

#define SSID        F("SSID_123")
#define PASSWORD    F("12345678")

SSTuino wifi = SSTuino();

void setup(void)
{
  Serial.begin(9600);

  Serial.println(F("Opening link to Wi-fi chip..."));

  wifi.openLink();

  Serial.print(F("Verifying link ok..."));

  if (wifi.smokeTest()) Serial.println("ok");
  else Serial.println("not ok");

  Serial.print(F("FW Version: "));
  Serial.println(wifi.getVersion().c_str());

  Serial.println("setup end");
}

void loop(void)
{
}
