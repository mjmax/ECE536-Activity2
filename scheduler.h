#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "queue.h"

#define arrlen(arr) sizeof(arr)/sizeof(arr[0])

typedef enum {FCFS=0, RR, MFQ, SJF}schtype_t;

int check_sch_argnum(char *schtype, char *queuenum,int argnum);
int set_sch_type(char const *input[]);
int get_sch_type(void);
int schedule_task(char *input, struct queue *pid_list, int *fg_pid, int *fg_suspended);

#endif /* SCHEDULER_H_ */



