#include "display.h" // clear_display, write_string
#include <lcd.h>
#include <stdio.h> // sprintf, printf

void escreverEmDuasLinhas(char linha1[], char linha2[]);

void print_sensor_to_console(char *sensor_name, int sensor_value) {
  char str[16];
  sprintf(str, "%9s: %4i", sensor_name, sensor_value);
  //clear_display();
  //str[17] = '\0';
  //write_string((char*) str);
  //printf("%9s: %4i\n", sensor_name, sensor_value);
  escreverEmDuasLinhas(str, "  ");
}


void escreverEmDuasLinhas(char linha1[], char linha2[]) {
        lcdHome(lcd);
        lcdPuts(lcd, linha1);
        lcdPosition(lcd,0,1);
        lcdPuts(lcd, linha2);
}

void init_lcd() {
  lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7,0,0,0,0);
}

