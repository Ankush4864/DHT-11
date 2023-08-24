#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>  // ThingSpeak library

// Pin configuration for DHT sensor
#define DHTPIN 2       // Pin connected to DHT11 data pin
#define DHTTYPE DHT11  // Type of DHT sensor

// WiFi credentials
char ssid[] = "Ankush";       // WiFi SSID
char pass[] = "12345678";     // WiFi Password

// ThingSpeak configuration
unsigned long channelID = 2250524;         //  ThingSpeak Channel ID
const char *apiKey = "9XY19ABES1ZHPP9C";   //  ThingSpeak API Key

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

WiFiClient client;  // Initialize WiFi client

void setup()
{
  Serial.begin(9600);  // Initialize serial communication
  dht.begin();         // Initialize DHT sensor
  connectWiFi();       // Connect to WiFi
  ThingSpeak.begin(client);  // Initialize ThingSpeak communication
}

void loop()
{
  delay(2000);  // Wait for a few seconds between readings

  // Read temperature in Celsius and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if sensor readings are valid
  if (!isnan(temperature) && !isnan(humidity)) {
    sendDataToThingSpeak(temperature, humidity);  // Send data to ThingSpeak
  } else {
    Serial.println("Failed to read from DHT sensor!");  // Display error message
  }
  
  delay(30000); // Wait for 30 seconds before the next reading
}

// Function to connect to WiFi
void connectWiFi()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("Connected to WiFi");  // Display connection success message
}

// Function to send data to ThingSpeak
void sendDataToThingSpeak(float temp, float humidity)
{
  ThingSpeak.setField(1, temp);       // Set field 1 with temperature data
  ThingSpeak.setField(2, humidity);   // Set field 2 with humidity data
  
  int httpCode = ThingSpeak.writeFields(channelID, apiKey);  // Send data to ThingSpeak

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak");  // Display success message
  } else 
  {
    Serial.println("Error sending data to ThingSpeak");  // Display error message
  }
}
