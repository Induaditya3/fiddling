#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t stringLen(const char* str){
    size_t length = 0;
    while(str[length] != '\0'){
        length++;
    }
    return length;
}

char* stringAppend( char* destination, char* source){
    size_t destinationLen = stringLen(destination);
    size_t totalLen = destinationLen + stringLen(source);
    char* concat = (char*) malloc(sizeof(char)*(totalLen+1));
    for (int i = 0; i < totalLen; i++){
        if (i < destinationLen){
            concat[i] = destination[i];
        }
        else{
            concat[i] = source[i-destinationLen];
        }
    }
    concat[totalLen] = '\0';
    return concat;
}
int main(){
    char* name = "Aditya Raj";    
    printf("%ld\n",stringLen(name));

    printf("\n\n%s",stringAppend("Hello ","World!"));
    return 0;
}