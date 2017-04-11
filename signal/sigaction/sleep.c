#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void fun(int sig){
}


int mysleep(int sec) {
	/*
	   struct  sigaction {
	   union __sigaction_u __sigaction_u; 
	   sigset_t sa_mask;              
	   int     sa_flags;             
	   };

	   union __sigaction_u {
	   void    (*__sa_handler)(int);
	   void    (*__sa_sigaction)(int, siginfo_t *,
	   void *);
	   };
	   */


	struct sigaction new, old;
	new.sa_flags = 0;
	new.sa_handler = fun;
	sigemptyset(&new.sa_mask);
	sigaction(SIGALRM, &new, &old);

	alarm(sec);
	pause();

	sigaction(SIGALRM, &old, NULL);
	return alarm(0);
}


int main(int argc, char* argv[]) {

	while (1) {
		int remain = mysleep(2);
		printf("%d \n", remain);
	}
	return 0;
}
