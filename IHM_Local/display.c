#include <string.h>
#include "display.h"

/**
 * @brief Escreve uma string no display
 *
 * @param s string a ser escrita no display
 */
void write_string(char* s) {
  for (int i = 0; i < strlen(s) && i < 16; i++) {
    write_char(s[i]);
  }; 
}
