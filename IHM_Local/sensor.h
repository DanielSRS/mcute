#include "queue.h"
#ifndef SENSOR_H
#define SENSOR_H

//USE WIRINGPI PIN NUMBERS - 103
#define LCD_RS  13               //Register select pin
#define LCD_E   18               //Enable Pin
#define LCD_D4  21               //Data pin D4
#define LCD_D5  24               //Data pin D5
#define LCD_D6  26               //Data pin D6
#define LCD_D7  27              //Data pin D7

int lcd;

enum sensor_type { Undefined, Analogic, Digital };

/**
 * @brief Informações sobre um sensor
 *
 */
struct sensor_data {
    char name[17];
    //char default_name[2];
    //char description[255];
    struct queue_head values;
    int id;
    enum sensor_type type;
};

typedef struct sensor_data Sensor;

void print_sensor_to_console(char *sensor_name, int sensor_value);
void escreverEmDuasLinhas(char linha1[], char linha2[]);
void init_lcd();

#endif
