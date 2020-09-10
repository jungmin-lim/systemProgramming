#include <stdio.h>

int main(int argc, char *argv[]){
    int sum = 0;
	int i;

	for(i = 1; i <= 500; ++i){
		sum += i;
	}

	printf("The sum of 1 to 500 is %d\n", sum);
	return 0;
}
