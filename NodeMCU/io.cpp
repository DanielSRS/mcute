/*
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
*/
#include <ArduinoOTA.h> // uint8_t
#include "io.h"

/**
 * @brief Obtem as informações de uma porta digita
 * dado o seu id
 * 
 * @param id endereço da porta
 * @return struct IO Informações da porta como nome.
 */
struct IO find_digital_IO_by_id(int id) {
  for (int index = 0; index < 9; index++) {
    struct IO io = digital[index];
    if (io.id == id) {
      return io;
    }
  }
  return { .name = (char*) "ER", .id = (uint8_t) -1};
}

/**
 * @brief Obtem as informações de uma porta digita
 * dado o seu index, onde index é a ordem da lista
 * digital[]
 * 
 * @param id index da porta
 * @return struct IO Informações da porta como nome.
 * @see digital[]
 */
struct IO find_digital_IO_by_index(int in) {
  for (int index = in; index < 9; index++) {
    struct IO io = digital[index];
    if (in == index) {
      return io;
    }
  }
  return { .name = (char*) "ER", .id = (uint8_t) -1};
}

/**
 * @brief Get the digital io name
 * 
 * @param io_id id/endereço da entrada digital
 * @return char* nome da entrada digital
 */
char* get_digital_io_name(unsigned char io_id) {
  struct IO sensor = find_digital_IO_by_id(io_id);
  if (sensor.id == (unsigned char) -1) {
    return 0;
  } else {
    return sensor.name;
  }
}

/**
 * @brief Obtem o id/endereço de uma das entradas digitais
 * dado seu index. (valor detro do range retornado por GET_NUMBER_OF_SENSORS comand)
 * 
 * @param index index da entrada de acordo com o valor de digitais
 * @return unsigned char io address/id
 */
int get_digital_io_address(int index) {
  struct IO sensor = find_digital_IO_by_index(index - 1);
  if (sensor.id == (unsigned char) -1) {
    return -1;
  } else {
    return sensor.id;
  }
}

/**
 * @brief Lê o valor da entrada analogica
 * 
 * @return String valor lido codificado em string
 */
String read_analog_input(void){
  int value = analogRead(analogic.id);
  char first_half = value >> 8;
  char second_half = value;
  //Serial.printf("%c%c", first_half, second_half);
  String res = "00";
  res.setCharAt(0, first_half);
  res.setCharAt(1, second_half);
  return res;
}

/**
 * @brief Lê o valor de uma entrada digital, dado o
 * endereço da entrada digita.
 * 
 * @param io_address id da entrada digital
 * @return int Se sucesso, retorna 0 ou 1, dependendo do valor liso.
 * Em caso de erro. retorna -1
 */
int read_digital_input(unsigned char io_address) {
  struct IO sensor = find_digital_IO_by_id(io_address);
  if (sensor.id == (unsigned char) -1) {
    return -1; // nao existe sensor com o id informado
  } else {
    pinMode((uint8_t) sensor.id, INPUT);
    return digitalRead((unsigned char) sensor.id); 
  }
}

/**
 * Define os pinos da GPIO (onde podem ser conectados sensores digitais)
 * como modo de entrada e liga o led
 */
void setup_io_pins() {
  pinMode(16, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(0, INPUT);
  pinMode(2, INPUT);
  pinMode(14, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(15, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
 }
 
