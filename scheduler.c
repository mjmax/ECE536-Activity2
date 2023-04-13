#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include "scheduler.h"
#include "queue.h"

int schtype = FCFS;	//default scheduling type

void schreqenqueue(int item, int type, int pcount, struct schqueue *q) {		
     struct schreqnode *p;						

     p= (struct schreqnode *)malloc(sizeof(struct schreqnode));
     p->reqid=item;
     p->reqtype=type;
     p->pcount=pcount;	
     p->next=NULL;
     if (q->head==NULL) q->head=q->tail=p;
     else {
          q->tail->next=p;
          q->tail=p;
     }

}


int schreqdequeue(struct schqueue *q) {						
    struct schreqnode *p;
    int d;
    
    d= q->head->reqid;   
    p=q->head;
    q->head=q->head->next;
    free(p);
    return d;
}


void schreqdelete(struct schqueue *q, int key) {
     if (q->head->reqid == key)
     {
        struct schreqnode *p = q->head;
        q->head = q->head->next;
        free(p);
        return;
    }
    struct schreqnode *current = q->head->next;
    struct schreqnode *previous = q->head;
    while (current != NULL && previous != NULL)
    {
      if (current->reqid == key)
      {
        struct schreqnode *tmp = current;
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

int check_sch_argnum(char *schtype, char *queuenum,int argnum) {
        int status = -1;
        //printf("input type: %s", schtype);
        if(strcmp(schtype,"fcfs")==0) {
                status =(argnum==1)?1:-1;
        }
        else if(strcmp(schtype,"rr")==0){
                status =(argnum==2)?1:-1;
        }
        else if(strcmp(schtype,"mfq")==0){
                status =(atoi(queuenum)==(argnum-2))?1:-1;
        }
        else if(strcmp(schtype,"sjf")==0){
                status =(argnum==1)?1:-1;

        }
        else {
                status = -1;
        }
        return status;
}

int set_sch_type(char const *input[]) {
        int status = -1;
        printf("\ntest pass\n");
        if(strcmp(input[0],"fcfs")==0) {
                schtype = FCFS;
                status = 1;
        }
        else if(strcmp(input[0],"rr")==0){
                schtype = RR;
                status = 1;
        }
        else if(strcmp(input[0],"mfq")==0){
                schtype = MFQ;
                status = 1;
        }
        else if(strcmp(input[0],"sjf")==0){
                schtype = SJF;
                status = 1;
        }
        else {
                status = -1;
        }
        status = 1;
        return status;
}

int get_sch_type(void) {
        return schtype;
}

int schedule_task(char *input, struct queue *pid_list, int *fg_pid, int *fg_suspended)
{
        int i,t,status;
	char *args[10];
	char *temp;
	struct node *p;

	for (i = 0; i < 10; i++)
	{
		args[i]=(char *)malloc(10*sizeof(char));
	}

	strcpy(args[0],strtok(input,"(,"));
	for (i=1; (temp=strtok(NULL,",)"))!=NULL; i++) 
		strcpy(args[i],temp);
	printf("\n");
        if (strcmp(args[i-1],"&")==0)
	{
		args[i-1]=NULL;
	}
	else
		args[i]=NULL;
	if ((t=fork())==0)
	{
		execv(args[0],args);
	}
	enqueue(t,args[0],pid_list);
	if (args[i-1]!=NULL)
	{
		*fg_pid=t;
		while((*fg_pid)!=0 && (*fg_suspended)!=1)
			pause();
	}
}
