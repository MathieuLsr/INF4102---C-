#include <iostream>  
#include <tgmath.h>

using namespace std;  
int main()  
{  
    long long n ;
    int i, m=0, flag=0;  
    cout << "Enter the Number to check Prime: ";  
    cin >> n;  
    //  m=n/2;  
    for(i = 2; i <= sqrt(n); i++) {  
        if(n % i == 0) {     
            flag=1;  
            break;  
        }
    } 
    cout <<  n << " is a prime: " << boolalpha << (flag == 0) <<endl ;  
    return 0;  
}  
