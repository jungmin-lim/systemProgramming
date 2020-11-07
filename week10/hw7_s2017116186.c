#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define DELAY 2

void parent_proc();
void child_proc();
void child_child_proc(char);

int main(int argc, char* argv[]){
	int newpid;
	void parent_proc(), child_proc(), child_child_proc(char);

	if((newpid = fork()) == -1){
		perror("fork");
	}
	else if(newpid = 0){
		child_proc();
	}
	else{
		parent_prco();
	}
	return 0;
}

void parent_proc(){
	int wait_rv;

	wait_rv = wait(NULL);
	
}
void child_proc(){
	char c;

	printf("\tIam Child %d\n", getpid());
	printf("Input symbol(+ - * /) :");
	fflush(stdout);

	c = getchar();

}
