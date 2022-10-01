#pragma once

#include <ostream>
#include <iostream>
#include <initializer_list>

#include "config.h"

class Vector
{
public:
// DO NOT CHANGE THIS
    Vector(const Vector&) = default;
    Vector& operator=(const Vector&) = default;
    ~Vector() = default;
//

// Add suitable constructors
    Vector();
    //Vector(value tab[NDIM]);
    Vector(const std::initializer_list<int> list) ;
    // possibly more
    value* getTab() ; 
    //void setTab(value tab_[NDIM]) ;

// Public Member functions here
    Vector& operator+=(const Vector& v);
    Vector& operator+=(const value& v);
    Vector& operator-=(const Vector& v);
    Vector& operator-=(const value& v);

    Vector operator+(const Vector& v) const ;
    Vector operator-(const Vector& v) const ;
    
    
    value operator*(const Vector& v) const ;
    Vector operator*(const value& v) const ;
    Vector& operator*=(const value& v);
    
    //value operator[](const int& v) const ;
    value& operator[](const int v) ;
    value& operator[](const int v) const ;
    
    Vector& operator=(const int& v);
    friend std::ostream& operator<<(std::ostream &os, Vector const &vec) ;
    
    // More to go

    

private:
// Private Member functions here
    value tab_[NDIM] ;

// Member variables are ALWAYS private, and they go here
};

// Nonmember function operators go here