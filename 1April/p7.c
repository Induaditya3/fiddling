#include <stdio.h>
#include <math.h>

int no_of_digits(int n){
    int counter = 0;
    for (int i = n; i > 0; i/=10, counter ++);
    return counter;
}

int sum_arms(int n){
    int sum = 0;
    int no_digits = no_of_digits(n);
    while(n > 0){
        sum+=pow(n%10,no_digits);
        n/=10;
    }
    return sum;
}

int is_armstrong(int no){
    if (no == sum_arms(no)){
        return 1;
    }
    return 0;
}

int is_prime(int no){
    if (no == 1)
        return 0;
    else if (no == 2)
        return 1;
    else if (no % 2 == 0)
        return 0;
    for (int i = 3; i < no; i+=2){
        if (no % i == 0){
            return 0;
        }
    }
    return 1;
}

int is_perfect(int no){
    int sum_divisors = 1;
    for (int i = 2; i < no; i++){
        if (no%i == 0){
            sum_divisors += i;
        }
    }
    if (sum_divisors == no)
        return 1;
    return 0;
}
int main(){
    printf("%d",is_perfect(2));
    return 0;
}