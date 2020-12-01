#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFF_SIZE 100

FILE* popen2(const char*, const char*);
void sigint_handler(int);
void sigquit_handler(int);

int main(int argc, char* argv[]){
    FILE* pipein_fp, * pipeout_fp;
    char readbuf[BUFF_SIZE];

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    if( (pipein_fp = popen2("ls", "r")) == NULL ){
        perror("popen2");
        exit(1);
    }

    if( (pipeout_fp = popen2("sort -r", "w")) == NULL ){
        perror("popen2");
        exit(1);
    }

    while( fgets(readbuf, BUFF_SIZE, pipein_fp) ){
        fputs(readbuf, pipeout_fp);
    }

    pclose(pipein_fp);
    pclose(pipeout_fp);
    sleep(1);

    return 0;
}

FILE* popen2(const char* command, const char* mode){
    int thepipe[2], pid;
    int p_end, c_end;
    if( !strcmp(mode, "r") ){
        p_end = 0;
        c_end = 1;
    }
    else if( !strcmp(mode, "w") ){
        p_end = 1;
        c_end = 0;
    }
    else return NULL;

    if( pipe(thepipe) == -1 ){
        return NULL;
    }

    if( (pid = fork()) == -1 ){
        close(thepipe[0]);
        close(thepipe[1]);
        return NULL;
    }

    // parent process
    if( pid > 0 ){
        if( close(thepipe[c_end]) == -1 ){
            return NULL;
        }
        return fdopen( thepipe[p_end], mode );
    }

    // child process
    if( close(thepipe[p_end]) == -1 ){
        exit(1);
    }
    if( dup2(thepipe[c_end], c_end) == -1){
        exit(1);
    }

    if( close(thepipe[c_end]) == -1 ){
        exit(1);
    }

    execl( "/bin/sh", "sh", "-c", command, NULL );
    exit(1);
}

void sigint_handler(int SIG){
    fprintf(stdout, "\tSIGINT received but ignored...\n");
    return;
}
void sigquit_handler(int SIG){
    fprintf(stdout, "\tSIGQUIT received but ignored...\n");
}