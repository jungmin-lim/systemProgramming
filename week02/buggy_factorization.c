#include <stdio.h>
#include <math.h>

int main()
{
	int a, b, c;
	double alpha, beta, D;

	printf("ax^2 + bx + c = 0\n");

	printf("a: ");
	scanf("%d", &a);

	printf("b: ");
	scanf("%d", &b);

	printf("c: ");
	scanf("%d", &c);

	D = b*b + 4*a*c;
	if(D < 0){
		alpha = ((-1 * b) + sqrt(D)) / (double)(2 * a);
		beta = ((-1 * b) - sqrt(D)) / (double)(2 * a);
		printf("%d(x%+.2f)(x%+.2f) = 0\n",a, -1 * alpha, -1 * beta);
	}
	else
		printf("Impossible\n");
	return 0;
}
