#include <stdio.h>

int gcd (int a, int b){
    if (a % b == 0){
        return b;
    }
    else{
        return gcd(b,a%b);
    }
}

int lcm(int a, int b){
    if (a % b == 0){
        return a;
    }
    else if (gcd(a,b) == 1) {
        return  a*b;
    }
    else{
        int hcf = gcd(a,b);
        return lcm(a/hcf,b/hcf);
    }
}
int main(){
    printf("%d\n",lcm(4,12));
}