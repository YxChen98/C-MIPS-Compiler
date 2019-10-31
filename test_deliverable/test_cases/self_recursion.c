int rec(int b){
    b=b+1;

    if(b<10){
        rec(b);
    }

    else{
        return b;
    }
}
