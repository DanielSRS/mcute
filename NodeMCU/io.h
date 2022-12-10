#ifndef IO_H 
#define IO_H

struct IO {
  char* name;
  uint8_t id;
};

/** Porta analógica disponível */
const struct IO analogic = { .name = (char*) "A0", .id = A0};

/** Portas digitais disponiveis */
const struct IO digital[] = {{ .name = (char*) "D0", .id = 16},
                             { .name = (char*) "D1", .id = 5 },
                             { .name = (char*) "D2", .id = 4 },
                             { .name = (char*) "D3", .id = 0 },
                             { .name = (char*) "D4", .id = 2 },
                             { .name = (char*) "D5", .id = 14},
                             { .name = (char*) "D6", .id = 12},
                             { .name = (char*) "D7", .id = 13},
                             { .name = (char*) "D8", .id = 15}};

struct IO find_digital_IO_by_id(int id);
struct IO find_digital_IO_by_index(int in);
char* get_digital_io_name(unsigned char io_id);
int get_digital_io_address(int index);
String read_analog_input(void);
int read_digital_input(unsigned char io_address);
void setup_io_pins();

#endif
