#include "coreiot.h"

// Default fallbacks (overridden by saved config in globals when available)
const char* DEFAULT_COREIOT_SERVER = "app.coreiot.io";
const int DEFAULT_MQTT_PORT = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect (username=token, password=empty)
    //if (client.connect("ESP32Client", coreIOT_Token, NULL)) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Use configured token (CORE_IOT_TOKEN) as username if available
    const char *token = NULL;
    if (!CORE_IOT_TOKEN.isEmpty()) token = CORE_IOT_TOKEN.c_str();

    if (token != NULL) {
      if (client.connect(clientId.c_str(), token, NULL)) {
        Serial.println("connected to CoreIOT Server!");
        client.subscribe("v1/devices/me/rpc/request/+");
        Serial.println("Subscribed to v1/devices/me/rpc/request/+");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    } else {
      // no token configured, try anonymous connect
      if (client.connect(clientId.c_str())) {
        
        Serial.println("connected to CoreIOT Server (no-token)!");
        client.subscribe("v1/devices/me/rpc/request/+");
        Serial.println("Subscribed to v1/devices/me/rpc/request/+");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  // Allocate a temporary buffer for the message
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.print("Payload: ");
  Serial.println(message);

  // Parse JSON
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* method = doc["method"];
  if (strcmp(method, "setStateLED") == 0) {
    // Check params type (could be boolean, int, or string according to your RPC)
    // Example: {"method": "setValueLED", "params": "ON"}
    const char* params = doc["params"];

    if (strcmp(params, "ON") == 0) {
      Serial.println("Device turned ON.");
      //TODO

    } else {   
      Serial.println("Device turned OFF.");
      //TODO

    }
  } else {
    Serial.print("Unknown method: ");
    Serial.println(method);
  }
}


void setup_coreiot(){

  //Serial.print("Connecting to WiFi...");
  //WiFi.begin(wifi_ssid, wifi_password);
  //while (WiFi.status() != WL_CONNECTED) {
  
  // while (isWifiConnected == false) {
  //   delay(500);
  //   Serial.print(".");
  // }

  while(1){
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY)) {
      break;
    }
    delay(500);
    Serial.print(".");
  }


  Serial.println(" Connected!");

  // configure MQTT server and port (use saved config if available)
  const char *server = (CORE_IOT_SERVER.isEmpty() ? DEFAULT_COREIOT_SERVER : CORE_IOT_SERVER.c_str());
  uint16_t port = (CORE_IOT_PORT.isEmpty() ? DEFAULT_MQTT_PORT : (uint16_t)CORE_IOT_PORT.toInt());
  client.setServer(server, port);
  client.setCallback(callback);

}

void coreiot_task(void *pvParameters){

    setup_coreiot();

    while(1){

        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        // Sample payload, publish to 'v1/devices/me/telemetry'
        String payload = "{\"temperature\":" + String(glob_temperature) +  ",\"humidity\":" + String(glob_humidity) + "}";
        
        client.publish("v1/devices/me/telemetry", payload.c_str());


        
        Serial.println("Published payload: " + payload);
        vTaskDelay(10000);  // Publish every 10 seconds
    }
}