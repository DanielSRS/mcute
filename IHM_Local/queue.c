#include <stdio.h> // just for NULL and printf
#include <stdlib.h> // malock e free
/**
 * @brief It is actualy a reverse queue
 * 
 */

struct queue_head
{
  int number_of_items;
  int max_lenght;
  struct node * first_item;
  struct node * last_item;
};

struct node
{
  int value;
  struct node * next;
};


int push(struct queue_head *queue, int val) {
  if (queue == NULL) {
    return -1;
  }
  // se não houver nada na fila
  if (queue->number_of_items == 0) {
    // aloca o espaço nescessario
    struct node * new_node = (struct node*) malloc(sizeof (struct node));
    new_node->value = val;
    new_node->next = NULL;

    queue->number_of_items = 1;
    queue->first_item = new_node;
    queue->last_item = new_node;

    return 1;
  }

  else if (queue->number_of_items < queue->max_lenght) {
    // aloca o espaço nescessario
    struct node * new_node = (struct node*) malloc(sizeof (struct node));
    new_node->value = val;
    new_node->next = queue->first_item;

    queue->number_of_items = queue->number_of_items + 1;
    queue->first_item = new_node;
    return 1;
  }
  return 0; // não pode ser inserido 
}

int pop(struct queue_head *queue) {
  if (queue == NULL) {
    return -1;
  }
  if (queue->number_of_items == 0) {
    return -1;
  }

  if (queue->number_of_items == 1) {
    queue->number_of_items = 0;
    free(queue->first_item);
    queue->first_item = NULL;
    queue->last_item = NULL;
  }

  if (queue->number_of_items >= 2) {
    // decrementa a quantidade de itens
    queue->number_of_items = queue->number_of_items - 1;

    // ponteiro para percorrer os itens
    struct node * current_item = queue->first_item;

    // Enocontre o penultimo item
    while (current_item->next->next != NULL)
    {
      current_item = current_item->next;
    }

    // libera a memoria alocada
    free(current_item->next);
    current_item->next = NULL;

    if (queue->number_of_items == 1) {
      queue->last_item = queue->first_item;
    }
  }
}

int print_queue(struct queue_head *queue) {
  if (queue == NULL) {
    printf("Lista inválida!!\n");
    return  0;
  }
  printf("{\n");
  printf("\tnumber_of_items: %i,\n", queue->number_of_items);
  printf("\tmax_lenght: %i,\n", queue->max_lenght);
  printf("\tvalues: [");

  struct node * current_item = queue->first_item;

  // iterando
  while (current_item != NULL && queue->number_of_items > 0)
  {
    printf(" %i", current_item->value);
    current_item = current_item->next;
    if (current_item != NULL) {
        printf(",");
    }
  }
  printf(" ],\n");
  printf("}\n");
  return 0;
}

int peek_value(struct queue_head *queue) {
  if (queue == NULL) {
    printf("endereço invalod da lista\n");
    return -1;
  }
  //printf("lista de tamanho: %i\n", queue->number_of_items);
  if (queue->number_of_items <= 0) {
    printf("sem itens na lista\n");
    return -1;
  }
  //printf("retornando valor: %i\n", queue->first_item->value);
  return queue->first_item->value;
}
