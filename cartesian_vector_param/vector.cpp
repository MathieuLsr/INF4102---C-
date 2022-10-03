//#pragma once

#include <ostream>
#include <iostream>
#include <initializer_list>
#include "vector.hh"
#include "config.h"

Vector::Vector(){
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] = 0 ;
}

Vector::Vector(const std::initializer_list<value> l){
    //std::cout << l.size() << std::endl ;
    assert(l.size() == NDIM) ;
    int i = 0 ;
    for(value va : l){
        tab_[i] = va ;
        i++ ; 
    }
} 

value* Vector::getTab() {
    return this->tab_ ;
}
/*
void Vector::setTab(value tab[NDIM]) {
    this->tab_ = tab ;
}
*/

value& Vector::operator[](const int i) const {
    value v = this->tab_[i] ;
    //std::cout << "Operator[]" << v << std::endl ;
    return v ;
}

value& Vector::operator[](const int i) {
    value v = this->tab_[i] ;
    return this->tab_[i] ;
}

Vector& Vector::operator+=(const Vector& rhs){
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] += rhs[i] ;
    return *this ;
}

Vector& Vector::operator+=(const value& rhs) {
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] += rhs ;
    return *this ;
}


Vector& Vector::operator-=(const Vector& rhs){
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] -= rhs[i] ;
    return *this ;
}

Vector& Vector::operator-=(const value& rhs) {
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] -= rhs ;
    return *this ;
}

Vector Vector::operator+(const Vector& rhs) const {
    Vector v = Vector() ;

    for(int i=0 ; i<NDIM ; i++){
        v[i] = this->tab_[i] + rhs[i] ;
    }

    return v; 
}

Vector Vector::operator-(const Vector& rhs) const {
    Vector v = Vector() ;

    for(int i=0 ; i<NDIM ; i++){
        v[i] = this->tab_[i] - rhs[i] ;
    }

    return v; 
}


value Vector::operator*(const Vector& rhs) const {
    int sum = 0 ;
    for(int i=0 ; i<NDIM ; i++){
        sum += this->tab_[i] * rhs[i] ;
    }
    return sum ;
}

Vector Vector::operator*(const value& rhs) const {
    Vector v = Vector() ;
    for(int i=0 ; i<NDIM ; i++){
        v[i] = this->tab_[i] * rhs ;
        //std::cout << << this->tab_[i] * i << ", " << v[i] << std::endl ;
    }
    return v; 
}



Vector& Vector::operator*=(const value& rhs){
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] *= i ;
    return *this ;
}

Vector& Vector::operator=(const int& rhs){
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] = rhs ;
    return *this ;
}

std::ostream& operator<<(std::ostream &os, Vector const &vec) {

    os << "[" ;
    for(int i=0 ; i<NDIM ; i++){
        os << vec[i] ;
        if(i != NDIM-1) os << ", " ;
    }
    os << "]" ;
    return os;
}


/*
class Vector
{

private:
    value tab_[NDIM] ;

public:

    
    
Vector(){
    for(int i=0 ; i<NDIM ; i++)
        tab_[i] = 0 ;
}

Vector(const std::initializer_list<value> l){
    assert(l.size() != NDIM) ;
    int i = 0 ;
    for(value va : l){
        tab_[i] = va ;
        i++ ; 
    }
}



// Public Member functions here
    value operator[](const int& rhs) const {
        return tab_[rhs] ;
    }
    value& operator[](const int& rhs) {
        return tab_[rhs] ;
    }



} ;

*/