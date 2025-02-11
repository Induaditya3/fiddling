#include <stdio.h>
#include <assert.h>

void countdown_rec(unsigned int n){
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

