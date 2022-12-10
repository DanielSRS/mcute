/**
 * @brief Dado os dois bytes da resposta de um comando, retorna um numero inteiro.
 * Como todos os comandos (enviados, ou recebidos) tem dois bytes, não é possível
 * ler valores maiores que 255 (base 10).
 *
 * Como o valor de leitura do sensor analógico pode retornar valores até 1024, a
 * node MCU quebra esse valor em doys bytes no comando de resposta, e esse valor
 * é remontado por essa função;
 *
 * @param a primeiro byte de resposta
 * @param b segundo byte de resposta
 * @return int (um numero inteiro com tamanho de dois bytes)
 */
int command_to_int(char a, char b) {
    int val = a;
    val = val << 8;
    return val | b;
}
