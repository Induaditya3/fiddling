#include <stdio.h>
#include <string.h>

size_t stringLen(const char* str){
    int length = 0;
    while(str[length]!=0){
        length++;
    }
    return length;
}
int main(){
    char* name = "Aditya Raj";
    name[0]='E';
    
    printf("%ld\n",stringLen(name));
    return 0;
}