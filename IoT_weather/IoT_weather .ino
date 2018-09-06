/*________________________________________________________________
| Author : Akib Hosen Khan                                        |
|         Student of Information & Communication Engineering      |
| Hardware: ESP8266, DHT11                                        |
| IoT platform : ThingSpeak                                       |
|_________________________________________________________________*/
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

#define DHTPIN D5
#define DHTTYPE  DHT11

const char* ssid        = "your wifi ssid";
const char* password    = "your password";
const char* host        = "api.thingspeak.com";
const char* writeAPIKey = "your api";

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
}

void loop() {
 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperature);
  url+="&field2=";
  url+=String(humidity);
  url+="\r\n";

  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

    
    Serial.print("Temperature Value is :");  
  Serial.print(temperature);  
  Serial.println("C");  
  Serial.print("Humidity Value is :");  
  Serial.print(humidity);  
  Serial.println("%");  
    
    
    
    delay(5000);
}

