#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include "scheduler.h"
#include "queue.h"

int defschtype = FCFS;	//default scheduling type
int schchangereq = 0;   //how many times scheduler change request to serve(0 is default)
#define ASCENDING 1
#define DESCENDING -1

void schreqenqueue(int item, int type, struct schqueue *q) {		
     struct schreqnode *p;						

     p= (struct schreqnode *)malloc(sizeof(struct schreqnode));
     p->reqid=item;
     p->reqtype=type;	
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

int set_default_scheduler(struct schdetail *d) {
        d->schtype = defschtype;
        d->queuecount = 0;
        d->schtypechnage = 1;
        memset(d->timequantum,0,MAXSCHQUEUE);
        return 1;
}

int set_sch_type(char input[][30], struct schdetail *d) {
        int i;
        int status = -1;

        if(strcmp(input[1],"fcfs")==0) {
                d->schtype = FCFS;
                d->queuecount = 0;
                d->schtypechnage = 1;
                printf("test pass\ntype : %d\nqueue count : %d\n",d->schtype,d->queuecount);
                status = 1;
        }
        else if(strcmp(input[1],"rr")==0){
                d->schtype = RR;
                d->queuecount = 1;
                d->schtypechnage = 1;
                d->timequantum[0] = atoi(input[2]);
                printf("test pass\ntype : %d\nqueue count : %d\ntimequntom : %d\n",d->schtype,d->queuecount,d->timequantum[0]);
                status = 1;
        }
        else if(strcmp(input[1],"mfq")==0){
                d->schtype = MFQ;
                d->queuecount = atoi(input[2]);
                d->schtypechnage = 1;
                memset(d->timequantum,0,MAXSCHQUEUE);
                for(i=0;i<d->queuecount;i++){
                        d->timequantum[i] = atoi(input[i+3]);
                }
                printf("test pass\ntype : %d\nqueue count : %d",d->schtype,d->queuecount);
                for(i=0; i < d->queuecount; i++)
                        printf("timequntom : %d\n",d->timequantum[i]);
                status = 1;
        }
        else if(strcmp(input[1],"sjf")==0){
                d->schtype = SJF;
                d->queuecount = 0;
                d->schtypechnage = 1;
                printf("test pass\ntype : %d\nqueue count : %d\n",d->schtype,d->queuecount);
                status = 1;
        }
        else {
                status = -1;
        }
        status = 1;
        return status;
}

void fcfs(char *input, struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended)
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

void rr(char input[][30], int argnum,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended)
{

}

void mfq(char input[][30], int argnum,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended)
{

}

int compare(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    int x1, y1, x2, y2;

    if (strncmp(s1, "exec", 4) == 0) {
        return 1; // skip sorting the "ignore" string
    } else if (strncmp(s2, "exec", 4) == 0) {
        return -1; // skip sorting the "ignore" string
    } else {
        sscanf(s1, "p(%d,%d", &x1, &y1);
        sscanf(s2, "p(%d,%d", &x2, &y2);

        int result = x1 * y1 - x2 * y2;

        if (result > 0) {
            return 1;
        } else if (result < 0) {
            return -1;
        } else {
            return 0;
        }
    }
}

void sjf(char input[][30], int argnum,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended)
{
        int order = ODR_ASC; // set order to ASCENDING or DESCENDING
        int i;
        char *ptrarr[argnum + 1];

        for (i = 0; i < argnum + 1; i++) {
                ptrarr[i] = input[i];
        }

        qsort(ptrarr + 1, argnum, sizeof(char *), compare);
        printf("Number of arguments: %d\n", argnum);

        if (order == ODR_DES) {
                int i, j;
                char *temp;

                for (i = 1, j = argnum; i < j; i++, j--) {
                temp = ptrarr[i];
                ptrarr[i] = ptrarr[j];
                ptrarr[j] = temp;
                }
        }

        for (i = 1; i < argnum + 1; i++) {
                printf("%s\n", ptrarr[i]);
                fcfs(ptrarr[i],pid_list,d,fg_pid,fg_suspended);
        }
}

int schedule_task(char input[][30], int argnum, struct queue *pid_list, struct schdetail *d, int *fg_pid, int *fg_suspended)
{
        int i;
        switch (d->schtype)
        {
                case FCFS:
                        for(i=1; i<=argnum; i++)
                        {
                                printf("pocess name: %s\n", input[i]);
                                fcfs(input[i],pid_list,d,fg_pid,fg_suspended);
                        }
                        break;
                case RR:
                        rr(input,argnum,pid_list,d,fg_pid,fg_suspended);
                        break;
                case MFQ:
                        mfq(input,argnum,pid_list,d,fg_pid,fg_suspended);
                        break;
                case SJF:
                        sjf(input,argnum,pid_list,d,fg_pid,fg_suspended);
                        break;
                default:
                        printf("Invalid scheduler type\n");
                        break;
        }
}
