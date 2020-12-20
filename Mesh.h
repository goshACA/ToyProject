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

using namespace std;
class Mesh: public GlutWindow{
private:
    vector<Vector2D> vertices;
    vector<Triangle*> draw_triangles;
    
    Triangle* neighborInside(Triangle* base, int* ind);
public:
    Mesh(const std::vector<Triangle*>& triangles, const vector<Vector2D>& vertices, int argc,char **argv);
    Mesh(const vector<Vector2D>& vertices, int argc,char **argv);
    
    ~Mesh();
    void drawTriangles();
    void onDraw() override;
    void onStart() override {}
    void onQuit() override {}
    void onMouseMove(double cx,double cy) override;
    void onMouseDown(int button,double cx,double cy) override {}
    void onKeyPressed(unsigned char c,double x,double y) override;
    
    void solveDealunay();
    bool isAllDealunay();
    
    
    void triangulate();

  
    
};

#endif /* Mesh_h */
