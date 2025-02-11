// pass by value
#include <stdio.h>

void set_value(int n){
	n = 123;
}
void setValue(int* n){
	*n = 321;
}
int main(){
	int x = 12;
	printf("x = %d\n",x);
	set_value(x);
	printf("x = %d\n",x);
	// here value remains same
	setValue(&x);
	printf("x = %d\n",x);
	//here value get changed as intended
	return 0;
}
