#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

     
void enqueue(int item, char *s, struct queue *q) {		
     struct node *p;						

     p= (struct node *)malloc(sizeof(struct node));
     p->pid=item;
     p->name=s;	
     p->next=NULL;
     if (q->head==NULL) q->head=q->tail=p;
     else {
          q->tail->next=p;
          q->tail=p;
     }

}


int dequeue(struct queue *q, struct dequeuedet *det) {		
    int item;				
    struct node *p;
    
    det->pid=q->head->pid;
    det->name=q->head->name;
    item=q->head->pid;
    p=q->head;
    q->head=q->head->next;
    free(p);
    return item;
}


void delete(struct queue *q, int key)
{
     if (q->head->pid == key)
     {
        struct node *p = q->head;
        q->head = q->head->next;
        free(p);
        return;
    }
    struct node *current = q->head->next;
    struct node *previous = q->head;
    while (current != NULL && previous != NULL)
    {
      if (current->pid == key)
      {
        struct node *tmp = current;
        if (current == q->tail)
          q->tail = previous;
        previous->next = current->next;
        free(tmp);
        return;
      }
      previous = current;
      current = current->next;
    }
    return;  
  }
