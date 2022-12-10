#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum help_type {
  General,
  AddDigital,
  AddAnalogic,
  ListPorts,
};

void generalHelper(void);
void help(char *type);

/**
 * @brief Exibe mensagem de ajuda
 * 
 * @param type Tipo da mensagem de ajuda exibido
 */
void help(char *type) {
  if ((strcmp(type, "general") == 0)) {
    generalHelper();
  }
}

void generalHelper() {
  printf("\n\n");
  printf("Lista de comandos válidos:\n\n");
  printf("\t%s:\t %31sAdiciona o sensor analogico\n\n", "-analogic", " ");
  printf("\t%40s:\tAdiciona um sensor digital.", "-d.<nome do sensor>.<endereço do sensor>");
  printf(" O nome e o endereço do sensor são obrigatórios, ");
  printf("onde <endereço do sensor> é uma cadeia de caracteres envolta de aspas duplas:\n\n");
  printf("\t\t Exeplo de nome: \"Sensor exemplo\"\n\n");
  printf("\t\t Exeplo de uso do comando: -d.\"D0\".16\n\n");

  printf("\t-l:\t %38sLista os entradas digitais disponíveis\n\n", " ");
  printf("\t-h:\t %38sExibe este menu\n\n", " ");
  printf("\t-m:\t %38sUtilização manual\n\n", " ");
}

