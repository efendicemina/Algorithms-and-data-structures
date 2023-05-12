#include<iostream>
using namespace std;
int fib2_0(int n, int pretprosli=0, int prosli=1,int i=2){
   if(i==n) return pretprosli+prosli;
   return fib2_0(n,prosli,prosli+pretprosli,i+1);
}
int nzd(int x, int y){
    if(y==0) return x;
    return nzd(y,x%y);
}
int main(){
    return 0;
}
