#include <unistd.h>     // NULL, read, STDIN_FILENO
#include <sys/time.h>   // struct timeval, gettimeofday

/**
 * @brief Delay em milisegundos antes de continuar a execução
 * 
 * @param milisseconds tempo do delay em milisegundos
 */
void await(int milisseconds) {
  struct timeval start;
  gettimeofday(&start, NULL);
  long long initial_time = start.tv_sec*1000LL + start.tv_usec/1000;    // tempo inicial em ms
  long long current_time;
  long long elapsed_milliseconds;
  
  do {
    gettimeofday(&start, NULL);
    current_time = start.tv_sec*1000LL + start.tv_usec/1000;            // tempo atual em ms
    elapsed_milliseconds = current_time - initial_time;                 // tempo decorrido
    if (elapsed_milliseconds >= milisseconds) {                         // se já passou o tempo estipulado em ms
      return;                                                           // retorna
    }
  } while(1);
  
}

/**
 * @brief Incrementa um valor até um maximo definido
 * 
 * @param current valor atual
 * @param max maximo valor do incremento
 * @return int valor incrementado, caso menor que o maximo.
 * do contrario, retorna o valor maximo
 */
int increment_in_range(int current, int max) {
  if (current + 1 >= max) {
    return max;
  }
  return current + 1;
}

/**
 * @brief Decrementa um valor até um maximo definido
 * 
 * @param current valor atual
 * @param min minimo valor do decremento
 * @return int valor decrementado, caso maior que o minimo.
 * do contrario, retorna o valor minimo
 */
int decrement_in_range(int current, int min) {
  if (current - 1 <= min) {
    return min;
  }
  return current - 1;
}

/**
 * @brief Lê um caractere da entrada padrão
 * 
 * @param input variavel para armazenar o valor lido
 * @return int 1 se sucesso, -1 se houver algum erro ou 0
 * pois chegou no fim de arquivo
 */
int read_input_char(char * input) {
  return read(STDIN_FILENO, input, 1);
}

/**
 * @brief Verifica se há uma seleção. (quando ambos os valores são iguais)
 * 
 * @param current Valor sendo verificado
 * @param selected Valor da seleção 
 * @return int 1 se iguais 0 do contrario
 */
int isSelected(int current, int selected) {
  if (current == selected)
    return 1;
  return 0;
}

