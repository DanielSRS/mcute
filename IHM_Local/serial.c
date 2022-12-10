#include <termios.h>                // termios, tcgetattr, B115200, CS8, CLOCAL, CREAD, IGNPAR, tcflush, TCIFLUSH, tcsetattr, TCSANOW
#include <fcntl.h>                  // open, O_RDWR, O_NOCTTY, O_NDELAY
#include <stdio.h>                  // printf
#include <unistd.h>                 // read, write
#include <string.h>                 // strlen

int uart0_filestream = -1;

/**
 * @brief Realiza a configuração da uart
 *
 */
void uart_configure()
{
  // Abre o arquivo da uart
  uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

    struct termios options;

    // opções de configuração da
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

/**
 * @brief Faz a leitura do buffer com as informações lidas pela uart
 *
 * @param l Variável de destino da string
 */
void serialReadBytes(unsigned char l[100])
{
    static unsigned char rx_buffer[100];
    char response_command = -1;

    if (uart0_filestream != -1)
    {

        int rx_length = read(uart0_filestream, (void *)rx_buffer, 100); // Filestream, buffer to store in, number of bytes to read (max)

        if (rx_length > 0)
        {
            rx_buffer[rx_length] = '\0';
            //printf("%s", rx_buffer);
            for (int i = 0; i < 100; i++) {
                l[i] = rx_buffer[i];
            }
        }
    }
}

/**
 * @brief Envia string de forma seria pela uart
 *
 * @param tx_string String a ser enviada
 * @return int Valor de sucesso do envio. retorna -1 se erro
 */
int uart_send_string(char* tx_string) {
    if (uart0_filestream != -1)
        return write(uart0_filestream, tx_string, strlen(tx_string));           //Filestream, bytes to write, number of bytes to write
    return -1;
}

/**
 * @brief Envia um comando via uart
 * 
 * @param command 
 * @param value 
 */
void send_command(unsigned char command, unsigned char value) {
    char cmd[2] = "OO";
    cmd[0] = command;
    cmd[1] = value;
    uart_send_string((char*) cmd);
}
