#include <termios.h> // struct termios, tcsetattr, TCSAFLUSH, tcgetattr, ECHO, ICANON, ISIG, IXON, TCSAFLUSH
#include <unistd.h> // STDIN_FILENO, 

/**
 * Desabilita o modo raw
 */
void disableRawMode(struct termios orig_termios) {
  // reescreve as configurações originis
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/**
 * Coloca o terminal em modo row (onde é possivel ler cada byte do input)
 * diretamente, sem que o usuário precise pressionar enter
 */
struct termios enableRawMode() {

  // configurações salvas do terminal
  struct termios orig_termios;
    
  // Lê e salva os atributos atuais
  tcgetattr(STDIN_FILENO, &orig_termios);
  
  // desavilita o modo raw sempre que o program enxerrar, independete do motivo
  // de forma automatica
  //atexit(disableRawMode);
  
  // copia os valore antes de modificar
  struct termios raw = orig_termios;

  /**
   * Desabilita o eco do input
   * Desabilita o modo canonico
   * Desabilita Ctrl-C
   */
  raw.c_lflag &= ~(ECHO | ICANON | ISIG);

  /**
   * @brief Desabilita Crtl-S e Crtl-Q
   * 
   */
  raw.c_iflag &= ~(IXON);
  
  // define os novos atirbutos
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  return orig_termios;
}
