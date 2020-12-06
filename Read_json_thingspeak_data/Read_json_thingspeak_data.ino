/*  Preference Ctrl + comma > http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  Setting board to ESP8266 setting > board
 *  add library  arduinojson version 5.13
 * 
 *  ESP8266 JSON Decode of server response
 *  -Manoj R. Thkuar
 *  https://circuits4you.com
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/////////////////////////////////////////////// EDIT PROFILE //////////////////////////////////////////
const char* wifiName = "";
const char* wifiPass = "";

//Web Server address to read/write from 
String API = "B9ABSKETD0062LXG";
String ID = "803833";
String Field = "2";
//////////////////////////////////////////////////////////////////////////////////////////////////////

//const char *host = "http://api.thingspeak.com/channels/803833/fields/2/last.json";
String host = "http://api.thingspeak.com/channels/"+ID+"/fields/"+Field+"/last.json";
String wri = "http://api.thingspeak.com/update?api_key="+API+"&field"+Field+"=0";
String feed = "https://api.thingspeak.com/channels/"+ID+"/feeds.json";
const int output2 = 0;

void setup() {
  
  Serial.begin(115200);
  pinMode(output2, OUTPUT);
  digitalWrite(output2, HIGH);
  delay(100);
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(wifiName);

  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
}

void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  Serial.print("\nRequest Link:");
  Serial.println(host);
  Serial.println(wri);
  Serial.println(feed);
  http.begin(host);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("\nResponse Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload
  
  if(httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);
  
   // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
  
    // Decode JSON/Extract values

////////////////////////
     Serial.println(F("Response:"));
      int field = root["field"+Field]; // 1
    /*const char* Time = root["time"]; // "Leanne Graham"
      const char* username = root["data"][0]; // "Bret"
      const char* email = root["data"][1]; // "Sincere@april.b  */
     Serial.println("Field "+Field+" : "+String(field));
     if(field==1){
      digitalWrite(output2, LOW);
      Serial.println("Lamp ON");
     }
     else {
      digitalWrite(output2, HIGH);
      Serial.println("Lamp OFF");
     }
     

/*////////////////////
    Serial.println(F("Response:"));
    Serial.println(root["sensor"].as<char*>());
    Serial.println(root["time"].as<char*>());
    Serial.println(root["data"][0].as<char*>());
    Serial.println(root["data"][1].as<char*>());   */
  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
  Serial.println("Close connection");
  delay(5000);  //GET Data at every 5 seconds
}
