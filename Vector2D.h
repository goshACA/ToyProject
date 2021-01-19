//
// Created by bpiranda on 22/11/2019.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <cmath>

using namespace::std;

class Vector2D {
public :
    float x,y;
    Vector2D():x(0),y(0) {};
    Vector2D(const Vector2D &v) {x=v.x; y=v.y; }
    Vector2D(float p_x,float p_y):x(p_x),y(p_y) {};
    Vector2D operator-(const Vector2D &op2) const {
        return Vector2D(x - op2.x, y - op2.y);
    }
    Vector2D operator+(const Vector2D &op2) const {
        return Vector2D(x + op2.x, y + op2.y);
    }
    bool operator !=(const Vector2D &op2) const {
        return x != op2.x && y != op2.y;
    }
    
   
    
    bool operator ==(const Vector2D &op2) const {
        return x == op2.x && y == op2.y;
    }
    bool operator < (const Vector2D &op2) const {
        return (y < op2.y) ;
    }
    float operator*(const Vector2D &op2) const {
        return x*op2.x+y*op2.y;
    }
    Vector2D operator*(float a) const {
        return Vector2D(a*x,a*y);
    }
    void set(float p_x,float p_y) {
        x=p_x;
        y=p_y;
    }
    void normalize() {
        float norm=sqrt(x*x+y*y);
        x/=norm;
        y/=norm;
    }
    double norm() const{
        return sqrt(x*x+y*y);
    }
    Vector2D ortho() {
        return Vector2D(-y,x);
    }
    Vector2D rightOrtho() {
        return Vector2D(y,-x);
    }
    
    Vector2D operator=(const Vector2D &rhs) {

    if (this != &rhs) {
        this->x = rhs.x;
        this->y = rhs.y;
    }

    return *this;
  }
    
    void print(){
        std::cout << toString();
    }
    
    string toString(){
        return " ( " +  to_string((int)x) + ", " + to_string((int)y) +" )\n";
    }
   
};


#endif //VECTOR2D_H
