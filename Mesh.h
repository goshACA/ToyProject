//
//  Mesh.h
//  dealunay
//
//  Created by Gohar Kunjiyan on 11/30/20.
//  Copyright © 2020 Gohar Kunjiyan. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h
#include "Triangle.h"
#include <vector>
#include <list>
#include "glutWindow.h"
#include "Voronoi.h"

using namespace std;
class Mesh{
private:
    vector<Vector2D> vertices;
    vector<Triangle*> draw_triangles;
    Triangle* neighborInside(Triangle* base, int* ind);
    Voronoi voronoi;
    bool isVoronoiSolved = false;
public:
    Mesh(const std::vector<Triangle*>& triangles, const vector<Vector2D>& vertices);
    Mesh(const vector<Vector2D>& vertices);
    Mesh();
    
    ~Mesh();
    void drawTriangles();
   /* void onDraw() override;
    void onStart() override {}
    void onQuit() override {}
    void onMouseMove(double cx,double cy) override;
    void onMouseDown(int button,double cx,double cy);
    void onKeyPressed(unsigned char c,double x,double y) override;*/
    
    void solveDealunay();
    bool isAllDealunay();
    
    void triangulate();
    
    vector<Polygon> getVoronoiPolygons();
    vector<Triangle*> getTriangulation();

    Edge getUnshared(Polygon& p);
    vector<vector<Edge>> getAdjacency(){
        return voronoi.adjacency;
    }
  
    
};

#endif /* Mesh_h */
