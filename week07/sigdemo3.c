#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int count = 0;
void f(int signum){
    fprintf(stdout, "Currently ellapsed time: %d sec(s)\n", count);
}
int main(){
    signal(SIGINT, f);
    printf("you can't stop me!\n");
    while(1){
        sleep(1);
        count++;
        printf("haha\n");
    }

    return 0;
}
