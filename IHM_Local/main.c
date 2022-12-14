#include <stdio.h>            // printf,
#include <stdlib.h>           // atoi
#include <string.h>           // strncpy, strtok, strlen, strcmp,
#include "display.h"          // write_char, init_display, clear_display
#include "sensor.h"           // Sensor, print_sensor_to_console,
#include "serial.h"           // uart_configure, uart_send_string, serialReadBytes
#include "comunication.h"     // command_to_int
#include "utils.h"            // await
#include "menu.h"             // menu
#include "help.h"
//#include <lcd.h>
#include <MQTTClient.h>
//#include <wiringPi.h>
#include "mqtt.h"
#include "queue.h"

//USE WIRINGPI PIN NUMBERS
#define LCD_RS  13               //Register select pin
#define LCD_E   18               //Enable Pin
#define LCD_D4  21               //Data pin D4
#define LCD_D5  24               //Data pin D5
#define LCD_D6  26               //Data pin D6
#define LCD_D7  27               //Data pin D7

// Botões
#define BUTTON_1 19
#define BUTTON_2 23
#define BUTTON_3 25

MQTTClient client;
/*int lcd;

void escreverEmDuasLinhas(char linha1[], char linha2[]) {
        lcdHome(lcd);
        lcdPuts(lcd, linha1);
        lcdPosition(lcd,0,1);
        lcdPuts(lcd, linha2);
}*/

//int isPressed(int btt);

int get_number_of_digital_ios();
char *getSubstring(char *dst, const char *src, size_t start, size_t end);
void separate_string_in_3(char *str, char a[10], char b[10], char c[10]);
void print_io_name_and_id(int max_digital);
int add_digital_sensor(char *sensor_info, Sensor *digital);

/**
 * @brief Realiza a leitura de sensores de forma automatizada atraves de comunicação serial
 * da raspberry pi
 *
 * @param argc Numero de argumentos da linha de comando recebidos
 * @param argv Lista de argumentos recebidos
 * @return int Retorna 0 se executado com sucesso. Qualquer outro valor em caso de execução incorreta
 */
int main(int argc, char *argv[]) {
    // Configuração inicial
    init_display();                   // configura o display
    init_display();
    init_display();
    clear_display();                  // Limpa o conteudo do diaplay
    write_string("Iniciando");
    uart_configure();                 // configura a uart
    //wiringPiSetup();
    //lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7,0,0,0,0);
    //pinMode(BUTTON_1,INPUT);
    //pinMode(BUTTON_2,INPUT);
    //pinMode(BUTTON_3,INPUT);

    //escreverEmDuasLinhas("     MI - SD    ", "Protocolo MQTT");
    /*while (1) {
        //printf("no loop\n\n");
        isPressed(BUTTON_1);
        isPressed(BUTTON_2);
        //await(1000);
    }*/
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    Sensor analogico;
    Sensor digital[31];
    int digitalQtd = 0;               // Quantidade de sensores digitais selecionados
    ContextData data;
    data.analogic = &analogico;
    data.digitals = digital;
    data.digitalQtd = &digitalQtd;
    data.verif = 753;
    data.MQTT_CONFIG = &conn_opts;


    /*****************************************************************/
    printf("Inicio da configuração do mqtt\n");
    int rc = configMqttClient(&data);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
        exit(-1);
    } else {
      printf("conectado!!\n\n");
    }
    MQTTClient_subscribe(data.client, MQTT_SUBSCRIBE_TOPIC, 0);
    printf("Fim da configuração do mqtt em: %p\n", &data);
    /*****************************************************************/

    //escreverEmDuasLinhas("     MI - SD    ", "Protocolo MQTT");
    unsigned char ler[100];           // Leitura de respostas

    if (argc < 2) {                   // Se não tiver argumentos, encerra o programa
      printf("Uso inválido. Não há argumentos\n");
      return 0;
    }

    /**
     * Percorre a lista de argumentos para definir os sensores
     */
    for (int index = 1; index < argc; index++) {
        char str[6] = "\0\0\0";
        /**
         * @brief Se adicionado um sensor analogico
         */
        if (strcmp(argv[index], "-analogic") == 0) {
            printf("Há um sensor na porta analogica\n");
            char *j = "Analogico";
            analogico.name[9] = '\0';
            for (int i = 0; i < strlen(j) && i < 10; i++) {
                    //printf("%c", ptr[i]);
                    analogico.name[i] = j[i];
                }
                analogico.name[10] = '\0';
            //analogico.value = 0;
            //printf("\n\nDefinindo tamano do analogicon\n\n");
            struct queue_head *leituras = &(analogico.values);
            leituras->max_lenght = 10;
            leituras->number_of_items = 0;
            push(leituras, 0);
            printf("V - %i\n", leituras->number_of_items);
            analogico.id = 'A';
            analogico.type = Analogic;
        }
        /**
         * Se inserido uma porta digital
         */
        else if (strcmp(getSubstring(str, argv[index], 0, 2), "-d") == 0) {
            //char name[10];
            if (digitalQtd >= 31) {
                printf("Atingido maximo de sensores digitais!\n");
                continue;
            }
            int res = add_digital_sensor(argv[index], &digital[digitalQtd]);
            if (res == -1) { // se hoube erro
              exit(0);        // termina o programa
            }                 // do contrario
            digitalQtd++;     // contabilize o sensor adicionado

        }

        /**
         * @brief Se listando os sensores disponíveis
         */
        else if (strcmp(argv[index], "-l") == 0) {
          unsigned char read[100]; // Leitura de respostas
          //printf("\nPegando o numero de portas digitais disponíveis: \n");
          int max_digital = get_number_of_digital_ios();
          if (max_digital < 0) {
            printf("Erro na leitura do numero de portas disponíveis\n");
            return 0; // encerra programa
          }
          /** Numero de portas digitais disponíveis */
          print_io_name_and_id(max_digital);
        }

        /** Se opção manual */
        else if (strcmp(argv[index], "-m") == 0) {
          // exibe menu
          char notice[80] = " ";
          while (1)
          {
            menu_item entries[] = {
              {
                .item_name = "Status da NodeMCU", .id = 0
              },
              {
                .item_name = "Ligar o led", .id = 1
              },
              {
                .item_name = "Desligar o led", .id = 2
              },
              {
                .item_name = "Ler sensor analogico", .id = 3
              },
              {
                .item_name = "Ler sensor digital", .id = 4
              },
              {
                .item_name = "Sair", .id = 5
              },
            };
            int selected_entrie_id = menu(
              entries,
              Lenght(entries),
              "Use as setas para navegar entre as opções do menu. Pressione enter para selecionar a opção",
              "",
              notice
            );

            switch (selected_entrie_id)
            {
              case 0:
                send_command(GET_NODE_MCU_STATUS, 'O');
                printf("Obtendo status da NodeMCU...\n");
                await(3000);
                //serialReadBytes(ler); // lê resposta
                if (ler[0] == NODE_MCU_STATUS_OK && ler[1] == NODE_MCU_STATUS_ERROR) {
                  sprintf(notice, "Status: OK");                                                   // Limpa mensagem
                } else {
                  sprintf(notice, "Status: ERRO!");
                }
                clear_display();
                write_string(notice);
                break;

              case 1:
                send_command(NODE_MCU_ON_LED_BUILTIN, 'L');
                sprintf(notice, "LED ligado");
                clear_display();
                write_string(notice);                                            // Limpa mensagem
                break;

              case 2:
                send_command(NODE_MCU_OFF_LED_BUILTIN, 'L');
                sprintf(notice, "LED desligado");
                clear_display();
                write_string(notice);                                                   // Limpa mensagem
                break;

              case 3:
                send_command(GET_ANALOG_INPUT_VALUE, GET_ANALOG_INPUT_VALUE);
                printf("Lendo sensor analogico...\n");
                await(3000);
                //serialReadBytes(ler); // lê resposta
                print_sensor_to_console("Analogico", command_to_int(ler[1], ler[2]));
                sprintf(notice, "%9s: %4i", "Analogico", command_to_int(ler[1], ler[2]));
                break;

              case 4:
                printf("Digite o endereço do sensor: ");
                scanf("%s", ler);
                int sensor_address = atoi(ler);
                printf("Lendo sensor digital...\n");
                send_command(GET_DIGITAL_INPUT_VALUE, (char) sensor_address);           // Solicita leitura do sensor
                await(3000);                                                            // Aguarda comando ser processado
                //serialReadBytes(ler);                                                   // lê resposta


                if (ler[0] == NODE_MCU_STATUS_ERROR) {                                                     // Se houve erro na leitura
                  printf("NodeMCU com problema. Endereço do sensor é inválido!\n");
                  clear_display();
                  write_string("NodeMCU com erro");
                } else {                                                                // Se lido com sucesso
                  print_sensor_to_console("Sensor", ler[1] - '0');                         // Exibe informações lidas
                  sprintf(notice, "Sensor: %i: %4i", sensor_address, ler[1] - '0');
                }
                break;

              case 5:
                exit(0);
                break;

              default:
                sprintf(notice, " ");                                                   // Limpa mensagem
                break;
            }
          }

        }

        // menu de ajuda
        else if (strcmp(argv[index], "-h") == 0) {
          help("general");
          exit(0);
        }

        // Comando inválido
        else {
          printf("\n\nComando inválido!!\nEncerrando...\n");
          exit(0);
        }

    }

    printf("Iniciando leitura\n\n");

    char cmd[2] = "OO";

    while(1) {
      /**
       * Lê o sensor analogico
       */
      if (analogico.type == Analogic) { // Se existe um sensor analogico
        struct queue_head *leituras = &(analogico.values);
        int val = peek_value(leituras);
        print_sensor_to_console(analogico.name, val);
        send_command(GET_ANALOG_INPUT_VALUE, GET_ANALOG_INPUT_VALUE);
        cmd[0] = GET_ANALOG_INPUT_VALUE;
        cmd[1] = GET_ANALOG_INPUT_VALUE;
        // solicita valor para a ESP
        publish(data.client, "comand", (char*) cmd);
        // publica historico de valores
        char history[255];
        publish(data.client, "analogic/history", queue_to_string(leituras, &history));
        await(1000);
        //serialReadBytes(ler); // lê resposta
        //analogico.value = command_to_int(ler[1], ler[2]);
      }

      /**
       */
      for (int i = 0; i < digitalQtd; i++) {
        struct queue_head *leituras = &(digital[i].values);
        int val = peek_value(leituras);
        print_sensor_to_console(digital[i].name, val);  
        send_command(GET_DIGITAL_INPUT_VALUE, (char) digital[i].id);
        cmd[0] = GET_DIGITAL_INPUT_VALUE;
        cmd[1] = (char) digital[i].id;
        publish(data.client, "comand", (char*) cmd);            // Solicita leitura do sensor
        char history[255];
        char topic[255];
        sprintf(topic, "%i/history", cmd[1]);
        printf("Topico: %s\n", topic);
        publish(data.client, topic, queue_to_string(leituras, &history));
        await(1000);                                                            // Aguarda comando ser processado
        //serialReadBytes(ler);                                                   // lê resposta

        if (0) {                                                     // Se houve erro na leitura
          printf("NodeMCU com problema. Endereço do sensor é inválido!\n");
          clear_display();
          write_string("NodeMCU com erro");
        } else {                                                                // Se lido com sucesso
          //digital[i].value = ler[1] - '0';                                         // salva o valor lido
          //print_sensor_to_console(digital[i].name, digital[i].value);           // Exibe informações lidas
        }
      }

      if (analogico.type != Analogic && digitalQtd < 1) {                       // Se não houver nenhum sensor
        printf("\nNão foram adicionados sensores. Ecerrando...\n");
        return 0;                                                               // Encerra o programa
      }
    }
}

/**
 * @brief Obtem o numero de entradas digitais disponíveis para
 * leitura
 *
 * @return int o numero de entradas digitais ou -1 em caso de erro
 */
int get_number_of_digital_ios() {
  unsigned char read[100];
  send_command(GET_NUMBER_OF_SENSORS, GET_NUMBER_OF_SENSORS);
  await(3000);
  //serialReadBytes(read);
  if (read[0] != NUMBER_OF_DIGITAL_PORTS || strlen(read) < 2) {
    return -1;
  }
  /** Numero de portas digitais disponíveis */
  return read[1] - '0';
}

/**
 * @brief Exibe no terminal o nome e o endereço de cada uma das
 * entradas digitais disponíveis.
 *
 * @param max_digital Numero maximo de entradas digitais. (valor
 * retornado pela função get_number_of_digital_ios)
 * @see get_number_of_digital_ios
 */
void print_io_name_and_id(int max_digital) {
  unsigned char read[100];
  char command[100];
  printf("Portas digitais disponíveis: %i\n\n", max_digital);
  printf("%10s: %8s\n", "Nome", "Endereço\n");
  for (int p = 1; p <= max_digital; p++) {
    send_command(GET_SENSOR_ADDRESS, p);                                  // Solicita o endereço do sensor
    await(3000);                                                          // Aguarda processamento da solicitação
    //serialReadBytes(read);                                                // faz a leitura

    /** Se problema com a node, encerra a leitura*/
    if (0) {                                                  // se houver erro
      printf("NodeMCU com problema!!!\n\n");
      clear_display();
      write_string("NodeMCU com erro");
      exit(0);                                                            // termina o programa
    }

    unsigned char sensor_address = read[1];                               // endereço do sensor

    /** Obtendo o nome */
    send_command(GET_SENSOR_NAME, sensor_address);                        // Solicita o nome do sensor
    await(3000);                                                          // Aguarda processamento da solicitação
    //serialReadBytes(read);                                                // Lê a resposta

    command[0] = read[1];
    command[1] = read[2];

    // exibe a informação
    printf("%10s: %2i\n", command, sensor_address);                       // Exibe os dados lidos
  }
}

/**
 * @brief Get the Substring object
 *
 * @param dst Destination variable
 * @param src String to get a substring
 * @param start Initial position of the substring
 * @param end Ending position of the substring
 * @return char* Substring
 */
char *getSubstring(char *dst, const char *src, size_t start, size_t end) {
    return strncpy(dst, src + start, end);
}

/**
 * @brief Separa um argumento lido em 3 substrings
 *
 * @param str string lida
 * @param a priemira substring de tamanho 10
 * @param b segunda substring de tamanho 10
 * @param c terceira substring de tamanho 10
 */
void separate_string_in_3(char *str, char a[10], char b[10], char c[10]) {
  char *ptr = strtok(str, ".");
  int index = 0;

  while(ptr != NULL && index < 3)
  {
    //printf("%s\n", ptr);
    //res[i] = ptr;
    //strcpy(res[index], ptr);
    if (index == 0) {
      for (int i = 0; i < strlen(ptr) && i < 10; i++) {
        //printf("%c", ptr[i]);
        a[i] = ptr[i];
      }
      a[10] = '\0';
    }
    if (index == 1) {
      for (int i = 0; i < strlen(ptr) && i < 10; i++) {
        //printf("%c", ptr[i]);
        b[i] = ptr[i];
      }
      b[10] = '\0';
    }
    if (index == 2) {
      for (int i = 0; i < strlen(ptr) && i < 10; i++) {
        //printf("%c", ptr[i]);
        c[i] = ptr[i];
      }
      c[10] = '\0';
    }
    //printf("\n");
    ptr = strtok(NULL, ".");
    index++;
  }
}

/**
 * @brief Adiciona um sensor digital a lista de sensores salvos
 *
 * @param sensor_info String (argumento recebido pelo programa) contendo
 * as informações do sensor digital
 * @param digital Ponteiro para uma estrutura onde as informações serão salvas
 * @return int Retorna 1 se sucesso, ou -1 se houver algum erro.
 */
int add_digital_sensor(char *sensor_info, Sensor *digital) {
  printf("Há um sensor na porta digital\n");
  char arr[3][11] = {"", "", ""};
  separate_string_in_3(sensor_info, arr[0], arr[1], arr[2]);

  /**
   * @brief Os parametros para adiconar um sensor digital não
   * podem ser vazios. O nome e o endereço são obrigatórios
   */
  if (strlen(arr[0]) == 0 || strlen(arr[1]) == 0 || strlen(arr[2]) == 0) {
      printf("\nComando ilegal!!!\n\n");
      //exit(0);
      return -1;
  }

  /**
   * O endereço do sensor nunca é zero
   */
  int address = atoi(arr[2]);
  if (address == 0) {
      printf("Endereço invalido para o sensor!!!\n");
      //exit(0);
      return -1;
  }

  /**
   * @brief Salvando o nome do sensor
   */
  for (int i = 0; i < strlen(arr[1]) && i < 10; i++) {
    //printf("%c", ptr[i]);
    digital->name[i] = arr[1][i];
  }
  int end = 10;
  if (strlen(arr[1]) < 10) {
    end = strlen(arr[1]);
  }
  digital->name[end] = '\0';
  //digital[0].name = arr[1];
  /**
   * @brief Definido as outras propriedades do sensor
   *
   */
  //digital->value = 0;
  struct queue_head *leituras = &(digital->values);
  leituras->max_lenght = 10;
  leituras->number_of_items = 0;
  push(&digital->values, 0);
  digital->id = atoi(arr[2]);
  digital->type = Digital;


  //printf("cmd: %s\n", digital[0].name);
  printf("Nome: %s\n", digital->name);
  printf("Endereço: %i\n", digital->id);

  /**
   * Limpando substrings
   */
  for (int i = 0; i < 11; i++) {
    arr[0][i] = '\0';
    arr[1][i] = '\0';
    arr[2][i] = '\0';
  }

  //printf("nome %s", arr[1]);
  return 1;
}


/*
int isPressed(int gpio_number){
  if(digitalRead(gpio_number) == 0) {
    await(100);
    if(digitalRead(gpio_number) == 0){
      printf("Pressed %d! \n", gpio_number);
      return 1;
    }
  }
  return 0;
}
*/