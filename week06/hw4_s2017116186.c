#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

struct flaginfo{
    int fl_value;
    char *fl_name;
};

struct flaginfo input_flags[] = {
    IGNBRK  ,   "ignbrk",
    BRKINT  ,   "brkint",
    IGNPAR  ,   "ignpar",
    PARMRK  ,   "parmrk",
    INPCK   ,   "inpck",
    ISTRIP  ,   "istrip",
    INLCR   ,   "inlcr",
    IGNCR   ,   "igncr",
    ICRNL   ,   "icrnl",
    IXON    ,   "ixon",
    IXOFF   ,   "ixoff",
    0       ,   NULL
};

struct flaginfo output_flags[] = {
    OLCUC   ,   "olcuc"
};

struct flaginfo local_flags[] = {
    ISIG    ,   "isig",
    ICANON  ,   "icanon",
    ECHO    ,   "echo",
    ECHOE   ,   "echoe",
    ECHOK   ,   "echok",
    0       ,   NULL
};

void showBaud(int thespeed);
void showSomeFlags(struct termios *ttyp);
void showFlagset(int thevalue, struct flaginfo thebitnames[]);
void flipFlagset(tcflag_t *flaginfo, struct flaginfo thebitnames[]);

int main(int argc, char *argv[]){
    struct termios ttyinfo;

    if(argc > 2){
        fprintf(stderr, "Usage: %s <-OPTION>\n", argv[0]);
        exit(-1);
    }

    if(tcgetattr(0, &ttyinfo) == -1){
        perror("cannot get params about stdin\n");
        exit(-1);
    }

    if(argc > 1){
        if(!strcmp(argv[1], "-FI")){
            flipFlagset(&ttyinfo.c_iflag, input_flags);
        }
        else if(!strcmp(argv[1], "-FO")){
            flipFlagset(&ttyinfo.c_oflag, output_flags);
        }
        else if(!strcmp(argv[1], "-FL")){
            flipFlagset(&ttyinfo.c_lflag, local_flags);
        }
        else{
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "-FI: Flip input flags\n");
            fprintf(stderr, "-FO: Flip output flags\n");
            fprintf(stderr, "-FL: Flip local flags\n");
            exit(-1);
        }

        if(tcsetattr(0, TCSANOW, &ttyinfo) == -1){
            perror("canno set params about stdin\n");
            exit(-1);
        }

        if(tcgetattr(0, &ttyinfo) == -1){
            perror("cannot get params about stdin\n");
            exit(-1);
        }
    }

    showBaud(cfgetospeed(&ttyinfo));

    printf("erase = ^%c;kill = ^%c;\n", ttyinfo.c_cc[VERASE]-1+'A', ttyinfo.c_cc[VKILL]-1+'A');

    showSomeFlags(&ttyinfo);
    
    return 0;
}

void showBaud(int thespeed){
    switch(thespeed){
        case B300:      printf("speed 300 baud;\n"); break;
        case B600:      printf("speed 600 baud;\n"); break;
        case B1200:     printf("speed 1200 baud;\n"); break;
        case B1800:     printf("speed 1800 baud;\n"); break;
        case B2400:     printf("speed 2400 baud;\n"); break;
        case B4800:     printf("speed 4800 baud;\n"); break;
        case B9600:     printf("speed 9600 baud;\n"); break;
        default   :     printf("speed Fast baud;\n"); break;
    }

    return;
}

void showSomeFlags(struct termios *ttyp){
    showFlagset(ttyp->c_iflag, input_flags);
    showFlagset(ttyp->c_oflag, output_flags);
    showFlagset(ttyp->c_lflag, local_flags);

    return;
}

void showFlagset(int thevalue, struct flaginfo thebitnames[]){
    int i;
    for(i = 0; thebitnames[i].fl_value; ++i){
        if(!(thevalue & thebitnames[i].fl_value)){
            printf("-");
        }

        printf("%s ", thebitnames[i].fl_name);
    }

    printf("\n");
    return;
}

void flipFlagset(tcflag_t *flaginfo, struct flaginfo thebitnames[]){
    int i;
    for(i = 0; thebitnames[i].fl_value; ++i){
        *flaginfo ^= thebitnames[i].fl_value;
    }

    return;
}
