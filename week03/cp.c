#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

#define BUFFERSIZE  4096
#define COPYMODE    0644

int main(int argc, char* argv[]){
	int src, dest, len;
	char buf[BUFFERSIZE];

	if(argc != 3){
		printf("Usage: %s <SOURCE> <TARGET>\n", argv[0]);
		exit(1);
	}

	if((src = open(argv[1], O_RDONLY)) == -1){
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	if((dest = creat(argv[2], COPYMODE)) == -1){
		printf("Cannot creat %s\n", argv[2]);
		exit(1);
	}

	while((len = read(src, buf, BUFFERSIZE)) > 0){
		if(write(dest, buf, len) != len){
			printf("Error while writing from %s to %s\n", argv[1], argv[2]);
			exit(1);
		}
	}

	if(len == -1){
		printf("Error while reading from %s\n", argv[1]);
		exit(1);
	}

	if(close(src) == -1 || close(dest) == -1){
		printf("Error while closing file\n");
		exit(1);
	}

    return 0;
}
