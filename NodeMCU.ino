#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "comunication.h"
#include "io.h"
#include <PubSubClient.h>

const char* mqtt_server = "10.0.0.101";
WiFiClient espClient;

void callback(char* topic, byte* payload, unsigned int length);

PubSubClient client(mqtt_server,1883,callback,espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  //
  // Allocate the correct amount of memory for the payload copy
  String topico = String(topic);

  if(topico.equals("comand")){
    char* p = (char*)malloc(length);
    // Copy the payload to the new buffer
    memcpy(p,payload,length);
    String leitura = p;
    int comando = leitura.charAt(0);
    int endereco = leitura.charAt(1);
    comand(comando,endereco);
    // Free the memory
    free(p);
  }


  
} 

void comand(int comando,int endereco){
      char* output;
      switch (comando) {
      case GET_DIGITAL_INPUT_VALUE: // Le o valor de um sensor digital
        {
          int res = read_digital_input(endereco);
          if(res != -1){
            output[0] = char(DIGITAL_LEVEL);
            output[1] = char(res);
          }else{
            output[0] = char(DIGITAL_LEVEL);
            output[1] = char(NODE_MCU_STATUS_ERROR);
          }
          client.publish("output",output);
          /*res != -1
            ? send_response(DIGITAL_LEVEL, res + '0') // retorna valor com sucesso
            : send_error(DIGITAL_LEVEL);                // retorna erro de port inexistente
          */
        }
        break;

      case GET_NUMBER_OF_SENSORS: // obtem Numero de sensores
        {
        //send_response(NUMBER_OF_DIGITAL_PORTS, NELEMS(digital) + '0');
         output[0] = char(NUMBER_OF_DIGITAL_PORTS);
         output[1] = char(NELEMS(digital)); 
         client.publish("output",output);        
        }
        break;

      case NODE_MCU_ON_LED_BUILTIN: // Ligar o led
       {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
        //send_response(LED_CHANGED_VALUE, '1'); // led ligado com sucesso
        output[0] = char(LED_CHANGED_VALUE);
        output[1] = char(0x01); 
        client.publish("output",output);
       }
        break;

      case NODE_MCU_OFF_LED_BUILTIN: //  desligar o led
        {
          pinMode(LED_BUILTIN, OUTPUT);
          digitalWrite(LED_BUILTIN, HIGH);
          output[0] = char(LED_CHANGED_VALUE);
          output[1] = char(0x00); 
          client.publish("output",output);
          //send_response(LED_CHANGED_VALUE, '0');
        }
        break;

      case GET_ANALOG_INPUT_VALUE: // Fazer a leitura do sensor analogico
        {
          String res = read_analog_input();
          //send_response(ANALOG_LEVEL, res.charAt(0), res.charAt(1));
          output[0] = char(ANALOG_LEVEL);
          output[1] = res.charAt(0); 
          output[2] = res.charAt(1);
          client.publish("output",output);
        }
        break;

      // Obtem o endereço do sensor
      // onde o valor do endereço deve estar entre 1 e o valor retornado
      // pela função GET_NUMBER_OF_SENSORS
      case GET_SENSOR_ADDRESS: 
        {
          int res = get_digital_io_address(endereco);
          if(res != -1){
            output[0] = char(SENSOR_ADDRESS_VALUE);
            output[1] = char(res); 
          }else{
            output[0] = char(SENSOR_ADDRESS_VALUE);
            output[1] = char(NODE_MCU_STATUS_ERROR); 
          }
          client.publish("output",output);
          /*res != -1
            ? send_response(SENSOR_ADDRESS_VALUE, res) // resposta
            : send_error(SENSOR_ADDRESS_VALUE); // erro*/
        }
        break;

      case GET_SENSOR_NAME: // Obter o nome do sensor
        {
          char *name = get_digital_io_name(endereco);
          if(name != NULL){
            output[0] = char(DIGITAL_PORT_NAME);
            output[1] = name[0]; 
            output[2] = name[1];
          }else{
            output[0] = char(DIGITAL_PORT_NAME); 
            output[1] = char(NODE_MCU_STATUS_ERROR); 
          }
          client.publish("output",output);
         /*name != NULL
            ? send_response(DIGITAL_PORT_NAME, name[0], name[1])
            : send_error(DIGITAL_PORT_NAME);*/
        }
        break;

      case GET_NODE_MCU_STATUS: // Obter o status da placa
        {
          output[0] = char(NODE_MCU_STATUS_OK); 
          output[1] = char(NODE_MCU_STATUS_ERROR); 
          client.publish("output",output);
        }
        //send_response(NODE_MCU_STATUS_OK, NODE_MCU_STATUS_ERROR);
        break;
      
      // Comando não reconhecido
      default:
        {
          output[0] = char(NODE_MCU_STATUS_ERROR); 
          client.publish("output",output);
        }
        //send_error(NODE_MCU_STATUS_ERROR);
        break;
    }

}

void setup() {
  ota_setup();        // Configuração de atualização via OTA
  setup_io_pins();    // Define pinos da GPIO como entrada e acende o led
  client.connect("esp","admin","123");
  client.subscribe("comand");
}

void loop() {

  client.loop();

  handle_ota();
}
