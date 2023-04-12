#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>


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

int set_scheduling(char const *input[]) {
        int status = -1;
        printf("\ntest pass\n");
        status = 1;
        return status;
}
