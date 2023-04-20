# ECE536-Activity2
Unix shell for schedule task with different scheduling policies

# Summery


The most critical component of an operating system is the scheduler and the shell. The shell gives us the ability to launch, execute and stop applications at any time. In other words, a shell can be considered as the interface for the user. This activity implements a basic shell with different scheduling polices that user can configure.

# Makefile configurations
 
  * There are two different configurations of the make file 
  ```
        * compileshell
            compile the shell and the demo process that can used to test the scheduling
        * clean
            clear all the build files
  ``` 

# Build Information
* Run Unix terminal and navigate the file location.
* Use the "make clean" command to clean the object files.
* Use the "make complileshell" command to build the shell.
* Run the "shell.o" file by using the command ./shell
* Note that time quantum argument is removed except from MFQ and RR since this is the only scheduling scheme depend on the time quantum.
* Use "help" command to investigate the commands inside the shell.
* Example command for switch between every scheduling scheme is provided.
* Run the following command in the shell to change the scheduling schemes.
  ```
        * For FCFS scheduling
            ./sch fcfs
        * For RR scheduling with time quantum of 20
            ./sch rr 20 
        * For SJF scheduling
            ./sch sjf
        * For MFQ scheduling with 3 queues and corresponding time quantum’s
            ./sch mfq 3 10 20 40
  ``` 
* Note that the MFQ scheduling is supported for maximally 10 queues
