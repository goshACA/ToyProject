//
//  Polygon.cpp
//  dealunay
//
//  Created by Gohar Kunjiyan on 12/16/20.
//  Copyright © 2020 Gohar Kunjiyan. All rights reserved.
//

#include <stdio.h>
#include "Polygon.h"
#include <cmath>
#include <queue>


vector<Vector2D> Polygon::getVertices() const{
    return vertices;
}

vector<Triangle*> Polygon::getTriangulation() const{
    return triangles;
}
Polygon::Polygon(){
    
}
Polygon::Polygon( vector<Vector2D>& points){
    for(int i = 0; i < points.size(); ++i){
        vertices.push_back(points[i]);
    }
    auto p = points.begin();
    auto pymin = points.begin();
    sort(vertices.begin(), vertices.end());
    sort(points.begin(), points.end());
    Vector2D origin(points.begin()->x,points.begin()->y);
    vector<Vector2D> pointsRelative;
    for (auto pO:points) {
        pointsRelative.push_back(Vector2D(pO.x-origin.x,pO.y-origin.y)); }
    
    sort(pointsRelative.begin()+1,pointsRelative.end(), [] (const Vector2D& p1, const Vector2D& p2) {
        return acos(p1.x/p1.norm()) < acos(p2.x/p2.norm());
    });
    
    stack<Vector2D*> CHstack;
    Vector2D *top_1,*top;
    CHstack.push(&pointsRelative[0]);
    CHstack.push(&pointsRelative[1]);
    CHstack.push(&pointsRelative[2]);
    int i = 0;
    vector<Vector2D>::iterator pi=pointsRelative.begin()+3;
    while (pi!=pointsRelative.end()) {
        top = CHstack.top(); // extract top and top_1
        CHstack.pop(); // from the stack
        top_1 = CHstack.top();
        CHstack.push(top);
        while (!isOnTheLeft((*pi),*top_1,*top)) {
            CHstack.pop(); // update top and top_1
            top = CHstack.top();
            CHstack.pop();
            top_1 = CHstack.top();
            CHstack.push(top);
        }
        CHstack.push(&(*pi));
        pi++;
        ++i;
    }
    Vector2D* tabPts;
    int N=CHstack.size();
    int Nmax = N;
    tabPts = new Vector2D[Nmax+1];  i=N-1;
    while (!CHstack.empty()) {
        tabPts[i--]=*(CHstack.top())+origin; // place the popped point in
        CHstack.pop();
    }
    // the global referential
    tabPts[N]=tabPts[0];
    
    for(int i = 0; i < N; ++i){
        outside_verts.push_back(tabPts[i]);
    }
    
    triangulate();
    
    
}
Polygon::~Polygon(){
    vertices.clear();
    triangles.clear();
}


void Polygon::triangulate(){
    for(int i = 1; i < outside_verts.size()-1; ++i){
        triangles.push_back(new Triangle(outside_verts[0], outside_verts[i], outside_verts[i+1]));
    }
    
}

bool Polygon::comparison(const Vector2D& p1,  const Vector2D& p2){
    Vector2D x(1, 0);
    return p1*x < p2*x;
}

void Polygon::draw(){
    
    for(auto &t: triangles)
        t->draw();
    
    for(int i = 0; i < vertices.size(); ++i){
        glColor3d(0.0, 0.0, 0.0);
        GlutWindow::drawText(vertices[i].x, vertices[i].y, std::to_string(i));
    }
    
    
}
bool Polygon::isOnTheLeft( Vector2D &p,  Vector2D &p1,  Vector2D &p2){
    Vector2D AB = p2 - p1,
    AP = p - p1;
    return (AB.x * AP.y - AB.y * AP.x) >= 0;
}


void Polygon::tri(){
    vector<Triangle> base_triangles;
    queue<Triangle> tris;
    for(int i = 1; i < outside_verts.size() - 1; ++i){
        auto tri = Triangle(outside_verts[0], outside_verts[i], outside_verts[i+1]);
        tris.push(tri);
    }
    while(!tris.empty()){
        auto top = tris.front();
        tris.pop();
        bool is_inside = false;
        for(int i = 0; i < vertices.size(); ++i){
            if(top.isNotVertice(vertices[i]) && top.isInside(vertices[i])){
                if(!is_inside) is_inside = true;
                auto verts = top.getVertices();
                tris.push(Triangle(verts[0], verts[1], vertices[i]));
                tris.push(Triangle(verts[1], verts[2], vertices[i]));
                tris.push(Triangle(verts[2], verts[0], vertices[i]));
                break;
            }
        }
        if(!is_inside){
            base_triangles.push_back(new Triangle(top));
        }
   
    }
    
    triangles.clear();
    for(int i = 0; i <  base_triangles.size(); ++i){
        triangles.push_back(new Triangle(base_triangles[i]));
    }
    glutPostRedisplay();
}
