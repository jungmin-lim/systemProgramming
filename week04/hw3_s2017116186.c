// 2017116186 임정민

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void doMv(char *, char *);
void doLimitedMv(char *, char *, long int);

int main(int argc, char *argv[]){
    char src[1024], dest[1024];

    // insufficient argument
    if(argc < 3 || 5 < argc || argc == 4){
        fprintf(stderr, "Usage: %s <SOURCE> <DESTINATION> <OPTION -l> <OPTION limit>\n", argv[0]);
        exit(-1);
    }

    strcpy(src, argv[1]);
    strcpy(dest, argv[2]);

    if(argc == 3) doMv(src, dest);
    if(argc == 5){
        if(!strcmp(argv[3], "-l")) doLimitedMv(src, dest, (long int)atoi(argv[4]));
        else{
            fprintf(stderr, "Invalid option\n");
            exit(-1);
        } 
    }

    return 0;
}

void doMv(char *src, char *dest){
    char *filename, newdest[1024];
    struct stat src_stat, dest_stat;
    
    if(stat(src, &src_stat) == -1){
        fprintf(stderr, "%s: error while opening file\n", src);
        exit(-1);
    }
    filename = strrchr(src, '/');
    if(filename == NULL){
        filename = src;
    }

    // if destination exist
    if(stat(dest, &dest_stat) != -1){
        // check if destination is directory
        if(S_ISDIR(dest_stat.st_mode)){
            sprintf(newdest, "%s/%s", dest, filename);
            doMv(src, newdest);
            return;
        }
        else{
            fprintf(stderr, "%s: already exist.\n", src);
            exit(-1);

        }
    }
    
    if(rename(src, dest) != 0){
        fprintf(stderr, "rename failed with error %s\n", strerror(errno));
    }
    return;
}

void doLimitedMv(char *src, char *dest, long int limit){
    struct stat src_stat;
    
    if(stat(src, &src_stat) == -1){
        fprintf(stderr, "%s: error while opening file\n", src);
        exit(-1);
    }
 
    if(src_stat.st_size > limit){
        fprintf(stderr, "%s size is over than limit(%ld)\n", src, limit);
        exit(-1);
    }

    doMv(src, dest);
    return;
}
