#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "queue.h"

#define MAXSCHQUEUE 10
#define arrlen(arr) sizeof(arr)/sizeof(arr[0])

typedef enum {FCFS=0, RR, MFQ, SJF}schtype_t;
typedef enum {ODR_ASC=0, ODR_DES}porder_t;

struct schreqnode {
        int reqid;
        int reqtype;
        struct schreqnode *next;
};

struct schqueue {
        struct schreqnode *head;
        struct schreqnode *tail;
};

struct schdetail {
        int schtype;
        int queuecount;
        int schtypechnage;
        int timequantum[MAXSCHQUEUE];
};

void schreqenqueue(int item, int type, struct schqueue *q);
int schreqdequeue(struct schqueue *q);
void schreqdelete(struct schqueue *q, int key);
int check_sch_argnum(char *schtype, char *queuenum,int argnum);
int set_default_scheduler(struct schdetail *d);
int set_sch_type(char input[][30], struct schdetail *d);
void fcfs(char *input,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended);
void setchilddead(int dpid);
void loadrrqueue(char *input, struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended);
void rr(char input[][30], int argnum,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended);
void mfq(char input[][30], int argnum,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended);
int compare(const void *a, const void *b);
void sjf(char input[][30], int argnum,struct queue *pid_list, struct schdetail *d,int *fg_pid,int *fg_suspended);
int schedule_task(char input[][30], int argnum, struct queue *pid_list, struct schdetail *d, int *fg_pid, int *fg_suspended);

#endif /* SCHEDULER_H_ */



