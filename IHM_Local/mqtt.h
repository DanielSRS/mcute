#include <MQTTClient.h>
#include "sensor.h"
#ifndef MQTT_H
#define MQTT_H

#define MQTT_ADDRESS "192.168.1.2"
#define CLIENTID "98631145_c_client_paho_tcp"

#define USERNAME "aluno"
#define PASSWORD "@luno*123"

#define MQTT_PUBLISH_TOPIC "output"
#define MQTT_SUBSCRIBE_TOPIC "output"

struct mqtt_context {
  MQTTClient client;
  int verif;
  Sensor *analogic;
  Sensor *digitals;
  int *digitalQtd;
  MQTTClient_connectOptions *MQTT_CONFIG;
};

typedef struct mqtt_context ContextData;


void publish(MQTTClient client, char *topic, char *payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void on_disconnect(void *context, char *cause);
int configMqttClient(ContextData *data);

#endif