#include "MQTT.h"
#include "HC_SR04.h"
const int LED_PIN = A5;
const int trigPin = D6;
const int echoPin = D2;
double cm = 0.0;

HC_SR04 rangefinder = HC_SR04(trigPin, echoPin);
void callback(char* topic, byte* payload, unsigned int length);
MQTT client("broker.mqttdashboard.com", 1883, callback, true); //true = SYSTEM_THREAD(ENABLED);

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    Particle.publish("Message", String(p));
    delay(1000);
}


void setup() {
    WiFi.on();
    WiFi.setCredentials("SSID", "WPA PASS");
    WiFi.connect();
    waitUntil(WiFi.ready);
    while (WiFi.localIP() == IPAddress()) {
    	delay(10);
    }
    
    pinMode(LED_PIN, OUTPUT);
    
    // connect to the server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {
        
        client.publish("SIT210/wave","hello world");
        client.subscribe("SIT210/wave");
    }
}

void loop() 
{
    cm = rangefinder.getDistanceCM();
    if (client.isConnected())
    {
        if (cm > 0)
        {
            client.publish("SIT210/wave", "Martin");
        }
        client.loop();
    }
    delay(1000);
}
