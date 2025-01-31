#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define oops(m, x) {perror(m); exit(x);}
#define NUM_ENDS 2

void be_bc(int* todc, int* fromdc);
void be_dc(int* in, int* out);
void fatal(char mess[]){
    fprintf(stderr, "Error: %s\n", mess);
    exit(1);
}

int main(int argc, char* argv[]){
    int pid, todc[NUM_ENDS], fromdc[NUM_ENDS];

    if(pipe(todc) == -1 || pipe(fromdc) == -1){
        oops("pipe failed", 1);
    }

    if((pid = fork()) == -1){
        oops("cannot fork", 2);
    }

    if(pid == 0){
        be_dc(todc, fromdc);
    }
    else{
        be_bc(todc, fromdc);
        wait(NULL);
    }

    return 0;
}

void be_bc(int* todc, int* fromdc){
    int num1, num2;
    char operation[BUFSIZ], message[BUFSIZ], *fgets();
    FILE* fpout, * fpin, * fdopen();

    close(todc[0]);
    close(fromdc[1]);

    fpout = fdopen(todc[1],  "w");
    fpin = fdopen(fromdc[0], "r");

    if(fpout == NULL || fpin == NULL){
        fatal("Error converting pipes to streams");
    }

    while(printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL){
        if(sscanf(message, "%d%[-+*/^]%d", &num1, operation, &num2) == EOF){
            fatal("Syntax Error\n");
        }

        if(fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF){
            fatal("Error writing");
        }
        fflush(fpout);
        if(fgets(message, BUFSIZ, fpin) == NULL) break;
        printf("%d %c %d = %s", num1, *operation, num2, message);
    }

    fclose(fpout);
    fclose(fpin);

    return;
}

void be_dc(int* in, int* out){
    if(dup2(in[0], 0) == -1){
        oops("dc: cannot redirect stdin", 3);
    }

    close(in[0]);
    close(in[1]);

    if(dup2(out[1], 1) == -1){
        oops("dc: cannot redirect stdout", 4);
    }

    close(out[1]);
    close(out[0]);

    execlp("dc", "dc", "-", NULL);
    oops("cannot run dc", 5);
    
    return;
}