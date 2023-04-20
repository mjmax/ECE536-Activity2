#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/resource.h>
#include "queue.h"
#include "scheduler.h"

#define SHELL_VER "1.00" 

int fg_pid = 0;
int fg_suspended = 0;
int run = 1;
struct queue pid_list;
struct schdetail schd;

void help() {
	printf("This is manual page\n");
	printf("This shell supports the following commands:\n");
	printf("\tver\n\texec\n\tps\n\tkill\n\tsch\n\thelp\n\texit\n");
	printf("For more details please type 'help command'\n");
}

void helpcmd(char *cmd) {
	printf("This is manual page\n\n");
	if (strcmp(cmd,"ver")==0)
	{
		printf("\nver:\tShows details about the shell version\n");
	}
	else if (strcmp(cmd,"exec")==0)
	{
		printf("\nexec p1(n1,qt1) p2(n2,qt2) ...:\nExecutes the programs p1, p2 ...\nEach program types a message for n times and it is given a time quantum of qt msec.\n");
		printf("If parameter (&) is given the program will be executed in the background\n");
	}
	else if (strcmp(cmd,"ps")==0)
	{
		printf("\nps:\tShows the living process with the given pid\n");
	}
	else if (strcmp(cmd,"kill")==0)
	{
		printf("\nkill pid:\tEnds the process with the given pid\n");
	}
    else if(strcmp(cmd,"sch")==0){
        printf("\nsch type queue tomentum \nChanges the scheduling policy of the processes run in the OS\nScheduling policies supported\n");
        printf("\tFirst Come First Serve(FCFS)(default)\n\tRound Robbin(RR)\n\tMulti Level Feedback Queue(MFQ)\n\tShortest Job First(SJF)\n");
        printf("\nFollowing is the prototype command for every scheduling type\n");
        printf(" (FCFS)\t=>sch fcfs\n (RR)\t=>sch rr timeqt\n (MFQ)\t=>sch mfq queue timeqt1 timeqt2 timeqt3 ...\n (SJF)\t=>sch sjf\n");
    }
	else if (strcmp(cmd,"help")==0)
	{
		printf("\nhelp:\tYou should know this command by now\n");
	}
	else if (strcmp(cmd,"exit")==0)
	{
		printf("\nexit:\tEnds the experience of working in the new shell\n");
	}
	else 
	{
		printf("\nNo Such command. Type help to see a list of commands\n");
	}
}

void ver() {
	printf("\nNew Shell. Works properly!!\n");
}

void ps() {
	struct node *p;
	printf("\nNEW SHELL presents the following living processes\n");
	printf("\tPID\tNAME\n");
	for (p=pid_list.head; p!=NULL; p=p->next)
	{
		printf("\t%d\t%s\n",p->pid,p->name);
	}
}

void mykill(int pid) {
	kill(pid,SIGTERM);
	printf("You have just killed process %d\n",pid );
}


void exec(char input[][30],int argnum) {
	schedule_task(input,argnum,&pid_list,&schd,&fg_pid,&fg_suspended);
}


void myexit() {
	char yesno;
	if (pid_list.head==pid_list.tail)
	{
		run=0;
	}
	else {
		printf("There are still living processes Do you want to kill them? (y/n): ");
		yesno=getchar();
		if (yesno == 'y')
		{
			while(pid_list.head!=pid_list.tail)
			{
				mykill(pid_list.tail->pid);
				usleep(10000);
			}
			run=0;
		}
	}
}

void childdead(int signum) {
	int dead_pid, status;
	
	dead_pid=wait(&status);
	printf("The child %d is dead\n",dead_pid );
	setchilddead(dead_pid);

	delete(&pid_list,dead_pid);
	printf("\n");
	if (dead_pid==fg_pid)
	{
		fg_pid=0;
	}
}

void susp (int signum) {
	fg_suspended=1;
	printf("All processes supspended\n");
}

void cont (int signum) {
	fg_suspended=0;
	printf("Waking all processes...\n");
	while (fg_pid!=0 && fg_suspended!=1)
		pause();
}

int main(int argc, char const *argv[]) {
	char input[15][30];
	int argnum, i;

	pid_list.head=NULL;
	pid_list.tail=NULL;
	schd.schtype=-1;
	//signal(SIGCHLD, term_child);
	enqueue(getppid(),"NEW SHELL",&pid_list);
	set_default_scheduler(&schd);

	signal(SIGCHLD,childdead);
	signal(SIGTSTP,susp);
	signal(SIGQUIT,cont);
	setpriority(PRIO_PROCESS,0,-20);

	ver();

	while (run){
		printf("=>");
		for (argnum=0; (scanf("%s",&input[argnum]))==1;argnum++)
			if (getchar()=='\n') break;
		if (strcmp(input[0],"ver")==0 && argnum==0) ver();
		else if (strcmp(input[0],"help")==0 && argnum==0) help();
		else if (strcmp(input[0],"help")==0 && argnum==1) helpcmd(input[argnum]);
		else if (strcmp(input[0],"ps")==0 && argnum==0) ps();
		else if (strcmp(input[0],"kill")==0 && argnum==1) mykill(atoi(input[1]));
		else if (strcmp(input[0],"sch")==0 && (check_sch_argnum(input[1],input[2],argnum)>0)) set_sch_type(input,&schd);
		else if (strcmp(input[0],"exec")==0 && argnum!=0) exec(input,argnum);
		else if (strcmp(input[0],"exit")==0 && argnum==0) myexit();
	    else printf("No such command or check input arguments. Check help for more detail(help).\n");
	}

}
