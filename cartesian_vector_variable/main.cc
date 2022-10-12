// vector-test.cc
#include <iostream>

#include "vector.hh"

#include <vector>
#include <cstring> //strcmp

using namespace std ;

void testInit()
{
    {
    auto u = Vector(1);
    std::cout << u << '\n';
    }

    {
    auto u = Vector{1};
    std::cout << u << '\n';
    }


    {
    auto u = Vector(5);
    std::cout << u << '\n';
    }

    {
    auto u = Vector{1,2,3,4,5};
    std::cout << u << '\n';
    }

    {
    auto u = Vector{1};
    std::cout << u << '\n';
    u = Vector{1,2,3,4,5};
    std::cout << u << '\n';
    }

}

std::vector<Vector>
generate(size_t n, size_t dim)
{
    auto v = std::vector<Vector>();

    cout << "Start" << endl ;

    // Set some values

    cout << "n = " << n << endl ;
    cout << "dim = " << dim << endl ;
     
    for (size_t i= 0; i < n; ++i)
        {
            //cout << "i = " << i << endl ;
            v.push_back(Vector(dim));
            auto& vv = v[i];
            for (size_t j = 0; j < dim; ++j)
                {
                    //cout << "j = " << j << endl ;
                    vv[j] = (value) n*i + j;
                }
        }
    return v;
}

// Takes the number of Vectors used as an argument
void testAdd(size_t n, size_t dim)
{
    auto vv = generate(n, dim);

    // Add to each Vector his right neighbour
    for (size_t i = 1; i < n; ++i){
        vv[i-1] += vv[i];
    }
        

    // Sum them up
    auto sum = Vector(dim); // Expected to be all zeros
    for (const auto& other : vv)
        sum = sum + other;
    // Print for verif
    std::cout << sum << '\n';
}

// Takes as input the number of vectors
// As well a as a list of actions to perform
// scaling and offset will use two arguments from argv,
// all others will be performed on two neighbouring elements
// In the end a pairwise dot-product is calculated and summed up for all
void testVar(size_t n, size_t dim, int argc, char* argv[])
{
    if (n&1)
        throw std::runtime_error("Need an even number of elements.");

    auto v = generate(n, dim);
    for (int idx = 0; idx < argc; ++idx)
        {
            if (std::strcmp(argv[idx], "add") == 0)
                {
                    for (size_t i = 1; i < n; ++i)
                        v[i-1] += v[i];
                }
            else if (std::strcmp(argv[idx], "scale") == 0)
                {
                    ++idx;
                    auto s = (value) std::atoll(argv[idx]);
                    for (auto& vv : v)
                        vv *= s;
                }
            else if (std::strcmp(argv[idx], "offset") == 0)
                {
                    ++idx;
                    auto s = (value) std::atoll(argv[idx]);
                    for (auto& vv : v)
                        vv += s;
                }

        }
    value sum = 0;
    for (size_t i = 1; i < n; i += 2)
        sum += v[i-1]*v[i];
    std::cout << sum;
    std::cout.put('\n');
}


void testPerso(){

    cout << "Start test perso" << endl ;
    
    Vector v2 = Vector{5, 3, 1, 3} ;
    Vector v1 = Vector{5, -2, 4, 1} ;

    cout << v1 << endl ;
    cout << v2 << endl ;

    v2 += v1 ;

    cout << v2 << endl ;

    cout << "------" << endl ;

    //Vector v3 = v1 + v2 ;
}




int main(int argc, char* argv[])
{
    if (argc <= 3)
        throw std::runtime_error("Expects: first arg, number of elements, "
                                 "Second arg, number of dimension, "
                                 "other args, instructions to run");

    testPerso() ;

    size_t n_elem = std::atol(argv[1]);
    size_t dim = std::atol(argv[2]);

    if (std::strcmp(argv[3], "testInit") == 0)
        testInit();
    else if (std::strcmp(argv[3], "testAdd") == 0)
        testAdd(n_elem, dim);
    else
        testVar(n_elem, dim, argc - 3, &(argv[3]));

    return 0;
}
