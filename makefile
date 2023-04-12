compileshell: p-shell.c shell.c queue.h scheduler.c scheduler.h
	gcc p-shell.c -o p
	gcc shell.c scheduler.c -o shell

clean: p shell
	rm p shell	
