#include <stdio.h> // printf
#include <termios.h> // struct termios, 
#include "terminal.h" // enableRawMode, disableRawMode
#include "menu.h" // menu_item
#include "utils.h" // isSelected, read_input_char, decrement_in_range, increment_in_range

/**
 * @brief Printa na tela um entrada (item) de menu
 * 
 * @param item Item do menu
 * @param isSelected Se o item está selecionado. se 1
 * exibe um indicador de seleção o item do menu
 */
void menu_entrie(menu_item item, int isSelected) {
  char indicator = isSelected ? '*' : ' ';
  printf("(%c) %s\n", indicator, item.item_name);
}

/**
 * @brief Printa em tela todos os itens de um menu
 * 
 * @param items Lista de itens (entradas) de um menu
 * @param selected_item_index index do item do menu selecionado
 */
void render_menu_items(menu_item items[], int items_lenght, int selected_item_index) {
  for (int index = 0; index < items_lenght; index++) {
    int selected = isSelected(selected_item_index, index);
    menu_entrie(items[index], selected);
  }
}

/**
 * @brief Printa na tela um menu (modo texto) levemente interativo, permitido 
 * selecionar uma doas opções do menu
 * 
 * @param menu_entries Entradas do menu (items que serão exibidos)
 * @param entries_lenght Quantidade de entradas de menu
 * @param header_text Texto de cabeçalho, exibido antes das entradas de menu
 * @param footer_text Texto de rodapé, exibido depois das entradas de menu
 * @param notice_text Texto de notificação. exibido abaixo do rodape e antes
 * de uma interação. apos qualquer interação (como alterar a opção selecionada)
 * esse texto não deve ser exibido
 * @return int id da opção selecionada
 */
int menu(menu_item menu_entries[], int entries_lenght, char * header_text, char * footer_text, char * notice_text) {
  int min_index = 0;
  int max_index = entries_lenght - 1;
  int selected = min_index;                                             // opção selecionada: primeiro item do menu
  struct termios orig_termios = enableRawMode();
  char input;

  printf("\e[1;1H\e[2J");                                               // Limpa a tela
  printf("%s\n\n", header_text);                                        // printa o header
  render_menu_items(menu_entries, entries_lenght, selected);            // renderiza pela primeira vez os itens do menu
  printf("\n%s\n", footer_text);                                        // printa o footer
  printf("%s\n", notice_text);                                          // e o texto de notificação apena na primeira vez

  while (read_input_char( & input) == 1) {                              // quando houver um input do teclado

    if (input == 27) {                                                  // verifica se lido um cactere de escape
      if (read_input_char( & input) == 1 && input == 91) {              // se foi possivelmente pressionado uma das setas

        if (read_input_char( & input) == 1 && input == 65) {            // se pressionado seta para cima
          selected = decrement_in_range(selected, min_index);           // decrementa o index e sobe na lista
          printf("\e[1;1H\e[2J");
          printf("%s\n\n", header_text);
          render_menu_items(menu_entries, entries_lenght, selected);    // atuliza opção selecionada
          printf("\n%s\n", footer_text);
        } else if (input == 66) {                                       // se pressionado seta para baixo
          selected = increment_in_range(selected, max_index);           // incrementa a selação e desce na lista
          printf("\e[1;1H\e[2J");
          printf("%s\n\n", header_text);
          render_menu_items(menu_entries, entries_lenght, selected);    // atuliza opção selecionada
          printf("\n%s\n", footer_text);
        }
      }
    } else if (input == 10) {                                           // se precionado enter
      disableRawMode(orig_termios);
      return menu_entries[selected].id;                                                  // retorna a opção selecionada
    }
  }

}