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
Another thing to keep in mind is that as soon program comes out of some block, variables declared inside that block (or block scope) is reclaimed, meaning that memory taken up by variables in that block can be resused for storing other stuffs (variables, arrays, structs as needed).

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

## User defined data types

For example a vector with x component and y component can be defined as

```C
typedef struct vector
{
    float x;
    float y;
}vector_h;
```
Here struct is actually defining the type and typedef is reanaming it so we can refer to it by `vector_h` instead of `struct vector`.

## Stack based memory

It is working memory which is used to store temporary stuffs like local variables, from where the function was called and with what arguments, etc. These make up a *stack frame*.
A running program (called a process) uses this kind of memory and this is provided by *RAM*. When a process ends the memory is reclaimed by *os*. If this kind of memory is used to its full capacity then this is known as **stack overflow**.

*os* generally divides a process into 
- source code
- stack
- heap
- ...

Stack itself is implemented using data structure called **stack**. It has two basic actions : adding things (called **pushing** onto stack) and removing things which were last added (called **poping** off the stack). It exhibits **LIFO** (*Last In First Out*). 

For example 
```c
void foo(int x){
    int a = 1;
    int b = 2;
}

int main(){
    for(int i = 0; i < 9999999; i++){
        foo(i);
    }
}
```
Here each time a `foo` is called local variables `a` and `b` is added to stack and then removed. Then in the next iteration of loop, it is again pushed and then popped. And this goes on until the last iteration. Obviously, there is no *stack overflow* because memory is recalimed every time.
## Heap memory

It is large storage, long-lived and fully controlled by programmer. Whenever we use `malloc`, `realloc`, `calloc`, `free` , we are performing actions on this memory.

```c
#include <stdio.h>
#include <stdlib.h>

int* allocateHeapData(int size){
    int* array = (int*)malloc(sizeof(int)*size);
    printf("Address of array located in heap %p\n",array);
    printf("Address of local variable array in stack %p\n",&array);
    for (int i = 0; i < size; i++){
        array[i]=i;
    }
    return array;
}
int main(){
    int* heapData = allocateHeapData(100);
    printf("Address of heapData located in heap %p\n",heapData);
    printf("Address of local variable heapData in stack %p\n",&heapData);
    for(int i = 0; i < 100; i++){
        printf("%d\n",heapData[i]);
    }
    free(heapData);
    return 0;
}
```
This illustrates that we can allocate memory inside some function and access it inside other function (*main* in this case) because memory is not automatically freed unless we do it ourselves, which we do by calling `free(heapData)`. Note also that address stored in `heapData` and in `array` is same but address of each local variables are different but quite similar, because stack allocates memory one after another.

## Header files

Header files *.h* are like interface of OCaml contained in *.mli* . Those files only contain function prototypes, global variable declarations and user defined data types.
It acts as a reference for available data structures and functions.

As opposed to header files, source files *.c* only contain program logic and implementation of algorithms.

## Keywords

`const` in front of avriable means that that variable cannot be modified after first assignment.

`#include "library_name"` means that *library_name* is written by us and compiler should look for it in the current directory (or we full realtive or absolute path). As opposed to `#include <library_name>` which is generally found in location specified in system **PATH** variable.

`void` is lack of value and used to denote function having side effects and no return type much like *unit* of OCaml. It is also denote lack of argument in a function.

`void*` is pointer which can cast into any data type but needs to be done explicitly. For example `malloc` and similar functions have `void*` return type.

## Precautions

Never return a pointer created inside a function. Returning local variable is fine.