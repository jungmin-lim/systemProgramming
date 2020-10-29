// 2017116186 jungmin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#define BUFSIZE 2048

void f(int, siginfo_t*, void*);

int main(int argc, char *argv[]){
	struct sigaction handler;
	struct termios ttystate;
	char buf[BUFSIZE];

	handler.sa_sigaction = f;
	handler.sa_flags = SA_RESETHAND | SA_SIGINFO;

	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ECHO;
	tcsetattr(0,TCSANOW, &ttystate);

	if(sigaction(SIGINT, &handler, NULL) == -1){
		perror("sigaction");
	}
	else{
		while(1){
			fgets(buf, BUFSIZE, stdin);
			printf("input: %s\n", buf);
		}
	}

	return 0;
}

void f(int s, siginfo_t *siginfo, void* context){
	printf("Signal number %d\n", siginfo->si_signo);
	printf("Sending PID %d\n", siginfo->si_pid);
	printf("Done handling signal %d\n", s);
	kill(siginfo->si_pid, SIGQUIT);
}
