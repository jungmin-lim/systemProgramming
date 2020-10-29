#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmp.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void do_ls(char []);
void dostat(char* );
void show_file_info(char* ,struct stat* );
void mode_to_letters(int , char []);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);

int main(int argc, char *argv[]){
	if(argc == 1){
		do_ls(".");
	}

	else{
		while(--argc){
			printf("%s:\n", *++argv);
			do_ls(*argv);
		}
	}

	return 0;
}

void do_ls(char dirname[]){
	DIR* dir_ptr;
	struct dirent* direntp;

	if((dir_ptr = opendir(dirname)) == NULL){
		fprintf(stderr, "ls2: cannot open %s\n",dirname);
	}
	else{
		while((direntp = readdir(dir_ptr)) != NULL){
			dostat(direntp->d_name);
		}
		close(dir_ptr);
	}

	return;
}

void dostat(char* filename){
	struct stat info;
	if(stat(filename, &info) == -1){
		perror(filename);
	}
	else{
		show_file_info(filename, &info);
	}
}

void show_file_info(char* filename, struct stat* info_p){
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];

	mode_to_letters(info_p->st_mode, modestr);
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%-8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s \n", filename);
}

void mode_to_letters(int mode, char str[]){
	strcpy(str, "----------");

	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';
	
	if(mode &
}	
