#include <iostream>  
#include <tgmath.h>

using namespace std;  
int main()  
{  
    int i ;  
    int64_t n ;
    int64_t s ; 

    while(cin >> n){
        if(n == 9223372036854775807) continue ;
        bool b = true ;
        s = sqrt(n) ;
        for(i = 2; i <= s ; i++)  
            if(n % i == 0) {     
                b = false ;
                break ;
            }
        cout << n << " is a prime: " << (b ? "True" : "False") << endl ;
    }
    return 0;  
}  