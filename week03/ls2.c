// 2017116186 임정민

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char []);
void dostat(struct dirent *, DIR *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

blksize_t total = 0;

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
}

void do_ls(char dirname[]){
	DIR *dir_ptr;
	struct dirent* direntp;

	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls2: cannot open %s\n", dirname);
	
	else{
		if((direntp = readdir(dir_ptr)) != NULL)
			dostat(direntp, dir_ptr);

		closedir(dir_ptr);
	}
}

void dostat(struct dirent *direntp, DIR *dir_ptr){
	struct stat info;
	char filename[50];
	int namelength;

	if(stat(direntp->d_name, &info) == -1)
		perror(direntp->d_name);
	else{
		namelength = strlen(direntp->d_name);
		strcpy(filename, direntp->d_name);
		filename[namelength] = '\0';

		if((direntp = readdir(dir_ptr)) != NULL){
			total = total + info.st_blocks;
			dostat(direntp, dir_ptr);
			show_file_info(filename, &info);
		}
		else{
			total = total + info.st_blocks;
			printf("total %ld\n", total/2);
			show_file_info(filename, &info);
		}
	}
	return;
}

void show_file_info(char *filename, struct stat *info_p){
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

	if(mode & S_IRUSR) str[1] = 'r';
	if(mode & S_IWUSR) str[2] = 'w';
	if(mode & S_IXUSR) str[3] = 'x';

	if(mode & S_IRGRP) str[4] = 'r';
	if(mode & S_IWGRP) str[5] = 'w';
	if(mode & S_IXGRP) str[6] = 'x';

	if(mode & S_IROTH) str[7] = 'r';
	if(mode & S_IWOTH) str[8] = 'w';
	if(mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name(uid_t uid){
	struct passwd *etpwuid(), *pw_ptr;
	static char numstr[10];

	if((pw_ptr = getpwuid(uid)) == NULL){
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid){
	struct group *getgrid(), *grp_ptr;
	static char numstr[10];

	if((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
