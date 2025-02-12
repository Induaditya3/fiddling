//day 2: main.c
#include <stdio.h>
#include "d2adimath.h"

//entry point to the main program
int main(){
	vector_h first, second;
	first.x = 1.2;
	first.y = 2.3;
	second.x = 4.5;
	second.y = 8.0;
	Add(&first,&second);
	printf("x = %f, y = %f\n",first.x,first.y);
	return 0;
}

