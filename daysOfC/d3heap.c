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