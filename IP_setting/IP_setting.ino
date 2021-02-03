#include <ESP8266WiFi.h>
 
const char* ssid = "Thanks God"; // Isi dengan nama wifi anad
const char* password = "godbless@"; //wifi password nya
WiFiServer server(80);
 
#define pin_relay 0 // pin relaya pada GPIO0
 
IPAddress local_IP(192, 168, 100, 184);
IPAddress gateway(192, 168, 100, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


void setup() 
{
  Serial.begin(115200); //baud komunikasi
 
  pinMode(pin_relay,OUTPUT); //didefinisikan sebagai output
  digitalWrite(pin_relay, LOW); //nilai awalnya low = tidak aktif
 
  //Memulai menghubungkan ke wifi
  Serial.println();
  Serial.println();
   if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("Menghubungkan dengan wifi...");
  Serial.println(ssid);
 
 //prosedur koneksi ke wifi yang dijadikan target dan password
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("."); //..........
  }
  Serial.println("");
  Serial.println("terhubung ke WiFi Anda");
  server.begin();
  Serial.println("Siap digunakan");
 
  //menampilkan alamat IP address
  Serial.print("Gunakan IP ini sebagai kendali Relay : ");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
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
    digitalWrite(pin_relay,LOW);
    value = LOW;
  }
  if (request.indexOf("/pin_relay=OFF") != -1)  
  {
    Serial.println("Kondisi_Relay=OFF");
    digitalWrite(pin_relay,HIGH);
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
