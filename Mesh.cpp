//
//  Mesh.cpp
//  dealunay
//
//  Created by Gohar Kunjiyan on 12/4/20.
//  Copyright © 2020 Gohar Kunjiyan. All rights reserved.
//

#include <stdio.h>
#include "Mesh.h"
#include <queue>

Mesh::~Mesh(){
    draw_triangles.clear();
    vertices.clear();
}

Mesh::Mesh(const vector<Triangle*>& triangles, const vector<Vector2D>& vertices, int argc,char **argv) :GlutWindow(argc,argv,"Dealunay",1000,800,FIXED){
    
    for(int i = 0; i < vertices.size(); ++i){
        this->vertices.push_back(vertices[i]);
    }
    
    for(int i = 0; i < triangles.size(); ++i){
        triangles[i]->checkDelauney(this->vertices);
        draw_triangles.push_back(new Triangle(*triangles[i]));
    }
}

Mesh::Mesh(const vector<Vector2D>& vertices, int argc,char **argv) :GlutWindow(argc,argv,"Dealunay",1000,800,FIXED){
    
    for(int i = 0; i < vertices.size(); ++i){
        this->vertices.push_back(vertices[i]);
    }
    
    for(int i = 0; i < vertices.size() - 1; ++i){
        
    }
    std::sort(this->vertices.begin(), this->vertices.end());
    triangulate();
}

void Mesh::drawTriangles(){
    for(auto &p: draw_triangles){
        p->draw();
    }
    
    for(auto &p: draw_triangles){
        p->drawCircle();
    }
}

void Mesh::onDraw(){
    drawTriangles();
}

void Mesh::onMouseMove(double cx, double cy){
    Vector2D v((float)cx,(float)cy);
    for (auto &t:draw_triangles) {
        t->onMouseMove(v);
    }
}

void Mesh::solveDealunay() {
    while(!isAllDealunay()){
    list<int> processList;
    auto t = draw_triangles.begin();
    int c = 0;
    while(t!=draw_triangles.end()){
        processList.push_back(c);
        ++c;
        t++;
    }
    int* ind = new int(-1);
    int count = 0;
    while(processList.size()>1){
        int cur_ind = processList.front();
        auto current = draw_triangles[cur_ind];
        processList.pop_front();
        ++count;
        if(!current->isDelaunay){
            auto it = neighborInside(current, ind);
            if(it != nullptr){
                Triangle* Tneighbor = new Triangle(*it);
                if(Tneighbor != nullptr){
                    *draw_triangles[*ind] = *Tneighbor;
                    current->flip(draw_triangles[*ind]);
                    current->checkDelauney(vertices);
                    draw_triangles[*ind]->checkDelauney(vertices);
                    auto tr = processList.begin();
                    while (tr!=processList.end() && draw_triangles[(*tr)]!=Tneighbor) tr++;
                    if(tr!=processList.end()) processList.erase(tr);
                }else {
                    processList.push_back(cur_ind);
                }
            }
        }
    }
    }
    
}


Triangle* Mesh::neighborInside(Triangle* base, int* ind){
    for(int i = 0; i < draw_triangles.size(); ++i){
        if((*base == *draw_triangles[i] && !base->isNeighbor(draw_triangles[i])) || draw_triangles[i]->isDelaunay)
            continue;
        else if(base->isNeighbor(draw_triangles[i])){
            if(!draw_triangles[i]->isDelaunay){
                if(base->checkLocalDelauney(draw_triangles[i])){
                    *ind = i;
                    return draw_triangles[i];
                }
            }
        }
    } return nullptr;
}


void Mesh::onKeyPressed(unsigned char c, double x, double y){
    solveDealunay();
}


void Mesh::triangulate(){
    vector<Triangle> base_triangles;
    queue<Triangle> tris;
    for(int i = 0; i < vertices.size() - 1; ++i){
        auto tri = Triangle(vertices[0], vertices[i], vertices[i+1]);
        draw_triangles.push_back(new Triangle(tri));
        base_triangles.push_back(tri);
        tris.push(tri);
    }
    return;
    while(!tris.empty()){
        auto top = tris.front();
        tris.pop();
        bool is_inside = false;
        for(int i = 0; i < vertices.size(); ++i){
            if(top.isInside(vertices[i])){
                if(!is_inside) is_inside = true;
                auto verts = top.getVertices();
                tris.push(Triangle(verts[0], verts[1], vertices[i]));
                tris.push(Triangle(verts[0], verts[2], vertices[i]));
                tris.push(Triangle(verts[1], verts[2], vertices[i]));
            }
        }
        if(!is_inside){
            draw_triangles.push_back(new Triangle(top));
        }
        glutPostRedisplay();
    }
    
    
    
    
}

bool Mesh::isAllDealunay(){
    for(int i = 0; i < draw_triangles.size(); ++i){
        if(!draw_triangles[i]->isDelaunay){
            return false;
        }
    }
    return true;
}
