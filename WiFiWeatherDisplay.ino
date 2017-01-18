#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <ArduinoJson.h>

#define D0 3 // GPIO3 maps to Ardiuno D0
#define D1 1 // GPIO1 maps to Ardiuno D1
#define D2 16 // GPIO16 maps to Ardiuno D2
#define D3 5 // GPIO5 maps to Ardiuno D3
#define D4 4 // GPIO4 maps to Ardiuno D4
#define D5 14 // GPIO14 maps to Ardiuno D5
#define D6 12 // GPIO12 maps to Ardiuno D6
#define D7 13 // GPIO13 maps to Ardiuno D7
#define D8 0 // GPIO0 maps to Ardiuno D8
#define D9 2 // GPIO2 maps to Ardiuno D9
#define D10 15 // GPIO15 maps to Ardiuno D10

const char* ssid     = "PAKTURK ISLAMPURA";      // SSID of local network
const char* password = "Pakturkislampura2004";   // Password on network
//const char* ssid     = "BaRiS";      // SSID of local network
//const char* password = "barisailesi";   // Password on network

String APIKEY = "1b1ac338bbb911f1d248027ccca4a67e";
String CityID = "306571"; //KONYA, TURKEY
//String CityID = "1172451"; // LAHORE PAKISTAN



WiFiClient client;
char servername[]="api.openweathermap.org";  // remote server we will connect to
String result;

int  counter = 60;

String weatherDescription ="";
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;


void setup() {


  Serial.begin(115200);
  int cursorPosition=0;
    lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("   HAVA ISTASYONU");
   lcd.setCursor(2,1);
  lcd.print("www.arduinom.org");
  lcd.setCursor(5,2);
  lcd.print("BAGLANIYOR");  
  Serial.println("BAGLANIYOR");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(cursorPosition,3); 
    lcd.print(".");
    cursorPosition++;
  }
  lcd.clear();
   lcd.print("   HAVA ISTASYONU");
   lcd.setCursor(2,1);
  lcd.print("www.arduinom.org");
  lcd.setCursor(5,2);
  lcd.print(" BAGLANDI!");
  Serial.println("BAGLANDI");
  delay(1000);

}

void loop() {
    if(counter == 60) //Get new data every 10 minutes
    {
      counter = 0;
      displayGettingData();
      delay(1000);
      getWeatherData();
    }else
    {
      counter++;
      displayWeather(weatherLocation,weatherDescription);
      delay(5000);
      displayConditions(Temperature,Humidity,Pressure);
      delay(5000);
    }
}

void getWeatherData() //client function to send/receive GET request data.
{
  if (client.connect(servername, 80)) {  //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
      result = result+c;
    }

  client.stop(); //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);

char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';

StaticJsonBuffer<1024> json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);
if (!root.success())
{
  Serial.println("parseObject() failed");
}

String location = root["name"];
String country = root["sys"]["country"];
float temperature = root["main"]["temp"];
float humidity = root["main"]["humidity"];
String weather = root["weather"]["main"];
String description = root["weather"]["description"];
float pressure = root["main"]["pressure"];

weatherDescription = description;
weatherLocation = location;
Country = country;
Temperature = temperature;
Humidity = humidity;
Pressure = pressure;

}

void displayWeather(String location,String description)
{
  lcd.clear();
  lcd.print("    HAVA ISTASYONU");
   lcd.setCursor(2,1);
//  lcd.print("www.arduinom.org");
  lcd.setCursor(5,2);
  lcd.print(location);
  lcd.print(", ");
  lcd.print(Country);
  lcd.setCursor(3,3);
  lcd.print(description);
}

void displayConditions(float Temperature,float Humidity, float Pressure)
{
 lcd.clear();
  lcd.print("    HAVA ISTASYONU");
   lcd.setCursor(0,1);
//  lcd.print("www.arduinom.org");
 lcd.print("    HAVA DURUMU"); 
   lcd.setCursor(2,2);
 
 lcd.print(Temperature,1);
 lcd.print((char)223);
 lcd.print("C ");
 
 //Printing Humidity
 lcd.print(" NEM:");
 lcd.print(Humidity,0);
 lcd.print(" %");
 
 //Printing Pressure
 lcd.setCursor(1,3);
 lcd.print("BASINC: ");
 lcd.print(Pressure,1);
 lcd.print(" hPa");

}

void displayGettingData()
{
  lcd.clear();
   lcd.print("   HAVA ISTASYONU");
   lcd.setCursor(2,1);
  lcd.print("www.arduinom.org");
  lcd.setCursor(0,3);
  lcd.print("HAVA DURUMU ALINIYOR");
}


