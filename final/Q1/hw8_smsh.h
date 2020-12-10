#define YES 1
#define No	0

char *next_cmd();
char **splitline(char *, int*);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void*, size_t);
int execute(char **);
int execute_background(char **, int *);
void fatal(char *, char *, int);
int process(char**);
