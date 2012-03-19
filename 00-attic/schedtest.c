#include <stdio.h>

int main () {

		long double x;
		long double i;

		x = 1;

		for (i = 1; i <= 120000000; i++)
			x= x * i;
		printf ("%d faculty: %e \n", i - 1, x);
		return 0;
	}

