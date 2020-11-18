#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<signal.h>
#include	"hw8_smsh.h"

#define	DFL_PROMPT	"> "

int main()
{
	char	*cmdline, *prompt, **arglist;
	int	result, isBackground, c_pid = 0;
	void	setup();

	prompt = DFL_PROMPT ;
	setup();

	while ( (cmdline = next_cmd(prompt, stdin, &isBackground)) != NULL ){
		if ( (arglist = splitline(cmdline)) != NULL  ){
			if(!strcmp(arglist[0], "exit")){
				arglist = NULL;
				kill(0, 2);
				return 0;
			}

			if(isBackground){
				result = execute_background(arglist, &c_pid);
			}
			else{
				result = execute(arglist);
			}
			freelist(arglist);
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

