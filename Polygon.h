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
#define width 1280
#define height 840

using namespace std;



class Polygon {
private:
    vector<Vector2D> outside_verts;
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
    
    vector<Edge> edges;
    vector<Edge> borderEdges;
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
        
        for(int i = 0; i < outside_verts.size() - 1; ++i){
            Edge e(outside_verts[i], outside_verts[i+1]);
            if(outside_verts[i].x == 0 || outside_verts[i].x == width ||  outside_verts[i].y == 0 || outside_verts[i].y == height
               || outside_verts[i+1].x == 0 || outside_verts[i+1].x == width ||  outside_verts[i+1].y == 0 || outside_verts[i+1].y == height
               && (outside_verts[i].x == outside_verts[i+1].x || outside_verts[i].y == outside_verts[i+1].y))
                borderEdges.push_back(e);
            edges.push_back(e);
        }
        Edge e(outside_verts[0], outside_verts[outside_verts.size()-1]);
        if(outside_verts[0].x == 0 || outside_verts[0].x == width ||  outside_verts[0].y == 0 || outside_verts[0].y == height
           || outside_verts[outside_verts.size()-1].x == 0 || outside_verts[outside_verts.size()-1].x == width ||  outside_verts[outside_verts.size()-1].y == 0 || outside_verts[outside_verts.size()-1].y == height
        && (outside_verts[0].x == outside_verts[outside_verts.size()-1].x || outside_verts[0].y == outside_verts[outside_verts.size()-1].y))
            borderEdges.push_back(e);
        edges.push_back(Edge(outside_verts[outside_verts.size()-1], outside_verts[0]));
        sort(borderEdges.begin(), borderEdges.end());
    }
    
    void setEdges(vector<Edge> e){
        for(int i = 0; i < e.size(); ++i){
            edges.push_back(e[i]);
        }
    }
    
    void ccw(){
        
    }
    
};



#endif /* Polygon_h */
