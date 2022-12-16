#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>
#include "mqtt.h"
#include "comunication.h"

int configMqttClient(ContextData *data) {
    printf("entra nas config de: %p\n", data);
    /*Configuração do Cliente*/

    printf("criado obj\n");
    printf("client antes da config: %p\n", data->client);

    /* Inicializacao do MQTT (conexao & subscribe) */
    MQTTClient_create(&data->client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    printf("Cliente depois da config: %p\n", data->client);
    MQTTClient_setCallbacks(data->client, data, on_disconnect, on_message, NULL);

    printf("Setando os callbacks em: %p - %i\n", data->client, data->verif);

    return MQTTClient_connect(data->client, data->MQTT_CONFIG);
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
    //printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName, payload);
    int val = payload[1] - '0';
    int response_type = payload[0];

    // Atualiza o valor de um sensor digital
    if (response_type == ANALOG_LEVEL) {
        int update = 1;
        if (data->update_blocked == 1) {
            update = 0;
        }
        //printf("Atualizando valor do analogico\n");
        //data->analogic->valuees = command_to_int(payload[1], payload[2]);
        if (update) {
            if (data->analogic->values.number_of_items >= data->analogic->values.max_lenght) {
                pop(&data->analogic->values);
            }
            push(&data->analogic->values, command_to_int(payload[1], payload[2]));
        }
    }

    // Atualiza valor de um dos sensores digitais
    if (response_type == DIGITAL_LEVEL) {
        int update = 1;
        if (data->update_blocked == 1) {
            update = 0;
        }
        if (update) {
            int digital_sensor_address = payload[2];
            for (int i = 0; i < *data->digitalQtd; i++) {
                if (digital_sensor_address == data->digitals[i].id) {
                    //data->digitals[i].value = payload[1] - '0';
                    if (data->digitals[i].values.number_of_items >= data->digitals[i].values.max_lenght) {
                        pop(&data->digitals[i].values);
                    }
                    push(&data->digitals[i].values, payload[1] - '0');
                    //printf("Sensor %i: %s\n", i + 1, data->digitals[i].name);
                    //printf("new val: %i \n", data->digitals[i].value);
                }
            }
        }
    }

    if (response_type == NODE_MCU_STATUS_ERROR) {
        printf("Node mcu com erro. ");
        if (payload[1] == DIGITAL_LEVEL) {
            printf("Endereço do sensor incorreto!! \n");
        }
        if (payload[1] == SENSOR_ADDRESS_VALUE) {
            printf("Indice do sensor inváido!! \n");
        }
        if (payload[1] == DIGITAL_PORT_NAME) {
            printf("Endereço do sensor incorreto!! \n");
        }
    }

    /* Faz echo da mensagem recebida */
    //publish(data->client, MQTT_PUBLISH_TOPIC, payload);


    if (strcmp("nwUP", topicName) == 0) {
        int nnn = atoi(payload);
        *data->update_interval = nnn;
    }
    MQTTClient_freeMessage(&message);
    //MQTTClient_free(topicName);
    return 1;
}

void on_disconnect(void *context, char *cause) {
  printf("\n\nDesconectado!!\n");
  printf("Causa: %s\n\n\n", cause);

  ContextData *data = context;

  printf("Client mqtt: %p\n", data->client);
  printf("Client mqtt: %i\n", data->verif);

  int rc = MQTTClient_connect(data->client, data->MQTT_CONFIG);

    if (rc != MQTTCLIENT_SUCCESS)
    {
        printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
        exit(-1);
    } else {
        printf("Reconectado\n");
    }

    MQTTClient_subscribe(data->client, MQTT_SUBSCRIBE_TOPIC, 0);
};
