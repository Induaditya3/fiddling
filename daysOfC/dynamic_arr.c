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
