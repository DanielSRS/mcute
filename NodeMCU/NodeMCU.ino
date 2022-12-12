#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "comunication.h"
#include "io.h"
#include "mqtt.h"
#include <PubSubClient.h>

void callback(char* topic, byte* payload, unsigned int length);
void mqtt_publish(char *message);

long unsigned int initialTime = 0;
WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, callback, espClient);

void setup() {
  ota_setup();        // Configuração de atualização via OTA
  setup_io_pins();    // Define pinos da GPIO como entrada e acende o led
  client.connect(client_name, broker_username, broker_password);
  client.subscribe(listen_command_topic);
}

void loop() {
  handle_ota();
  client.loop();

  String leitura = Serial.readString();

  if (leitura.length() > 0) {
    int comando = leitura.charAt(0);
    int endereco = leitura.charAt(1);
    process_command(comando, endereco);
  }
}

void process_command(int comando,int endereco) {
      switch (comando) {
      case GET_DIGITAL_INPUT_VALUE: // Le o valor de um sensor digital
        {
          int res = read_digital_input(endereco);
          res != -1
            ? send_response(DIGITAL_LEVEL, res + '0', &mqtt_publish) // retorna valor com sucesso
            : send_error(DIGITAL_LEVEL, &mqtt_publish);                // retorna erro de port inexistente
        }
        break;

      case GET_NUMBER_OF_SENSORS: // obtem Numero de sensores
        send_response(NUMBER_OF_DIGITAL_PORTS, NELEMS(digital) + '0', &mqtt_publish);
        break;

      case NODE_MCU_ON_LED_BUILTIN: // Ligar o led
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
        send_response(LED_CHANGED_VALUE, '1', &mqtt_publish); // led ligado com sucesso
        break;

      case NODE_MCU_OFF_LED_BUILTIN: //  desligar o led
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        send_response(LED_CHANGED_VALUE, '0', &mqtt_publish);
        break;

      case GET_ANALOG_INPUT_VALUE: // Fazer a leitura do sensor analogico
        {
          String res = read_analog_input();
          send_response(ANALOG_LEVEL, res.charAt(0), res.charAt(1), &mqtt_publish);
        }
        break;

      // Obtem o endereço do sensor
      // onde o valor do endereço deve estar entre 1 e o valor retornado
      // pela função GET_NUMBER_OF_SENSORS
      case GET_SENSOR_ADDRESS: 
        {
          int res = get_digital_io_address(endereco);
          res != -1
            ? send_response(SENSOR_ADDRESS_VALUE, res, &mqtt_publish) // resposta
            : send_error(SENSOR_ADDRESS_VALUE, &mqtt_publish); // erro
        }
        break;

      case GET_SENSOR_NAME: // Obter o nome do sensor
        {
          char *name = get_digital_io_name(endereco);
          name != NULL
            ? send_response(DIGITAL_PORT_NAME, name[0], name[1], &mqtt_publish)
            : send_error(DIGITAL_PORT_NAME, &mqtt_publish);
        }
        break;

      case GET_NODE_MCU_STATUS: // Obter o status da placa
        send_response(NODE_MCU_STATUS_OK, NODE_MCU_STATUS_ERROR, &mqtt_publish);
        break;
      
      // Comando não reconhecido
      default:
        send_error(NODE_MCU_STATUS_ERROR, &mqtt_publish);
        break;
    }
    printf("Respondido em: %lu ms\n", millis() - initialTime);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  //
  // Allocate the correct amount of memory for the payload copy
  String topico = String(topic);
  printf("Recebido mensagem no topico: %s\n", topic);
  initialTime = millis();

  if(topico.equals("comand")){
    printf("processando comando\n");
    char* p = (char*)malloc(length);
    // Copy the payload to the new buffer
    memcpy(p,payload,length);
    String leitura = p;
    int comando = leitura.charAt(0);
    int endereco = leitura.charAt(1);
    process_command(comando, endereco);
    // Free the memory
    free(p);
  }
}

void mqtt_publish(char *message) {
  client.publish("output", message);
}
