# SSTuino Companion Library

This library is designed to be a companion to the SSTuino, offering a robust and easy to use Wi-Fi interface with an ESP8266 Wi-Fi board running the [ULWI](https://github.com/d3lta-v/ULWI) firmware.

![SSTuino](https://github.com/d3lta-v/SSTuino/raw/master/Image%20Assets/SSTuino.png)

This project is still in **BETA**, and things may change or break.

## Contributing

This repository uses Microsoft's Visual Studio Code, a cross platform text editor and IDE, alongside with the [Arduino CLI](https://github.com/arduino/arduino-cli)

## Quick Start

---

### Setup

To start the wifi chip instantiate the SSTuino class, open the link with the ESP01S Chip and reset the chip.

```cpp
SSTuino wifi = SSTuino();
void setup()
{
    wifi.openLink();
    wifi.reset();
}
```

Next, verify the link with the wifi chip.

```cpp
if (!wifi.smokeTest())
{
    Serial.println("Unable to establish link with Wi-Fi chip.");
}
```

### Connect to wifi

To connect to wifi, run the method `connectToWifi();` and check the wifi status after a delay with the `getWifiStatus();` method.

```cpp
void setup()
{
    //Previous code...
    wifi.connectToWifi("YOUR_SSID", "YOUR_WIFI_PASSPORD");
    delay(10000); //Delay for 10 seconds to allow the wifi chip to connect to wifi.
    Status wifiStatus = wifi.getWifiStatus();
    if (wifiStatus != SUCCESSFUL)
    {
        Serial.println("Failed to connect to Wi-Fi");
    }else
    {
        Serial.println("Wifi Connected");
    }
}
```

## HTTP

---

### HTTP Get

To perform a HTTP GET request, you first need to create a handle which is returned by the `setupHTTP();` method. Pass the `GET` parameter and the url of the api.

Then, check if the setup failed.

```cpp
void getHTTP(void)
{
    int handle  = wifi.setupHTTP(GET, "http://your-url-here.com");
    if (handle == -1)
    {
        Serial.println("Unable to setup HTTP");
        return;
    }
}
```

Next, transmit the HTTP Request and check if it failed

```cpp
void getHTTP(void)
{
    //Previous code...
    if (!wifi.transmitHTTP(handle))
    {
        Serial.print("HTTP Transmit failed");
        return;
    }
}
```

Next, check if the request succeeded and print the reply

```cpp
void getHTTP(void)
{
    //Previous code...
    delay(10000); //Allow a delay for the request to go through
    Status http = wifi.getHTTPProgress(handle);
    if (http != SUCCESSFUL)
    {
        Serial.println(F("HTTP request did not succeed!"));
        return;
    }
    Serial.println("HTTP request succeeded");
    Serial.println(wifi.getHTTPReply(handle, CONTENT, true));
}
```

### HTTP Post

To perform a HTTP POST request, you first need to create a handle which is returned by the `setupHTTP();` method. Pass the `POST` parameter and the url of the api.

Then, check if the setup failed.

```cpp
void postHTTP(void)
{
    int handle  = wifi.setupHTTP(POST, "http://your-url-here.com");
    if (handle == -1)
    {
        Serial.println("Unable to setup HTTP");
        return;
    }
}
```

Next, set the headers and check if it fails.

```cpp
void postHTTP(void)
{
    //Previous code...
    if (!wifi.setHTTPHeaders(handle, "authorization: bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJvcmlnaW4iOiJzc3R1aW5vIiwic2VjcmV0LW1lc3NhZ2UiOiJpZiB5b3UgZm91bmQgdGhpcywgY29udGFjdCBtZS4gNjE3LCBQUiAjMSJ9.DaTTiGjJuiSlqdjvmnOkFj3ZTi617_cXZKDYVdH2ZJQ")) {
    Serial.println(F("HTTP unable to set headers! Skipping this connection attempt"));
    return;
  }
}
```

Next, set the post data (the body)

```cpp
    if (!wifi.setHTTPPOSTParameters(handle, "easter-egg=true")) {
    Serial.println(F("HTTP unable to set POST parameters! Skipping this connection attempt"));
    return;
  }
```

Next, transmit the HTTP Request and check if it failed

```cpp
void postHTTP(void)
{
    //Previous code...
    if (!wifi.transmitHTTP(handle))
    {
        Serial.print("HTTP Transmit failed");
        return;
    }
}
```

Next, check if the request succeeded and print the reply

```cpp
void postHTTP(void)
{
    //Previous code...
    delay(10000); //Allow a delay for the request to go through
    Status http = wifi.getHTTPProgress(handle);
    if (http != SUCCESSFUL)
    {
        Serial.println(F("HTTP request did not succeed!"));
        return;
    }
    Serial.println("HTTP request succeeded");
    Serial.println(wifi.getHTTPReply(handle, CONTENT, true));
}
```

## Reference

---

### Basic chip functions

<table>
    <tr>
        <td>Method</td>
        <td>Description</td>
        <td>Parameters</td>
        <td>Return</td>
    </tr>
    <tr>
        <td><code>smokeTest();</code</td>
        <td>A simple smoke test to see if the module responds</td>
        <td>None</td>
        <td>true if the test is successful and false if the test times out</td>
    </tr>
    <tr>
        <td><code>verifyVersion();</code</td>
        <td>Verifys the version of the Wi-Fi chip</td>
        <td>None</td>
        <td>Returns true if the version is correct and false if the version is not</td>
    </tr>
    <tr>
        <td><code>reset();</code</td>
        <td>Resets the Wi-Fi chip</td>
        <td>None</td>
        <td>None</td>
    </tr>
</table>
<br>
<br>

### Wifi functions

<table>
    <tr>
        <td>Method</td>
        <td>Description</td>
        <td>Parameters</td>
        <td>Return</td>
    </tr>
    <tr>
        <td><code>getWifiHotspots();</code</td>
        <td>Gets nearby wifi hotspots</td>
        <td>None</td>
        <td>Returns a string of nearby hotspots</td>
    </tr>
    <tr>
        <td><code>wifiInRange();</code</td>
        <td>Verifys the version of the Wi-Fi chip</td>
        <td><code>(String) ssid</code></td>
        <td>Returns true if the wifi is in range and false if it is not</td>
    </tr>
    <tr>
        <td><code>connectToWifi();</code</td>
        <td>Connects to Wi-Fi</td>
        <td><code>(String) ssid, (String) password</code></td>
        <td>None</td>
    </tr>
    <tr>
        <td><code>getWifiStatus();</code</td>
        <td>Returns the wifi status</td>
        <td>None</td>
        <td>Returns a <a href="#status-enum">Status</a></td>
    </tr>
    <tr>
        <td><code>disconnectWifi();</code</td>
        <td>Disconnects from wifi</td>
        <td>None</td>
        <td>None</td>
    </tr>
</table>
<br>
<br>

### Network functions

<table>
    <tr>
        <td>Method</td>
        <td>Description</td>
        <td>Parameters</td>
        <td>Return</td>
    </tr>
    <tr>
        <td><code>getIP();</code</td>
        <td>Gets current IP adress</td>
        <td>None</td>
        <td>Returns a string the current IP adress</td>
    </tr>
</table>
<br>
<br>

### Status Enum

<table>
    <tr>
        <td>Type</td>
        <td>Number</td>
    </tr>
    <tr>
        <td>UNRESPONSIVE</td>
        <td>-1</td>
    </tr>
    <tr>
        <td>SUCCESSFUL</td>
        <td>0</td>
    </tr>
    <tr>
        <td>UNSUCCESSFUL</td>
        <td>1</td>
    </tr>
    <tr>
        <td>IN_PROGRESS</td>
        <td>2</td>
    </tr>
    <tr>
        <td>NOT_ATTEMPTED</td>
        <td>3</td>
    </tr>
</table>
