#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <time.h>

#define SHOWHOST 1

void showInfo(struct utmp*);
void showTime(long timeval);

int main(int argc, char *argv[]){
    struct utmp currentRecord;
    int utmpfd;
    int reclen = sizeof(currentRecord);

    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
        perror(UTMP_FILE);
        exit(1);
    }

    while(read(utmpfd, &currentRecord, reclen) == reclen){
        showInfo(&currentRecord);
    }

    close(utmpfd);
    return 0;
}

void showInfo(struct utmp* utmpfd){
    if(utmpfd->ut_type != USER_PROCESS) return; // user only

    printf("%-8.8s ", utmpfd->ut_name);
    printf("%-8.8s ", utmpfd->ut_line);
    showTime(utmpfd->ut_time);

    #ifdef SHOWHOST
        printf("(%s)", utmpfd->ut_host);
    #endif
        printf("\n");
}

void showTime(long timeval){
    printf("%12.12s ", ctime(&(timeval)) + 4);
    return;
}
