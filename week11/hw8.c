#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include 	<sys/wait.h>
#include	"smsh.h"

#define	DFL_PROMPT	"> "

void func(int signum){
	wait(NULL);
}

int main(){
	char	*cmdline, *prompt, **arglist;
	int	result, isBackground, pid;
	void	setup();

	prompt = DFL_PROMPT ;
	setup();

	while ( (cmdline = next_cmd(prompt, stdin, &isBackground)) != NULL ){
		if ( (arglist = splitline(cmdline)) != NULL  ){
			if(isBackground){
				pid = fork();
				if(pid == 0){
					// execute command on child
					printf("PID : %d\n", getpid());
					result = execute(arglist);
					freelist(arglist);
					exit(1);
				}
				else{
					signal(SIGCHLD, func);
					freelist(arglist);
				}
			}
			else{
				result = execute(arglist);
				freelist(arglist);
			}
		}
		free(cmdline);
	}
	return 0;
}

void setup(){
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n){
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}

