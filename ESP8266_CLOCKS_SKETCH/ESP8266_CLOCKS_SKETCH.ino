// *** If ESP8266 can`t find WiFi, you can edit SSID & pass using serial port: in serial monitor enter new SSID, press "Enter", and enter new pass. ESP8266 remember new data

#include <ESP8266_SEGMENT_LED_SSD1306_SW_I2C.h>

// WiFi & NTP trash
#include <EEPROM.h>
#include <NTPClient.h> // https://github.com/FWeinb
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h> 


struct WIFI_DATA 
{
  char writeBuffer_SSID[256] = "";
  char writeBuffer_PASSWORD[256] = "";
};
WIFI_DATA DATA_BUFFER;

String SSID_BUFFER;
String PASSWORD_BUFFER;
bool SSID_READ = false;
bool NEW_WIFI_DATA_AVAILABLE = false;


char ssid[256]     = "";
char password[256] = "";
bool badWiFi = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "1.ru.pool.ntp.org", 10800,60000);


SEGMENT_SSD1306 SEG_1000(2, 0);
SEGMENT_SSD1306 SEG_0100(4, 5);
SEGMENT_SSD1306 SEG_0010(14, 12);
SEGMENT_SSD1306 SEG_0001(-1, -1);


int WiFiAttempts = 0;
int WiFi_animation_dots = 0;

void WiFi_tryConnect() 
{
  EEPROM.get(0, DATA_BUFFER);  
  Serial.print("Current SSID: ");  
  Serial.println(DATA_BUFFER.writeBuffer_SSID);  
  Serial.print("Current PASSWORD: ");  
  Serial.println(DATA_BUFFER.writeBuffer_PASSWORD);
  
  WiFi.begin(DATA_BUFFER.writeBuffer_SSID, DATA_BUFFER.writeBuffer_PASSWORD);
  while ( WiFi.status() != WL_CONNECTED && WiFiAttempts < 20) // WiFiAttemps >= 20 - bad WiFi. Use RTC
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

  if (WiFiAttempts >= 20)
  {
    badWiFi = true;
    SEG_1000.show_F();
    SEG_0100.show_A();
    SEG_0010.show_I();
    SEG_0001.show_L();
  }    
  else 
  {
    timeClient.begin();
    timeClient.update();     
  }   
}

void setup() 
{  
  Serial.begin(115200); 
  EEPROM.begin(512);

  Serial.println("");
  Serial.println(">>> START SERIAL <<<");  
  
  SEG_1000.begin(); 
  SEG_0100.begin();    
  SEG_0010.begin();  

  SEG_0001.setSDA(3);
  SEG_0001.setSCL(1);
  SEG_0001.begin();  

  WiFi_tryConnect();        
}



void updateTime(String timeVar) 
{
  SEG_1000.showDigit(getDigit(timeVar[0]));
  SEG_0100.showDigit(getDigit(timeVar[1]));
  SEG_0010.showDigit(getDigit(timeVar[3]));
  SEG_0001.showDigit(getDigit(timeVar[4])); 
}
int getDigit(char symbol) { return symbol-'0'; }

bool sendMessage_badWifi = false;
void loop() 
{
  if (!badWiFi) 
  {
    updateTime(timeClient.getFormattedTime());
    delay(1000);
  }
  else 
  {    
    // READ NEW SSID & PASSWORD
    if (!sendMessage_badWifi)
    {
      Serial.println("");
      Serial.println("Bad WiFi! Enter new SSID and then enter new PASSWORD:");    
      sendMessage_badWifi = true;
    }

    if (Serial.available()) 
    {
      if (!SSID_READ) 
      {
        SSID_BUFFER = Serial.readString();
        SSID_BUFFER.remove(SSID_BUFFER.length()-1);
        SSID_READ = true;
      }
      else 
      {
        PASSWORD_BUFFER = Serial.readString();    
        PASSWORD_BUFFER.remove(PASSWORD_BUFFER.length()-1);
        Serial.println(""); 
         
        NEW_WIFI_DATA_AVAILABLE = true;
        SSID_READ = false;            
      }    
    }    
  }


  if (NEW_WIFI_DATA_AVAILABLE) 
  {
    Serial.println("REWRTIE WIFI DATA...");
      
    SSID_BUFFER.toCharArray(DATA_BUFFER.writeBuffer_SSID, SSID_BUFFER.length()+1);
    PASSWORD_BUFFER.toCharArray(DATA_BUFFER.writeBuffer_PASSWORD, PASSWORD_BUFFER.length()+1);
    
    EEPROM.put(0, DATA_BUFFER);  
    EEPROM.commit();

    EEPROM.get(0, DATA_BUFFER);  
    Serial.println(DATA_BUFFER.writeBuffer_SSID);   
    Serial.println(DATA_BUFFER.writeBuffer_PASSWORD);

    badWiFi = false;
    WiFiAttempts = 0;
    WiFi_tryConnect();

   
    NEW_WIFI_DATA_AVAILABLE = false;      
  }  
}
