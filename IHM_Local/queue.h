/**
 * @brief It is actualy a reverse queue
 * 
 */

#ifndef QUEUE_H
#define QUEUE_H

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

int push(struct queue_head *queue, int val);
int pop(struct queue_head *queue);
int print_queue(struct queue_head *queue);
int peek_value(struct queue_head *queue);

#endif