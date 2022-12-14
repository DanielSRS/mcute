#include <stdio.h>
#include <string.h>

void clear_display(void) {
  //printf("--Clear display--\n");
  printf("\e[1;1H\e[2J");
  //printf("\n");
}

void init_display(void) {
  printf("--Init display--\n");
}
void write_char(char c) {
  printf("%c", c);
}
