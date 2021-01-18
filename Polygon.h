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
#include <map>
#define INF 10000

using namespace std;

struct Room{
    string name = "";
    Vector2D point;
    int hexValue = -1;
    Room(): point(Vector2D(-1, -1)) {
        
    }
    Room(string _name, float x, float y, int _hexValue): name(_name), point(Vector2D(x, y)){
        hexValue = _hexValue;
    }
};

class Polygon {
private:
    
    Edge unsharedEdge;
    
    vector<Triangle*> triangles;
    bool isOnTheLeft( Vector2D &p,  Vector2D &p1,  Vector2D &p2);
    bool comparison(const Vector2D& p1,  const Vector2D& p2);
    bool isVoronoiMode = false;
    void print(std::stack<Vector2D*> s){
        while(!s.empty()) {
            s.top()->print();
            s.pop();
        }
    }
public:
    vector<Vector2D> outside_verts;
    vector<Edge> edges;
    map<Vector2D, Edge> _edges;
    vector<Vector2D> neighborEdges;
    vector<Vector2D> vertices;
    float color[3];
    string name;
    void setVoronoiMode(bool isVoronoi);
    vector<Vector2D> getVertices() const;
    vector<Triangle*> getTriangulation() const;
    void drawText();
    Polygon();
    Polygon(vector<Vector2D>& points);
    ~Polygon();
    
    void triangulate();
    void draw();
    void tri();
    void addPoint(Vector2D v);
    void addVertice(int position,Vector2D v);
    void defineColor(int hexValue);
    double calculateSurface();
    void setEdges(){
        vector<Vector2D> norm;
       for(int i = 0; i < outside_verts.size(); ++i){
           
           norm.push_back(outside_verts[i]);
       }
        
        for(int i = 0; i < norm.size(); ++i){
           
        }
        
        for(int i = 0; i < outside_verts.size() - 1; ++i){
           
            edges.push_back(Edge(norm[i], norm[i+1]));
        }
        edges.push_back(Edge(outside_verts[outside_verts.size()-1], outside_verts[0]));
    }
    
    void setEdges(vector<Edge> e){
        for(int i = 0; i < e.size(); ++i){
            edges.push_back(e[i]);
        }
    }
    
};



#endif /* Polygon_h */
