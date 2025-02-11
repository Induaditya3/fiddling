#include <stdio.h>

int main(){
	// Declare a pointer to an integer meaning it stores address of integer
	int* px;
	int x = 12;
	px = &x;
	printf("Address of x = %d is %p\n",x,px); 
	return 0;
}
