#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
     int c;
     while((c = getchar()) != EOF){
         putchar(c);
     }
     return 0;
}
