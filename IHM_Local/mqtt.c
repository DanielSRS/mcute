#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>
#include "mqtt.h"

void publish(MQTTClient client, char *topic, char *payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void on_disconnect(void *context, char *cause);

int configMqttClient(ContextData *data) {
    printf("entra nas config\n");
    /*Configuração do Cliente*/
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    printf("criado obj\n");
    printf("client antes da config: %p\n", data->client);

    /* Inicializacao do MQTT (conexao & subscribe) */
    MQTTClient_create(&data->client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    printf("Cliente depois da config: %p\n", data->client);
    MQTTClient_setCallbacks(data->client, &data, on_disconnect, on_message, NULL);

    printf("Setando os callbacks em: %p\n", data->client);

    return MQTTClient_connect(data->client, &conn_opts);
}

void publish(MQTTClient client, char *topic, char *payload)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    ContextData *data = context;
    char *payload = message->payload;

    /* Mostra a mensagem recebida */
    printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName, payload);

    /* Faz echo da mensagem recebida */
    //publish(data->client, MQTT_PUBLISH_TOPIC, payload);

    MQTTClient_freeMessage(&message);
    //MQTTClient_free(topicName);
    return 1;
}

void on_disconnect(void *context, char *cause) {
  printf("\n\nDesconectado!!\n");
  printf("Causa: %s\n\n\n", cause);

  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  conn_opts.username = USERNAME;
  conn_opts.password = PASSWORD;

  ContextData *data = context;

  printf("Client mqtt: %p\n", data->client);

  int rc = MQTTClient_connect(data->client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS)
    {
        printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
        exit(-1);
    } else {
        printf("Reconectado\n");
    }

    MQTTClient_subscribe(data->client, MQTT_SUBSCRIBE_TOPIC, 0);
};
