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
    
    void solveDealunay();
    bool isAllDealunay();
    
    void triangulate();
    
    vector<Polygon> getVoronoiPolygons();
    vector<Triangle*> getTriangulation();

    vector<vector<Edge>> getAdjacency(){
        return voronoi.adjacency;
    }
  
    
};

#endif /* Mesh_h */
