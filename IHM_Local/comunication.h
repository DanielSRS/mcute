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

int command_to_int(char a, char b);

#endif