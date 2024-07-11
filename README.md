# WiFi clocks based ESP8266

![Image alt](https://github.com/S-Sushka/ESP8266_CLOCKS/blob/main/CLOCKS.jpg)
> [!NOTE]
> This project use library NTPClient by Fabrice Weinberg: https://github.com/FWeinb

## How to use?
#### 1) Open ESP8266_CLOCKS_SKETCH.ino
#### 2) Add "ESP8266_SEGMENT_LED_SSD1306_SW_I2C" library
Sketch > Include Library > Add .ZIP Library, then select `ESP8266_SEGMENT_LED_SSD1306_SW_I2C.zip`
#### 3) Add "NTPClient" library
Sketch > Include Library > Add .ZIP Library, then select `NTPClient_v321.zip`  
OR  
Sketch > Include Library > Manage libraries. Search `NTPClient` by `Fabrice Weinberg`
#### 4) Edit `SSID_BUFFER` & `PASSWORD_BUFFER` according to your SSID & password. (SSID - is name of WiFi network)
#### 5) Upload sketch to ESP8266!
