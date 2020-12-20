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
    Edge(Vector2D& A, Vector2D& B){
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
};

#endif /* Edge_h */
