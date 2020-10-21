// 2017116186 jungmin

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>

int i, j = 0, interrupted = 0;
int get_response(char *question){
    int input;
    printf("%s (y/n)?", question);
    while(1){
        input = getchar();
        switch(input){
            case 'y':
            case 'Y':    return 0;
            case 'n':
            case 'N':                   
            case EOF: return 1;
            default :
                printf("\n\t\tcannot understand %c, Please type y or n. :(\n", input);
        }
    }
}

int set_crmode(){
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

int tty_mode(int how){
    static struct termios original_mode;

    if(!how) return tcgetattr(0, &original_mode);
    else return tcsetattr(0, TCSANOW, &original_mode);
}

void ctrl_c_interrupted(int signum){
    int response;

    signal(SIGINT, SIG_IGN);
    interrupted = 1;

    tty_mode(0);
    set_crmode();
    response = get_response("\t\tCTRL+C! Interrupted! OK to quit");

    if(!response){
        printf("\nOkay, Bye...\n");
        tty_mode(1);
        exit(1);
    }
    else{
        printf("\n\t\tHello printed %d times\n", j);
        j = 0;
        tty_mode(1);
        return;
    }
}

int main(int argc, char *argv[]){
    void ctrl_c_interrupted(int);
    signal(SIGINT, ctrl_c_interrupted);

    for(i = 0; i < 12; ++i){
        if(interrupted){
            signal(SIGINT, ctrl_c_interrupted);
            interrupted = 0;
        }
        printf("Hello %d\n", i+1);
        j++;
        sleep(1);
    }

    return 0;
}
