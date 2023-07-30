#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include "ESP8266WiFi.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);  

char myssid[] = "Network SSID"; 
char mypass[] = "Network password";        

SoftwareSerial ss(12, 13);

const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "API Key";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

double latitude    = 0.0;
double longitude   = 0.0;
double accuracy    = 0.0;
int more_text = 1; 

double f_lat=0, f_long=0, f_dist=0;

static double haversine(double lat1, double lon1, double lat2, double lon2)
  {
    double dLat = (lat2 - lat1) * 3.14 / 180.0;
    double dLon = (lon2 - lon1) * 3.14 / 180.0;

    // convert to radians
    lat1 = (lat1) * 3.14 / 180.0;
    lat2 = (lat2) * 3.14 / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * atan2(sqrt(a),sqrt(1-a));
    return (rad * c);

  }


void setup()   {
  Wire.begin(D2,D1);
  lcd.begin(16, 2);   // initializing the LCD
  lcd.home();
  lcd.backlight();
  Serial.begin(115200);
  Serial.println("Start");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  lcd.print("Setup done");
  delay(10);
  lcd.clear();
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.begin(myssid, mypass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  lcd.print("WiFi connected");
  delay(10);
  lcd.clear();

}

void loop() {
  char bssid[6];
  DynamicJsonDocument doc(1024);
  Serial.println("Scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0)
    Serial.println("No networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found...");

    if (more_text) {
      // Print out the formatted json
      Serial.println("{");
      Serial.println("\"homeMobileCountryCode\": 234,");  // this is a real UK MCC
      Serial.println("\"homeMobileNetworkCode\": 27,");   // and a real UK MNC
      Serial.println("\"radioType\": \"gsm\",");          // for gsm
      Serial.println("\"carrier\": \"Vodafone\",");       // associated with Vodafone
      Serial.println("\"wifiAccessPoints\": [");
      for (int i = 0; i < n; ++i)
      {
        Serial.println("{");
        Serial.print("\"macAddress\" : \"");
        Serial.print(WiFi.BSSIDstr(i));
        Serial.println("\",");
        Serial.print("\"signalStrength\": ");
        Serial.println(WiFi.RSSI(i));
        if (i < n - 1)
        {
          Serial.println("},");
        }
        else
        {
          Serial.println("}");
        }
      }
      Serial.println("]");
      Serial.println("}");
    }
    Serial.println(" ");
  }
  // now build the jsonString...
  jsonString = "{\n";
  jsonString += "\"homeMobileCountryCode\": 234,\n"; // this is a real UK MCC
  jsonString += "\"homeMobileNetworkCode\": 27,\n";  // and a real UK MNC
  jsonString += "\"radioType\": \"gsm\",\n";         // for gsm
  jsonString += "\"carrier\": \"Vodafone\",\n";      // associated with Vodafone
  jsonString += "\"wifiAccessPoints\": [\n";
  for (int j = 0; j < n; ++j)
  {
    jsonString += "{\n";
    jsonString += "\"macAddress\" : \"";
    jsonString += (WiFi.BSSIDstr(j));
    jsonString += "\",\n";
    jsonString += "\"signalStrength\": ";
    jsonString += WiFi.RSSI(j);
    jsonString += "\n";
    if (j < n - 1)
    {
      jsonString += "},\n";
    }
    else
    {
      jsonString += "}\n";
    }
  }
  jsonString += ("]\n");
  jsonString += ("}\n");
  Serial.println("");

  WiFiClientSecure client;

  //Connect to the client and make the api call
  Serial.println("Retreiving data");
  Serial.println("https://" + (String)Host + thisPage + "YOUR API KEY");
  Serial.println(" ");
  delay(500);
  client.setInsecure();

  if (client.connect(Host, 443)) {
    Serial.println("Connected to port");
    delay(100);
    lcd.clear();
    delay(10);
    lcd.print("Connected to port");

    client.println("POST " + thisPage + key + " HTTP/1.1");
    client.println("Host: " + (String)Host);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.print(jsonString);
    delay(500);
  }
  else { 
    lcd.print("Not connected to port");
    delay(10);
    lcd.clear();
    delay(1000);
  }

  //Read and parse all the lines of the reply from serve
  while (client.available()) {
  String line = client.readStringUntil('\r');
  if (more_text) {
    Serial.println(line);
  }

  deserializeJson(doc, line);
  lcd.clear(); 
  lcd.print("GPS info");
  delay(100);
  lcd.clear();
  // Retrieve the geolocation data from the parsed JSON
  latitude = doc["location"]["lat"];
  longitude = doc["location"]["lng"];
  accuracy = doc["accuracy"];
  // Serial.println("latitude,longitude");
  // Serial.println(latitude);
  // Serial.println(longitude);
  if (haversine(latitude, longitude, 13.0115720, 80.236330 )<100){  
      f_lat=latitude;
      f_long=longitude;
      f_dist=haversine(latitude, longitude, 13.0115720, 80.236330 );
    }
  }

  Serial.println("Latitude and longitude");
  Serial.println(f_lat);
  Serial.println(f_long);

  Serial.println("Accuracy : ");
  Serial.println(acc);

  Serial.println("Distance in metres : ");
  Serial.println(f_dist*1000);

  lcd.print(f_dist*1000);
  delay(100);
  lcd.clear();

  Serial.print("ETA in seconds : ");
  Serial.println((f_dist/20.0)*60*60);

  lcd.print(f_dist/25.0);
  delay(100);
  lcd.clear();

  Serial.println();

  delay(100);

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>Buggy tracking system</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER>Buggy tracking system</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude(buggy)</th>";
  s += "<td ALIGN=CENTER >";
  s += lati_str;
  s += "</td> </tr> <tr> <th>Longitude(buggy)</th> <td ALIGN=CENTER >";
  s += long_str;
  s += "</td> </tr> <tr>  <th>Latitude(user)</th> <td ALIGN=CENTER >";
  s += f_lat;
  s += "</td></tr> <tr> <th>Longitude(user)</th> <td ALIGN=CENTER >";
  s += f_long;
  s += "</td> </tr> <tr>  <th>Distance in metres</th> <td ALIGN=CENTER >";
  s += dist;
  s += "</td></tr> <tr> <th>ETA in minutes</th> <td ALIGN=CENTER >";
  s += eta/60;
  s += "</td>  </tr> </table> ";
  s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
  s += lati_str;
  s += "+";
  s += long_str;
  s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  s += "</body> </html> \n";

  client.print(s);
  Serial.println(WiFi.localIP());

  client.stop();
  lcd.clear(); 
}