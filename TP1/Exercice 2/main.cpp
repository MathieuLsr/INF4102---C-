#include <iostream>  
using namespace std;  
int main()  
{  
    int n, i, m=0, flag=0;  
    cout << "Enter the Number to check Prime: ";  
    cin >> n;  
    m=n/2;  
    for(i = 2; i <= m; i++) {  
        if(n % i == 0) {  
            flag=1;  
            break;  
        }
    } 
    cout <<  n << " is a prime: " << boolalpha << (flag == 0) <<endl ;  
    return 0;  
}  