#include "vector.hh"

// DO NOT CHANGE THIS
    Vector::Vector(const Vector&){

    }
    Vector& Vector::operator=(const Vector& vec){
        this->size_array = vec.size_array ;
        for(int i = 0 ; i < vec.size() ; i++)
            this->tab_[i] = vec[i] ;
        
        return *this ;
    }
    
//

// Add suitable constructors

    Vector::Vector(std::initializer_list<value> l){
        this->size_array = l.size() ;
        int i = 0 ;
        for(value v : l){
            this->tab_[i] = v ;
            i++ ;
        }
    }

    Vector::Vector(size_t N){
        this->size_array = N ; 
        for(int i = 0 ; i < N ; i++)
            this->tab_[i] = 0 ;
    }

    size_t Vector::size() const {
        return this->size_array ;
    }

// Public Member functions here
    Vector& Vector::operator+=(const Vector& rhs){
        if(this->size() != rhs.size()) throw std::runtime_error("Incompatible size") ;
        for(int i=0 ; i<rhs.size() ; i++)
            tab_[i] += rhs[i] ;
        return *this ;
    }
    // More to go
    Vector& Vector::operator-=(const Vector& rhs){
        if(this->size() != rhs.size()) throw std::runtime_error("Incompatible size") ;
        for(int i=0 ; i<rhs.size() ; i++)
            tab_[i] -= rhs[i] ;
        return *this ;
    }



    Vector& Vector::operator+=(value v){
        for(int i=0 ; i<this->size() ; i++)
            tab_[i] += v ;
        return *this ;
    }
    Vector& Vector::operator*=(value v){
        for(int i=0 ; i<this->size() ; i++)
            tab_[i] *= v ;
        return *this ;
    }

    Vector Vector::operator+(const Vector& rhs) const{
        if(this->size() != rhs.size()) throw std::runtime_error("Incompatible size") ;
        Vector v = Vector(rhs.size()) ;
        for(int i=0 ; i<rhs.size() ; i++){
            v[i] = this->tab_[i] + rhs[i] ;
        }
        return v; 
    }
    Vector Vector::operator+(value v) const{
        Vector vec = Vector(this->size()) ;
        for(int i=0 ; i<this->size() ; i++){
            vec[i] = this->tab_[i] + v ;
        }
        return vec; 
    }


    value Vector::operator*(const Vector& rhs) const{
        if(this->size() != rhs.size()) throw std::runtime_error("Incompatible size") ;
        int sum = 0 ;
        for(int i=0 ; i<this->size() ; i++)
            sum += this->tab_[i] * rhs[i] ;
        return sum ;
    }
    Vector Vector::operator*(value v) const{
        Vector vec = Vector(this->size()) ;
        for(int i=0 ; i<this->size() ; i++){
            vec[i] = this->tab_[i] * v ;
        }
        return vec ; 
    }


    value& Vector::operator[](const size_t i) {
        value v = this->tab_[i] ;
        return this->tab_[i] ;
    }

    value Vector::operator[](const size_t i) const {
        value v = this->tab_[i] ;
        return v ;
    }



    // Nonmember function operators go here
Vector operator*(const value& s, const Vector& v){
    Vector vec = Vector(v.size()) ;
    for(int i=0 ; i<v.size() ; i++)
        vec[i] = v[i] * s ;
    return vec ;

}
Vector operator+(const value& s, const Vector& v){
    Vector vec = Vector(v.size()) ;
    for(int i=0 ; i<v.size() ; i++)
        vec[i] = v[i] + s ;
    return vec ;
}

std::ostream& operator<<(std::ostream& os, const Vector& vec){
    os << "[" ;
    for(int i=0 ; i<vec.size() ; i++){
        os << vec[i] ;
        if(i != vec.size()-1) os << ", " ;
    }
    os << "]" ;
    return os;
}