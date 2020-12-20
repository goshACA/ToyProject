//
//  Polygon.h
//  dealunay
//
//  Created by Gohar Kunjiyan on 12/16/20.
//  Copyright © 2020 Gohar Kunjiyan. All rights reserved.
//

#ifndef Polygon_h
#define Polygon_h

#include <stdio.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "glutWindow.h"
#include "Triangle.h"
#include "Vector2D.h"
#include <stack>
#define INF 10000

using namespace std;

class Polygon {
private:
    vector<Vector2D> vertices;
    vector<Vector2D> outside_verts;
    vector<Triangle*> triangles;
    bool isOnTheLeft( Vector2D &p,  Vector2D &p1,  Vector2D &p2);
    bool comparison(const Vector2D& p1,  const Vector2D& p2);
    void print(std::stack<Vector2D*> s){
       while(!s.empty()) {
            s.top()->print();
            s.pop();
        }
    }
public:
    float *color;
    
    vector<Vector2D> getVertices() const;
    vector<Triangle*> getTriangulation() const;
    Polygon();
    Polygon(vector<Vector2D>& points);
    ~Polygon();
    
    void triangulate();
    void draw();
    void tri();
    
};



#endif /* Polygon_h */
