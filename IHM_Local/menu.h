#ifndef MENU_H
#define MENU_H

/**
 * @brief Inforamções de um item de menu
 * como nome (exibido) e id (para identificação unica)
 */
struct item_menu {
  char * item_name;
  int id;
};

typedef struct item_menu menu_item;

void menu_entrie(menu_item item, int isSelected);
void render_menu_items(menu_item items[], int items_lenght, int selected_item_index);
int menu(menu_item menu_entries[], int entries_lenght, char * header_text, char * footer_text, char * notice_text);

#endif
