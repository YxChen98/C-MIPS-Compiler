int localArray(int a[]);

int main(){
    int b[10];
    int i=0;
    while(i<9){
        b[i]=1;
        i=i+1;
    }
    return localArray(b)-1;
}
