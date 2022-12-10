#ifndef COMUNICATION_H
#define COMUNICATION_H

/**
 * Tabela de comandos aceitos entre a SBC e o nodeMCU
 */
#define GET_NUMBER_OF_SENSORS 0x21        //  33 - !
#define GET_SENSOR_ADDRESS 0x53           //  83 - S
#define GET_SENSOR_NAME 0x4E              //  78 - N
#define GET_DIGITAL_INPUT_VALUE 0x05      //  05
#define GET_ANALOG_INPUT_VALUE 0x04       //  04
#define GET_NODE_MCU_STATUS 0x03          //  03
#define NODE_MCU_ON_LED_BUILTIN 0x06      //  06
#define NODE_MCU_OFF_LED_BUILTIN 0x09     //  09

// Resonse Command
#define NODE_MCU_STATUS_ERROR 0x1F        //  31
#define NODE_MCU_STATUS_OK 0x00           //  00
#define ANALOG_LEVEL 0x01                 //  01
#define DIGITAL_LEVEL 0x02                //  02
#define NUMBER_OF_DIGITAL_PORTS 0x40      //  64 - @
#define DIGITAL_PORT_NAME 0x41            //  65 - A
#define SENSOR_ADDRESS_VALUE 0x42         //  66 - B
#define LED_CHANGED_VALUE 0x43            //  67 - C

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/**
 * @brief Envia resposta atravésda UART
 * 
 * @param code codigo de resposta
 * @param value valor da resposta
 */
void send_response(unsigned char code, unsigned char value, unsigned char second_value) {
  Serial.write(code);
  Serial.write(value);
  Serial.write(second_value);
  Serial.printf(" - %i_%i_%i\n", code, value, second_value);
}

void send_response(unsigned char code, unsigned char value) {
  send_response(code, value, '\0');
}

/**
 * @brief Envia resposta de erro
 * 
 * @param specific_error_code codigo de erro específico ao 
 * procedimento que gerou o erro
 */
void send_error(unsigned char specific_error_code) {
  send_response(NODE_MCU_STATUS_ERROR, specific_error_code);
}

#endif
