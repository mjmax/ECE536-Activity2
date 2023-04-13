#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "queue.h"

#define arrlen(arr) sizeof(arr)/sizeof(arr[0])

typedef enum {FCFS=0, RR, MFQ, SJF}schtype_t;

struct schreqnode {
        int reqid;
        int reqtype;
        int pcount;
        struct schreqnode *next;
};

struct schqueue {
        struct schreqnode *head;
        struct schreqnode *tail;
};

void schreqenqueue(int item, int type, int pcount, struct schqueue *q);
int schreqdequeue(struct schqueue *q);
void schreqdelete(struct schqueue *q, int key);
int check_sch_argnum(char *schtype, char *queuenum,int argnum);
int set_sch_type(char const *input[]);
int get_sch_type(void);
int schedule_task(char *input, struct queue *pid_list, int *fg_pid, int *fg_suspended);

#endif /* SCHEDULER_H_ */



