#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

#define TRUE 1
#define MQTT_ADDRESS "10.0.0.101"
#define CLIENTID "98631145_c_client_paho_tcp"

#define USERNAME "aluno"
#define PASSWORD "@luno*123"

#define MQTT_PUBLISH_TOPIC "/swd/"
#define MQTT_SUBSCRIBE_TOPIC "/swd/"

struct mqtt_context {
  MQTTClient client;
};

typedef struct mqtt_context ContextData;

void publish(MQTTClient client, char *topic, char *payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);

int v(int argc, char *argv[])
{
    int rc;

    ContextData data;

    /*Configuração do Cliente*/
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    /* Inicializacao do MQTT (conexao & subscribe) */
    MQTTClient_create(&data.client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(data.client, &data, NULL, on_message, NULL);

    rc = MQTTClient_connect(data.client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS)
    {
        printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
        exit(-1);
    }

    MQTTClient_subscribe(data.client, MQTT_SUBSCRIBE_TOPIC, 0);

    while (TRUE)
    {
    }
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