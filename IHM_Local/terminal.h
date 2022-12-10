#ifndef TERMINAL_H
#define TERMINAL_H

struct termios enableRawMode();
void disableRawMode(struct termios orig_termios);

#endif