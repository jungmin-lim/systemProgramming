#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"
#include "varlib.h"

#define DFL_PROMPT ">"

void setup();
void fatal(char* , char* , int );

int main(int argc, char *argv[]){
	char *cmdline, *prompt, **arglist;
	int result, process(char** );
	void setup();

	prompt = DFL_PROMPT;
	setup();

	while((cmdline = next_cmd(prompt, stdin)) != NULL){
		if((arglist = splitline(cmdline)) != NULL){
			result = process(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}

	return 0;
}

void setup(){
	extern char** environ;
	VLenviron2table(environ);

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n){
	fprintf(stderr, "Error: %s, %s\n", s1, s2);
	exit(n);
}
