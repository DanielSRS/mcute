#ifndef SENSOR_H
#define SENSOR_H

enum sensor_type { Undefined, Analogic, Digital };

/**
 * @brief Informações sobre um sensor
 *
 */
struct sensor_data {
    char name[17];
    //char default_name[2];
    //char description[255];
    int value;
    int id;
    enum sensor_type type;
};

typedef struct sensor_data Sensor;

void print_sensor_to_console(char *sensor_name, int sensor_value);

#endif
