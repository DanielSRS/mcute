#ifndef UTILS_H
#define UTILS_H

#define Lenght(x)  (sizeof(x) / sizeof((x)[0]))

void await(int milisseconds);
int increment_in_range(int current, int max);
int decrement_in_range(int current, int min);
int read_input_char(char * input);
int isSelected(int current, int selected);

#endif
