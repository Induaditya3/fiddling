void foo(int x){
    int a = 1;
    int b = 2;
}

int main(){
    for(int i = 0; i < 9999999; i++){
        foo(i);
    }
}