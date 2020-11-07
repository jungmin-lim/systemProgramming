#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

#define DELAY 2

void child_proc();
void child_child_proc(char);
void sigint_handler(int);
void sigquit_handler(int);

int main(int argc, char* argv[]){
	int new_pid, wait_rv, child_status;
	void parent_proc(), child_proc();

	if((new_pid = fork()) == -1){
		perror("fork");
	}
	else if(new_pid == 0){
		child_proc();
	}
	else{
		printf("I am Parent %d.\n", getpid());
		wait_rv = wait(&child_status);
		printf("\texit code = %d\n", child_status >> 8);
	}
	return 0;
}

void child_proc(){
	char c;
	int new_child_pid;
	int wait_child_rv, child_child_status;
	int exitcode;
	void chil_child_proc(char);
	void sigint_handler(int), sigquit_handler(int);

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);

	printf("\tI am Child %d.\n", getpid());
	while(1){
		printf("\tInput symbol(+ - * /) :");
		fflush(stdout);

		scanf(" %c", &c);

		if((new_child_pid = fork()) == -1){
			perror("fork");
		}
		else if(new_child_pid == 0){
			child_child_proc(c);
		}
		else{
			wait_child_rv = wait(&child_child_status);
			exitcode = child_child_status >> 8;
			printf("\t\texit code = %d\n", exitcode);
			if(exitcode == 255){
				printf("\tChild %d dead. :(\n", getpid());
				exit(17);
			}
		}
	}

}

void child_child_proc(char operator){
	double op1, op2;
	double ret;

	printf("\t\tI am Child's child %d.\n", getpid());
	printf("\t\tInput two number(x and y) :");
	fflush(stdout);

	scanf("%lf %lf", &op1, &op2);

	if(op1 == op2){
		printf("\t\tThe program received the same numbers. So the progam exits\n");
		printf("\t\t\tChild %d dead. :(\n", getpid());
		exit(-1);
	}
	fflush(stdin);

	switch(operator){
		case '+':
			printf("\t\t%lf + %lf = %.1lf\n", op1, op2, op1+op2);	
			printf("\t\tChild %d dead. :)\n", getpid());
			exit(16);
			break;
		case '-':
			printf("\t\t%lf - %lf = %.1lf\n", op1, op2, op1-op2);	
			printf("\t\tChild %d dead. :)\n", getpid());
			exit(16);
			break;
		case '*':
			printf("\t\t%lf * %lf = %.1lf\n", op1, op2, op1*op2);	
			printf("\t\tChild %d dead. :)\n", getpid());
			exit(16);
			break;
		case '/':
			printf("\t\t%lf / %lf = %.1lf\n", op1, op2, op1/op2);	
			printf("\t\tChild %d dead. :)\n", getpid());
			exit(16);
			break;
		default :
			printf("\t\tThe program received the not correct symbol. So the program exits.\n");
			printf("\t\t\tChild %d dead. :(\n", getpid());
			exit(255);
	}

	return;
}

void sigint_handler(int s){
	exit(-1);
}

void sigquit_handler(int s){
	exit(-1);
}
