#ifndef QUEUE_H_
#define QUEUE_H_

struct node {				
       int pid;
	char *name;
       struct node *next;
	
};


struct queue {				
       struct node *head;
       struct node *tail ;
};



void enqueue(int item, char *s, struct queue *q);
int dequeue(struct queue *q);
void delete(struct queue *q, int key);

#endif /* QUEUE_H_ */
