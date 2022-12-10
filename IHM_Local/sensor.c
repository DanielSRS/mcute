#include "display.h" // clear_display, write_string
#include <stdio.h> // sprintf, printf

void print_sensor_to_console(char *sensor_name, int sensor_value) {
  char str[16];
  sprintf(str, "%9s: %4i", sensor_name, sensor_value);
  clear_display();
  //str[17] = '\0';
  write_string((char*) str);
  printf("%9s: %4i\n", sensor_name, sensor_value);
}
