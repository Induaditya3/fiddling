# C

Credit: [Mike Shah](https://www.youtube.com/watch?v=yLNWu6AxftI&list=PLvv0ScY6vfd8M-Mi_Vyrg7KgISTW3Sklt)

## Block Scope

`{}` defines scope of variables in C, meaning where the variables are visible. It is often called block scope. For example following code will not compile.
```C
#include <stdio.h>

int main(){
    {
        int x = 27;
    }
    printf("%d\n",x);
}
```

## Assert

It helps to safe guard a program from crashing by only executing statements that follows when given condition is true, otherwise safely terminating.

```c
#include <stdio.h>
#include <assert.h>

void countdown_rec( int n){
    assert(n >=0 && "n must be non negative integer");
    if (n < 0){
        return;
    }
    else{
        printf("%d\n",n);
        return countdown_rec(n-1);
    }
}

int main(){
    countdown_rec(10);
    return 0;
}

```

This will terminate as soon as argument n is negative saying 
```bash
Assertion `n >=0 && "n must be non negative integer"' failed.
Aborted (core dumped)
```
(Due to base case of our recursive function this will also terminate if we do not use **assert** )

## Pointers

It stores the memory address of variable or other data structures.

 - `&` operator gives the address of a variable 
 - `*` has dual purpose: declaring a pointer variable and going to particular memory address (in this case called deference operator)

For example 
```c
#include <stdio.h>

int main(){
        int x = 45;
        printf("x = %d\n",x);
        // %p is format specifier for printing memory addresses in hexadecimal
        printf("Address of x is %p\n",&x);
        //this again prints value of but illustrates deference operator * to go to an address
        printf("x = %d\n",*(&x));
        return 0;
}

```

To declare pointer variable, we do

```c
#include <stdio.h>

int main(){
        // Declare a pointer to an integer meaning it stores address of integer
        int* px;
        int x = 12;
        px = &x;
        printf("Address of x = %d is %p\n",x,px);
        return 0;
}

```

We can also dynamically alocate memory at runtime by using `malloc` and use `free` to release the memory back for reuse. Let us dynamically create an array and populate it using userdata.
```c
#include <stdio.h>
//following library called standard library provides malloc 
#include <stdlib.h>

int main(){
    puts("Enter the size of array");
    int size_arr = 0;
    scanf("%d",&size_arr);
    //sizeof operator gives size of single instance of data type in bytes
    int* dynamic_arr = malloc(sizeof(int)*size_arr);
    int n;
    for (int i = 0; i < size_arr; i++){
        printf("Enter a no.:");
        scanf("%d",&n);
        dynamic_arr[i] = n;
    }
    for (int i = 0; i < size_arr; i++){
        printf("dynamic_arr[%d] = %d\n",i,dynamic_arr[i]);
    }
    free(dynamic_arr);
    return 0;
}
```

## Pass by Value and Pass by Reference

Generally, when parameters are provided to functions in C, the function usually receives copy of it, i.e., variables outside the scope remain unaffected  whatever we do inside that block scope of the function.
This is called **pass by value**.

In some cases like passing large data structure such as array as an argument to a function, C provides address of that data structure instead of creating a new copy. Acutally, it does create copy but of address of that data stucture. Here address is the location of starting byte. This is called **pass by reference**.

For example, following illustrates how modifying value of variable by passing of variable doesn't work but passing by reference does.
```c
// pass by value and pass by reference
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
```

## Header files

Header files *.h* are like interface of OCaml contained in *.mli* . Those files only contain function prototypes, global variable declarations and user defined data types.
It acts as a reference for available data structures and functions.

As opposed to header files, source files *.c* only contain program logic and implementation of algorithms.

## Keywords

`const` in front of avriable means that that variable cannot be modified after first assignment.

`#include "library_name"` means that *library_name* is written by us and compiler should look for it in the current directory (or we full realtive or absolute path). As opposed to `#include <library_name>` which is generally found in location specified in system **PATH** variable.

## Precautions

Never return a pointer created inside a function. Returning local variable is fine.