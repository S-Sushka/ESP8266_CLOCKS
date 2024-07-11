#include <ESP8266_SEGMENT_LED_SSD1306_SW_I2C.h>

// WiFi & NTP trash
#include <NTPClient.h> // https://github.com/FWeinb
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h> 

#define SSID_BUFFER "TP-Link_03EC"
#define PASSWORD_BUFFER "92261754"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "1.ru.pool.ntp.org", 10800,60000);

bool badWiFi = false;
int WiFiAttempts = 0;
const int WiFiAttemps_MAX = 30;
int WiFi_animation_dots = 0;

SEGMENT_SSD1306 SEG_1000(2, 0);
SEGMENT_SSD1306 SEG_0100(4, 5);
SEGMENT_SSD1306 SEG_0010(14, 12);
SEGMENT_SSD1306 SEG_0001(-1, -1);
void show_FAIL() {
  SEG_1000.show_F();
  SEG_0100.show_A();
  SEG_0010.show_I();
  SEG_0001.show_L(); }


void setup() 
{  
  SEG_1000.begin(); 
  SEG_0100.begin();    
  SEG_0010.begin();  

  SEG_0001.setSDA(3);
  SEG_0001.setSCL(1);
  SEG_0001.begin();  


  // >>> WiFi Try Connect        
  WiFi.begin(SSID_BUFFER, PASSWORD_BUFFER);
  while ( WiFi.status() != WL_CONNECTED && WiFiAttempts < WiFiAttemps_MAX) // WiFiAttemps >= WiFiAttemps_MAX - bad WiFi :(
  {    
    SEG_1000.show_WIFISearch(WiFi_animation_dots);
    SEG_0100.show_WIFISearch(WiFi_animation_dots);
    SEG_0010.show_WIFISearch(WiFi_animation_dots);
    SEG_0001.show_WIFISearch(WiFi_animation_dots);
    if (WiFi_animation_dots < 2)
      WiFi_animation_dots++;
    else
      WiFi_animation_dots = 0;
    
    WiFiAttempts++;
    delay(250);
  }    

  if (WiFiAttempts >= WiFiAttemps_MAX)
  {
    badWiFi = true;
    show_FAIL();
  }    
  else 
  {
    timeClient.begin();
    timeClient.update();     
  }  
}


void updateTime(String timeVar) 
{
  SEG_1000.showDigit(getDigit(timeVar[0]));
  SEG_0100.showDigit(getDigit(timeVar[1]));
  SEG_0010.showDigit(getDigit(timeVar[3]));
  SEG_0001.showDigit(getDigit(timeVar[4])); 
}
int getDigit(char symbol) { return symbol-'0'; }

void loop() 
{
  if (!badWiFi) 
  {
    updateTime(timeClient.getFormattedTime());
    delay(1000);

    if (WiFi.status() != WL_CONNECTED) 
    {
      badWiFi = true;
      show_FAIL(); 
    }    
  } 
}
