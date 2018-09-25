#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

#define DHTPIN D5
#define DHTTYPE  DHT11

// defines pins numbers
const int trigPin = 5;  //D1
const int echoPin = 4;  //D2

// defines variables
long duration;
int distance;


const char* ssid        = "ICE_Innovation_lab";
const char* password    = "bolajabena15";
const char* host        = "api.thingspeak.com";
const char* writeAPIKey = "2YWYOEJGBA44YVM7";

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {

  //Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  
  dht.begin();
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop() {

 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();

 digitalWrite(trigPin, LOW); 
 delay(200); 
 digitalWrite(trigPin, HIGH); 
 delay(100); 
 digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034/2;

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
  url+="&field3=";
  url+=String(distance);
  
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
  
  Serial.print("Distance is : "); 
  Serial.print(distance);
  Serial.print("cm"); 
  Serial.println();
  
  delay(1000);  
  
}
