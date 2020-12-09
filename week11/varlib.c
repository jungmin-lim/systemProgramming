#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "varlib.h"

#define MAXVARS 200

struct var{
	char* str;
	int global;
};

static struct var tab[MAXVARS];

static char* new_string(char*, char*);
static struct var* find_item(char*, int);

int VLstore(char* name, char* val){
	struct var* itemp;
	char* s;
	int rv = 1;

	if((itemp = find_item(name, 1)) != NULL && (s = new_string(name,val)) != NULL){
		if(itemp->str) free(itemp->str);

		itemp->str = s;
		rv = 0;
	}

	return rv;
}

char* new_string(char* name, char* val){
	char* retval;

	retval = malloc(strlen(name)+strlen(val)+2);
	if(retval != NULL){
		sprintf(retval, "%s=%s", name, val);
	}

	return retval;
}

char* VLlookup(char* name){
	struct var* itemp;
	if((itemp = find_item(name, 0)) != NULL){
		return itemp->str+1+strlen(name);
	}
}

int VLexport(char* name){
	struct var* itemp;
	int rv = 1;

	if((itemp = find_item(name, 0)) != NULL){
		itemp->global = 1;
		rv = 0;
	}
	else if(VLstore(name, "") == 1){
		rv = VLexport(name);
	}

	return rv;
}

static struct var* find_item(char* name, int first_blank){
	int i;
	int len = strlen(name);
	char* s;

	for(i = 0; i < MAXVARS && tab[i].str != NULL; ++i){
		s = tab[i].str;
		if(strncmp(s, name, len) == 0 && s[len] == '='){
			return &tab[i];
		}
	}

	if(i < MAXVARS && first_blank){
		return &tab[i];
	}

	return NULL;
}

char* replaceWordInText(const char *text, const char *oldWord, const char *newWord) {
   int i = 0, cnt = 0;
   int len1 = strlen(newWord);
   int len2 = strlen(oldWord);
   for (i = 0; text[i] != '\0'; i++) {
      if (strstr(&text[i], oldWord) == &text[i]) {
         cnt++;
         i += len2 - 1;
      }
   }
   char *newString = (char *)malloc(i + cnt * (len1 - len2) + 1);
   i = 0;
   while (*text) {
      if (strstr(text, oldWord) == text) {
         strcpy(&newString[i], newWord);
         i += len1;
         text += len2;
      }
      else
      newString[i++] = *text++;
   }

   return newString;
}

char* replace_variable(char* buf){
	char var[BUFSIZ];
	char temp_val[BUFSIZ], val[BUFSIZ];
	char* cp, * cp1;
	struct var* v;

    // replace variables with its value
	cp = strchr(buf, '$');
	while(cp != NULL){
		sscanf(cp, "%s", var);
		v = find_item(&var[1], 0);
		if(v == NULL){
			val[0] = '\0';
		}
		else{
			strcpy(temp_val, v->str);
			cp1=strchr(temp_val, '=');
			*cp1 = '\0';
			strcpy(val, cp1+1);
		}
		buf = replaceWordInText(buf, var, val);

		cp = strchr(cp+1, '$');
	}

	return buf;
}

void VLlist(){
	int i;

	for(i = 0; i < MAXVARS && tab[i].str != NULL; ++i){
		if(tab[i].global){
			printf(" *%s\n", tab[i].str);
		}
		else{
			printf("  %s\n", tab[i].str);
		}
	}
}

int VLenviron2table(char* env[]){
	int i;
	char* newstring;
	for(i = 0; env[i] != NULL; ++i){
		if(i == MAXVARS) return 0;
		newstring = malloc(1+strlen(env[i]));
		if(newstring == NULL){
			return 0;
		}

		strcpy(newstring, env[i]);
		tab[i].str = newstring;
		tab[i].global = 1;
	}

	while(i < MAXVARS){
		tab[i].str = NULL;
		tab[i++].global = 0;
	}

	return 1;
}

char** VLtable2environ(){
	int i, j;
	int n = 0;
	char** envtab;

	for(i = 0; i < MAXVARS && tab[i].str != NULL; i++){
		if(tab[i].global == 1) n++;
	}

	envtab = (char** )malloc((n+1)*sizeof(char* ));
	if(envtab == NULL) return NULL;

	for(i =0, j = 0; i < MAXVARS && tab[i].str != NULL; ++i){
		if(tab[i].global == 1) envtab[j++] = tab[i].str;
	}

	envtab[j] = NULL;
	return envtab;
}
