//
//  Edge.h
//  dealunay
//
//  Created by Gohar Kunjiyan on 12/13/20.
//  Copyright © 2020 Gohar Kunjiyan. All rights reserved.
//

#ifndef Edge_h
#define Edge_h
#include "Vector2D.h"
struct Edge {
    Vector2D A;
    Vector2D B;
    Edge(Vector2D A, Vector2D B){
        this->A = A;
        this->B = B;
    }
    Edge(){
        this->A = Vector2D(-1, -1);
        this->B = Vector2D(-1, -1);
    }
    bool operator==(const Edge& other) const{
        return (A == other.A && B == other.B) || (A == other.B && B == other.A);
    }
    bool operator!=(const Edge& other) const{
        return !(*this == other);
    }
    bool isNotVertice(const Vector2D& point) const{
        return point != A && point != B;
    }
    
    bool areIdentical(const Edge& other) const{
        return A == other.A && B == other.B;
    }
    
    Vector2D center(){
        return Vector2D((B.x + A.x)/2, (B.y + A.y)/2);
    }
    
    double norm() const{
        return sqrt((B.x-A.x)*(B.x-A.x)+(B.y-A.y)*(B.y-A.y));
    }
    
    void print(){
        std::cout<< A.toString() << " ; " << B.toString() << std::endl;
    }
    
    Edge operator*(float a) const {
        const auto c = A*a;
        const auto b = B*a;
        Edge e;
        e.A = A*a;
        e.B = B*a;
        return e;
    }
    
    bool isParallelOrEqual(Edge e){
        if((int)A.x == (int)e.B.x && (int)A.y ==(int)e.B.y && (int)B.x == (int)e.A.x && (int)B.y == (int)e.A.y)
            return true;
        if((int)A.x == (int)e.A.x && (int)A.y ==(int)e.A.y && (int)B.x == (int)e.B.x && (int)B.y == (int)e.B.y)
        return true;
        Vector2D a = B - A;
        Vector2D c = e.B - e.A;
        return a.x*c.y - a.y*c.x == 0;
    }
};

#endif /* Edge_h */
