//
// Created by Kiui on 14/12/2020.
//

#ifndef CIRCUMSCRIBE_VORONOI_H
#define CIRCUMSCRIBE_VORONOI_H

#include <stdio.h>
#include <vector>
#include <algorithm>
#include "glutWindow.h"
#include "Triangle.h"
#include "Edge.h"
#include "Polygon.h"
#include <map>
#define width 1280
#define height 840
#define UNDEF Vector2D(-10000, -10000);
using namespace std;

class Voronoi {
public:
    vector<vector<Edge>> adjacency;
    vector<Polygon> P;
    vector<Vector2D> vertices;
    vector<Triangle*> triangles;
    vector<Triangle*> sub_triangles;
    vector<pair<Vector2D, vector<int>>> edges;
    map<double, vector<int>> vert;
    double hashFunc(Vector2D &k){
    size_t h1 = std::hash<float>()(k.x);
    size_t h2 = std::hash<float>()(k.y);
    return double(h1 ^ (h2 << 1));
    }
        
    Voronoi(){
        
    }
    
    Voronoi(const vector<Vector2D>& vertices, const vector<Triangle*>& triangles){
        for(int i = 0; i < vertices.size(); ++i){
            this->vertices.push_back(vertices[i]);
        }
        
        for(int i = 0; i < triangles.size(); ++i){
            this->triangles.push_back(new Triangle(*triangles[i]));
        }
        Vector2D undef(-10000, -10000);
        for(int i = 0; i < vertices.size(); ++i){
            vector<Edge> row;
            for(int k = 0; k < vertices.size(); ++k){
                row.push_back(Edge(undef, undef));
            }
            adjacency.push_back(row);
        }
    }
    
    vector<Triangle *> getSubTriangles(Vector2D &qi) {
        vector<Triangle*> tmp;
        for(int i = 0; i < triangles.size(); ++i){
            if(triangles[i]->isVertice(qi)){
                tmp.push_back(triangles[i]);
            }
        }
        return tmp;
    }
    
    void insert(vector<Vector2D>& points){
        if(points.size() > 1){
            auto k = points[points.size()-1] - points[points.size()-2];
            auto key = hashFunc(k);
        auto p = vert[key];
        p.push_back(P.size());
        vert.insert({key, p});
        }
    }
    
    void insert(vector<Vector2D>& points, int i){
        if(points.size() > 1){
            auto k = points[i] - points[i-1];
            auto key = hashFunc(k);
        auto p = vert[key];
        p.push_back(P.size());
        vert.insert({key, p});
        }
    }
    
    void setAdjacency(){
        for(int i = 0; i < adjacency.size(); ++i){
            for(int j = 0; j <= i; ++j){
                for(auto &e: P[i].edges){
                    if(i == j) {
                        int size = P[i].borderEdges.size();
                        if(size > 0)
                            adjacency[i][j] = P[i].borderEdges[size-1];
                    }else{
                        for(auto &k: P[j].edges){
                            if(e == k || e == Edge(k.B, k.A)){
                                adjacency[i][j] = e;
                                adjacency[j][i] = k;
                            }
                        }
                    }
                }
            }
        }
    }
    
    vector<Edge> getUnsharedBorders(int i){
        vector<Edge> edges;
        return edges;
    }
    
    vector<Edge> setEdges(vector<Vector2D> points){
        vector<Edge> edges;
        for(int i = 0; i < points.size() - 1; ++i){
            edges.push_back(Edge(points[i], points[i+1]));
        }
        return edges;
    }
    
    void solveVoronoi(){
        for(int i = 0; i < vertices.size(); ++i){
            class Polygon p;
            vector<Vector2D> points;
            sub_triangles = getSubTriangles(vertices[i]);
            auto t = leftTriangle(sub_triangles, vertices[i]);
            bool isOpened = t != nullptr;
            if(isOpened){
                Edge* E = /*nextEdge(t, vertices[i]);*/ new Edge(vertices[i], *nextPoint(t, vertices[i]));
                Vector2D H = E->center();
                Vector2D u = rightOrthogonalVector(*E);
                auto Q = IntersectionWithBorders(H, u, 0, 0, width, height);
                points.push_back(Q);
                insert(points);
            }else{
                t = sub_triangles[0];
            }
            t->print();
            while(sub_triangles.size() > 1){
                t->calculateCircle();
                points.push_back(t->circumCenter);
                insert(points);
                auto const tprev = t;
                t =  rightTriangle(*t,vertices[i]);
                removeTriangle(sub_triangles, tprev);
            }
            std::cout << " i: " << i << std::endl;
            //t->print();
            t->calculateCircle();
            points.push_back(t->circumCenter);
            insert(points);
            if(isOpened){
                //new Edge(vertices[i], *nextPoint(t, vertices[i]));
                auto E_ = new Edge(*prevPoint(t, vertices[i]), vertices[i]);
                auto H_ = E_->center();
                auto u_ = rightOrthogonalVector(*E_);
                auto Q_ =  IntersectionWithBorders(H_, u_ ,0,0,width,height);
                points.push_back(Q_);
                insert(points);
            }
            removeTriangle(sub_triangles, t);
            //p = Polygon(points);
            addCornerPoints(points);
            p = Polygon(points);
            p.setEdges();
            p.setVoronoiMode(true);
            //p.setEdges();
            P.push_back(p);
        }
        setAdjacency();
        printAdjacency();
        
    }
    
    
    void printAdjacency(){
        cout <<"ADJACENCY\n";
        for(int i = 0; i < 10; ++i){
            for(int j = 0; j < 10; ++j){
                adjacency[i][j].print();
            }
            cout << endl;
        }
    }
    
    
    
    void removeTriangle(vector<Triangle*>& sub_triangles, Triangle* tprev){
        sub_triangles.erase(remove(sub_triangles.begin(), sub_triangles.end(), tprev), sub_triangles.end());
    }
    
    Vector2D* nextPoint(Triangle* t, Vector2D qi){
        int i = 0;
        
        int ind = t->verticeIndex(qi);
        if(ind != -1){
            return new Vector2D(t->ptr[(ind+1)%3]->x, t->ptr[(ind+1)%3]->y);
        } return nullptr;
    }
    
    Vector2D* prevPoint(Triangle* t, Vector2D qi){
        int i = 0;
        
        int ind = t->verticeIndex(qi);
        if(ind != -1){
            int d = ind == 0 ? 3 : 0;
            return new Vector2D(t->ptr[(ind + d -1)%3]->x, t->ptr[(ind + d-1)%3]->y);
        } return nullptr;
    }
    Edge* nextEdge(Triangle* t, Vector2D qi){
        int i = 0;
        while (i<3){
            if (t->edge[i].A == qi)
                return &t->edge[i];
            i++;
        }
        return nullptr;
        
    }
    Edge* prevEdge(Triangle* t, Vector2D qi){
        int i = 0;
        while (i<3){
            if (t->edge[i].B == qi)
                return &t->edge[i];
            i++;
        }
        return nullptr;
        
    }
    Triangle* leftTriangle(const vector<Triangle*> &T, Vector2D Q){
       
        
        auto p=T.begin();
        while (p!=T.end()) {
            auto nextP = nextPoint(*p, Q);
            bool isNotLeft = false;
            for(int i = 0; i < T.size(); ++i){
                int ind = T[i]->verticeIndex(*nextP);
                if(*T[i] != **p && ind != -1 && *nextPoint(T[i], *nextP) == Q){
                    isNotLeft = true;
                    break;
                }
            }
            if(!isNotLeft && p!=T.end()){
                return *p;
            }
            ++p;
        } return nullptr;
    }
    
    Triangle* rightTriangle(Triangle &t, const Vector2D& A){
        auto B = prevPoint(&t, A);
        for(int i = 0; i < sub_triangles.size(); ++i){
            if(*sub_triangles[i] != t){
                auto next = prevPoint(sub_triangles[i], *B);
                if(next != nullptr && *next == A){
                    return sub_triangles[i];
                }
            }
        }
        
        if(sub_triangles.size() == 2){
            return *sub_triangles[0] == t? sub_triangles[1] : sub_triangles[0];
        }
        return nullptr;
    }
    
    
    void addCornerPoints(vector<Vector2D> &c){
        
        vector<Vector2D> vert;
        for(int i = 0; i < c.size(); ++i){
            vert.push_back(Vector2D(c[i].x, c[i].y));
        }
        
        //TODO: do while
        if (((vert[0].x == 0)&&(vert[vert.size()-1].y == 0)) || ((vert[0].y == 0)&&(vert[vert.size()-1].x == 0))){
            c.push_back(Vector2D(0,0));
            //insert(c, i+1);
        } else if(((vert[0].y == 0)&&(vert[vert.size()-1].x == width)) || ((vert[0].x == width)&&(vert[vert.size()-1].y == 0))){
            c.push_back(Vector2D(width,0));
           //insert(c, i+1);
        } else if(((vert[0].x == width)&&(vert[vert.size()-1].y == height)) || ((vert[0].y == height)&&(vert[vert.size()-1].x == width))){
            c.push_back(Vector2D(width,height));
           //insert(c, i+1);
        } else if(((vert[0].x == 0)&&(vert[vert.size()-1].y == height)) || ((vert[0].y == height)&&(vert[vert.size()-1].x == 0))){
            c.push_back(Vector2D(0,height));
            //insert(c, i+1);
        }
        
        
        for(int i = 0; i < vert.size()-1; ++i){
            if (((vert[i].x == 0)&&(vert[i+1].y == 0)) || ((vert[i].y == 0)&&(vert[i+1].x == 0))){
                c.insert(c.begin() + i+1, Vector2D(0,0));
                //insert(c, i+1);
            } else if(((vert[i].y == 0)&&(vert[i+1].x == width)) || ((vert[i].x == width)&&(vert[i+1].y == 0))){
                c.insert(c.begin() + i + 1, Vector2D(width,0));
               //insert(c, i+1);
            } else if(((vert[i].x == width)&&(vert[i+1].y == height)) || ((vert[i].y == height)&&(vert[i+1].x == width))){
                c.insert(c.begin() +  i+1, Vector2D(width,height));
               //insert(c, i+1);
            } else if(((vert[i].x == 0)&&(vert[i+1].y == height)) || ((vert[i].y == height)&&(vert[i+1].x == 0))){
                c.insert(c.begin() + i+1, Vector2D(0,height));
                //insert(c, i+1);
            }
        }
    }
    
    
    
    
    Vector2D rightOrthogonalVector(Edge e){
        Vector2D A = e.A;
        Vector2D B = e.B;
        return Vector2D((B.y - A.y), -B.x + A.x);
    }
    
    Vector2D IntersectionWithBorders(Vector2D& a, Vector2D& u, int x0, int y0, int x1, int y1){
        Vector2D point;
        double k[] = {(x0 - a.x)/u.x, (x1 - a.x)/u.x, (y0 - a.y)/u.y, (y1 - a.y)/u.y};
        std::sort(k, k + 4);
        double ind = -1;
        for(int i = 0; i < 4; ++i){
            if(int(k[i]) > 0){
                ind = k[i];
                break;
            }
        }
        point = a + u*(float)ind;
        if(isInside(point))
        return point;
        
        auto p = point;
       
        auto end = Vector2D(a.x + u.x, a.y + u.y);
        auto e = Edge(a, point);
        auto l = Edge(Vector2D(0, 0), Vector2D(0, height));
        auto t = Edge(Vector2D(0, height), Vector2D(width, height));
        auto r = Edge(Vector2D(width, height), Vector2D(width, 0));
        auto b = Edge(Vector2D(width, 0), Vector2D(0, 0));
        
        if(Intersects(a, point, l.A, l.B, p)){
            return p;
        }
        
        if(Intersects(a, point, r.A, r.B, p)){
            return p;
        }
        
        if(Intersects(a, point, b.A, b.B, p)){
            return p;
        }
        
        if(Intersects(a, point, t.A, t.B, p)){
            return p;
        }
        
        return point;
    }
    
    bool isInside(Vector2D& point){
        return point.x >= 0 && point.x <= width && point.y >= 0 && point.y <= height;
    }
     bool Intersects(Vector2D a1, Vector2D a2, Vector2D b1, Vector2D b2,  Vector2D& intersection)
    {
        intersection = Vector2D(-10000, -10000);

        Vector2D b = a2 - a1;
        Vector2D d = b2 - b1;
        float bDotDPerp = b.x * d.y - b.y * d.x;

        // if b dot d == 0, it means the lines are parallel so have infinite intersection points
        if (bDotDPerp == 0)
            return false;

        Vector2D c = b1 - a1;
        float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
        if (t < 0 || t > 1)
            return false;

        float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
        if (u < 0 || u > 1)
            return false;

        intersection = a1 + b*t;

        return true;
    }
    
    
};


#endif //CIRCUMSCRIBE_VORONOI_H
