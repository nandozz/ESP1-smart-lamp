#include <ESP8266WiFi.h>

const char* ssid = "SMART IoT";      // Nama AP/Hotspot
const char* password = "123456789";    // Password AP/Hotspot

// Mengatur IP Address ----------------------------------------------------
IPAddress IP(192,168,100,111);
IPAddress NETWORK(192,168,100,1);
IPAddress NETMASK(255,255,255,0);
IPAddress DNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4); //optional
int LED = 2;  
  
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  delay(10);
  
 // Mengatur WiFi ----------------------------------------------------------
  Serial.println();
  Serial.print("Configuring access point...");
   
  WiFi.mode(WIFI_AP);             // Mode AP/Hotspot
  WiFi.softAP(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("."); //..........
  }
  
    if (!WiFi.config(IP, NETWORK, NETMASK, DNS,secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
 // Start the server -------------------------------------------------------
  server.begin();
  Serial.println("Server dijalankan");
  
 // Print the IP address ---------------------------------------------------
  Serial.println(WiFi.localIP());
 }

void loop() {
  //cek kondisi jika terhubung dengan module
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // menunggu sampai module relay programming kirim data
  Serial.println("Client Baru");
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Mencocokan permintaan awal
  int value = LOW;
  if (request.indexOf("/pin_relay=ON") != -1)  
  {
    Serial.println("Kondisi_Relay=ON");
    digitalWrite(LED,LOW);
    value = LOW;
  }
  if (request.indexOf("/pin_relay=OFF") != -1)  
  {
    Serial.println("Kondisi_Relay=OFF");
    digitalWrite(LED,HIGH);
    value = HIGH;
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP-01 IOT Kendali module Relay Programming</title></head>");
  client.print("Kondisi relay sekarang ; ");
 
  if(value == HIGH) 
  {
    client.print("MATI");
  } 
  else 
  {
    client.print("HIDUP");
  }
  client.println("<br><br>");
  client.println("Turn <a href=\"/pin_relay=OFF\">MATI</a> RELAY<br>");
  client.println("Turn <a href=\"/pin_relay=ON\"HIDUP</a> RELAY<br>");
    client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
