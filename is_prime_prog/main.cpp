#include <iostream>  
#include <tgmath.h>

using namespace std;  
int main()  
{  
    int i ;  
    long long n ;
    cout << "Enter the Number to check Prime: ";  
    cin >> n;  
    for(i = 2; i <= sqrt(n); i++)  
        if(n % i == 0) {     
            cout << n << " is a prime: False" << endl ;
            return 0 ; 
        }

    cout << n << " is a prime: True" << endl ;
    //cout <<  n << " is a prime: " << boolalpha << (flag == 0) <<endl ;  
    return 0;  
}  
