#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/resource.h>

void cont (int sig_num) {
     printf("recived the continue signal\n");
}

void stop (int sig_num) {
	signal(SIGTSTP,stop);
    pause();
}

void contprocess(int sig_num){
	signal(SIGCONT,contprocess);
}

void pauseprosess(int sig_num){
	printf("recived the pausing signal\n");
	signal(SIGUSR1,pauseprosess);
	pause();
}


int main (int argc, char *argv[]) {


	int i,num,sltime;

	signal(SIGQUIT,cont);
	signal(SIGTSTP,stop);
	signal(SIGUSR1,pauseprosess);
	signal(SIGCONT,contprocess);
	//setpriority(PRIO_PROCESS,0,-20);
	pause();
	
	num =atoi(argv[1]);
	sltime = 1000*atoi(argv[2]);
	//pause();
	
	for (i=1; i<=num; i++){
		printf("This is program %s and it prints for the %d time of %d...\n",argv[0],i,num);
		usleep(sltime);
	}
}
