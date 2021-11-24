#include "DHT.h"
#define DHTPIN 13     
#define DHTTYPE DHT11   
#include <WiFi.h>
#include <FirebaseESP32.h> 
FirebaseData fbdo;
FirebaseConfig config_data;
FirebaseAuth auth;
const char* ssid ="";
const char* password="";
#define FIREBASE_HOST "https://iotcam-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyBAIiMGuwp7X5y-gIGYWSEe4z-M2pAO3Yc"

DHT dht(33, DHT11);
int red_pin   = 19;
int green_pin = 21;
int buzzer_pin = 32;

void redlight (){
  Serial.println("ENGINE OVERHEAT, HOT START");
  digitalWrite(red_pin, HIGH);
  digitalWrite(buzzer_pin, HIGH);
}

void greenlight(){
  Serial.println("ENGINE TEMP FINE");
  digitalWrite(green_pin, HIGH);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config_data.host = FIREBASE_HOST;
  config_data.api_key = API_KEY;
  auth.user.email = "";
  auth.user.password = "";
  Firebase.begin(&config_data, &auth );
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 60000);
  pinMode (red_pin, OUTPUT);
  pinMode (green_pin, OUTPUT);
  pinMode (buzzer_pin, OUTPUT);
  digitalWrite(red_pin, LOW);
  digitalWrite(green_pin, LOW);
  digitalWrite(buzzer_pin, LOW);
  dht.begin();
}

void loop() {

  delay(2000);
  /*float t = random(25,50);*/
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  int t1= map(t, 0, 50, 500, 3000);
  if ( isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  else{
  Serial.println(F("ENGINE  TEMPERATURE: "));
  Serial.print(t1);
  Serial.print(F("°C "));
  Firebase.setFloat(fbdo, "/ENGINE TEMP", t1);
  
  
    if (t1>2000){
    redlight();
    delay(2000);
    }
    else {
    greenlight();
    delay(2000);
    }
  }
  digitalWrite(red_pin, LOW);
  digitalWrite(green_pin, LOW);
  digitalWrite(buzzer_pin, LOW);
}
