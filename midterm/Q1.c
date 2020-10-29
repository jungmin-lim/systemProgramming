// 2017116186 임정민

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define TRUE	1
#define FALSE	0

void do_ls(char [], int);
void dostat(struct dirent *, DIR *, char []);
void do_size_sorted_stat(struct dirent *, DIR *, char []);
void do_recursive_stat(struct dirent *, DIR *, char []);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int cmp_size_val(const void *p1, const void *p2);

blksize_t total = 0;
typedef struct fileinfo{
	struct stat info;
	char filename[128];
}FILEINFO;

FILEINFO *files;
char dirs[1024][128];

int size = 64;
int count = 0;
int dir_count = 0;

int main(int argc, char *argv[]){
	int i;
	if((files = (FILEINFO *)malloc(sizeof(FILEINFO) * size)) == NULL){
		fprintf(stderr, "error while allocating memory\n");
		exit(EXIT_FAILURE);
	}

	// with no argument passed
	if(argc == 1){
		do_ls(".", FALSE);
	}
	// with some arguments passed
	else{
		// with -s option 
		if(!strcmp(argv[1], "-S") || !strcmp(argv[1], "-s")){
			// directory not specified
			if(argc == 2){
				do_ls(".", TRUE);
			}
			// directory specified
			else{
				for(i = 2; i < argc; ++i){
					do_ls(argv[i], TRUE);
				}
			}
		}

		else if(!strcmp(argv[1], "-R") || !strcmp(argv[1], "-r")){

		}

		else if(!strcmp(argv[1], "-SR") || !strcmp(argv[1], "-sr") ||
				!strcmp(argv[1], "-RS") || !strcmp(argv[1], "-rs")){
		}

		// without option
		else{
			for(i = 1; i < argc; ++i){
				printf("%s:\n", argv[i]);
				do_ls(argv[i], FALSE);
			}
		}
	}
}

// check availability of direcotry
void do_ls(char dirname[], int sort_by_size){
	DIR *dir_ptr;
	struct dirent* direntp;
	int i;

	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls2: cannot open %s\n", dirname);
	
	else{
		if((direntp = readdir(dir_ptr)) != NULL){
			if(sort_by_size){

				do_size_sorted_stat(direntp, dir_ptr, dirname);
		
				// sort list by size 
				qsort(files, count, sizeof(FILEINFO), cmp_size_val);

				// print result
				for(i = 0; i <= count; ++i){
					show_file_info(files[i].filename, &(files[i].info));
				}
			}
			else{
				dostat(direntp, dir_ptr, dirname);
			}
		}
		closedir(dir_ptr);
	}
}

// traverse directory recursively
void dostat(struct dirent *direntp, DIR *dir_ptr, char dirname[]){
	struct stat info;
	char filename[1024];

	sprintf(filename, "%s/%s", dirname, direntp->d_name);
	
	// cannot retreive file info
	if(stat(filename, &info) == -1)
		perror(direntp->d_name);

	else{
		// traverse 
		if((direntp = readdir(dir_ptr)) != NULL){
			total = total + info.st_blocks;
			dostat(direntp, dir_ptr, dirname);
			show_file_info(filename, &info);
		}
		// terminal node
		else{
			total = total + info.st_blocks;
			printf("total %ld\n", total/2);
			show_file_info(filename, &info);
		}
	}
	return;
}

// traverse directory recursively while recording stat
void do_size_sorted_stat(struct dirent *direntp, DIR *dir_ptr, char dirname[]){
	char filename[1024];

	sprintf(files[count].filename, "%s/%s", dirname, direntp->d_name);
	
	if(count >= size){
		if((files = (FILEINFO *)realloc(files, size * 2)) == NULL){
			fprintf(stderr, "error while allocating memroy\n");
			exit(EXIT_FAILURE);
		}
		size = size * 2;
	}

	// cannot retreive file info
	if(stat(files[count].filename, &files[count].info) == -1)
		perror(direntp->d_name);

	else{
		// traverse 
		if((direntp = readdir(dir_ptr)) != NULL){
			total = total + files[count].info.st_blocks;
			count ++;
			do_size_sorted_stat(direntp, dir_ptr, dirname);
		}
		// terminal node
		else{
			total = total + files[count].info.st_blocks;
			printf("total %ld\n", total/2);
		}
	}
	return;
}

void do_recursive_stat(struct dirent *direntp, DIR *dir_ptr, char dirname[]){
	char filename[1024];

	sprintf(files[count].filename, "%s/%s", dirname, direntp->d_name);
	if(direntp->d_type == DT_DIR){
		sprintf(dirs[dir_count++], "%s/%s", dirname, direntp->d_name);
	}
	
	if(count >= size){
		if((files = (FILEINFO *)realloc(files, size * 2)) == NULL){
			fprintf(stderr, "error while allocating memroy\n");
			exit(EXIT_FAILURE);
		}
		size = size * 2;
	}

	// cannot retreive file info
	if(stat(files[count].filename, &files[count].info) == -1)
		perror(direntp->d_name);

	else{
		// traverse 
		if((direntp = readdir(dir_ptr)) != NULL){
			total = total + files[count].info.st_blocks;
			count ++;
			do_size_sorted_stat(direntp, dir_ptr, dirname);
		}
		// terminal node
		else{
			total = total + files[count].info.st_blocks;
			printf("total %ld\n", total/2);
		}
	}
	return;
}

// print file info
void show_file_info(char *filename, struct stat *info_p){
	const char ch = '/';
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	char *ret;

	ret = strrchr(filename, ch);

	mode_to_letters(info_p->st_mode, modestr);
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%-8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s \n", &ret[1]);
}

// file info to letters
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

//  uid to name
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

// gid to name
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

int cmp_size_val(const void *p1, const void *p2){
	return ((FILEINFO *)p1)->info.st_size < ((FILEINFO *)p2)->info.st_size;
}
