#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define LED D0

WiFiUDP Udp;
#define UDPPORT 11011
#define UDPMULTICASTIP IPAddress(239,0,0,10)

bool startWPSPBC();

void setup() {
  Serial.begin(9600);
  delay(1000);
  Udp.begin(UDPPORT);
  Serial.printf("\nTry connecting to WiFi with SSID '%s'\n", WiFi.SSID().c_str());

  WiFi.mode(WIFI_STA);
  // WiFi.begin(WiFi.SSID().c_str(),WiFi.psk().c_str()); // reading data from EPROM, last saved credentials
  WiFi.begin("KOTIKI_2G", "truck311");
  while (WiFi.status() == WL_DISCONNECTED) {
    delay(500);
    Serial.print(".");
  }

  wl_status_t status = WiFi.status();
  if(status == WL_CONNECTED) {
    Serial.printf("\nConnected successfull to SSID '%s'\n", WiFi.SSID().c_str());
  } else {
    Serial.printf("\nCould not connect to WiFi. state='%d'", status); 
    Serial.println("Please press WPS button on your router.\n Press any key to continue...");
    while(!Serial.available()) { ; }
    if(!startWPSPBC()) {
       Serial.println("Failed to connect with WPS :-(");  
    }
  } 
}

bool startWPSPBC() {
  Serial.println("WPS config start");
  bool wpsSuccess = WiFi.beginWPSConfig();
  if(wpsSuccess) {
      // Well this means not always success :-/ in case of a timeout we have an empty ssid
      String newSSID = WiFi.SSID();
      if(newSSID.length() > 0) {
        // WPSConfig has already connected in STA mode successfully to the new station. 
        Serial.printf("WPS finished. Connected successfull to SSID '%s' '%s'\n", newSSID.c_str(), WiFi.psk().c_str());
      } else {
        wpsSuccess = false;
      }
  }
  return wpsSuccess; 
}

void loop() {
  yield();
  digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually 
                          //the LED is on; this is because it is acive low on the ESP8266.
  delay(1000);            // wait for 1 second.
  yield();
  digitalWrite(LED, LOW); // turn the LED on.
  delay(1000); // wait for 1 second.
  yield();


  WiFi.localIP().printTo(Serial);
  Serial.printf(" - %d(%d) - %s - ", int(WiFi.status()), WiFi.isConnected(), WiFi.SSID().c_str());
  Serial.println(WiFi.macAddress());

  if( WiFi.status() == wl_status_t::WL_CONNECTED ) {
    Serial.println("Sending UDP...");
    Udp.beginPacket("192.168.88.150", 2115);
    Udp.print("Hello World!");
    Udp.endPacket();
    delay(1000);
  }
}
