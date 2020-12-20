#include <iostream>
#include <cstdlib>
#include <vector>
#include "Mesh.h"
#include "Polygon.h"
using namespace std;

int main(int argc,char **argv) {
    float tabVect[][2]={{280,740},{700,750},{500,700},{900,720},{50,410},{340,400},
        {650,390},{950,300},{400,200},{550,190},{200,50},{800,100}};
    int tabTri[][3]={{0,2,1},{0,4,2},{1,6,3},{2,5,8},{2,6,1},{2,8,6},{3,6,11},{3,11,7},
        {4,5,2},{4,8,5},{4,10,8},{6,8,9},{6,9,11},{8,10,9},{9,10,11}};
    
    int v_size = sizeof(tabVect)/sizeof(tabVect[0]);
    int t_size = sizeof(tabTri)/sizeof(tabTri[0]);
    
    std::vector<Vector2D> vertices;
    std::vector<Triangle*> tris;
    
    
    for(int i = 0; i < v_size; ++i){
        vertices.push_back(Vector2D(tabVect[i][0], tabVect[i][1]));
    }
    for(int j = 0; j < t_size; ++j){
        tris.push_back(new Triangle(vertices[tabTri[j][0]], vertices[tabTri[j][1]], vertices[tabTri[j][2]]));
    }
    
    Polygon p(vertices);
    p.tri();
    Mesh m(p.getTriangulation(), vertices, argc, argv);
    m.start();
    return 0;
}
