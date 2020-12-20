// https://etudiants-stgi.pu-pm.univ-fcomte.fr/tp_bpiranda/adv_algo/circumscribe.zip
// Created by bpiranda on 22/11/2019.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "glutWindow.h"
#include "Vector2D.h"
#include <vector>
#include "Edge.h"
const float BLACK[4]={0.0,0.0,0.0,1.0f};
const float GREY[4]={0.75f,0.75f,0.75f,1.0f};
const float RED[4]={1.0f,0.0,0.0,1.0f};
const float ORANGE[4]={1.0f,0.27f,0.0,1.0f};
const float YELLOW[4]={1.0f,1.0f,0.0,1.0f};
const float GREEN[4]={0.0,1.0f,0.0,1.0f};
const float BLUE[4]={0.0,0.0,1.0f,1.0f};
const float PURPLE[4]={1.0,0.0,1.0f,1.0f};

using namespace::std;

class Triangle {
public:
    Vector2D *ptr[3];
    Edge edge[3];
    bool isHighlighted;
    Vector2D circumCenter;
    float circumRadius = -1;
    bool isDelaunay=false;
    
    
    Triangle(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3) {
        ptr[0] = ptr1;
        ptr[1] = ptr2;
        ptr[2] = ptr3;
        updateEdges();
        isHighlighted = false;
        calculateCircle();
    }
    
    Triangle(Vector2D ptr1,Vector2D ptr2,Vector2D ptr3) {
        ptr[0] = new Vector2D(ptr1);
        ptr[1] = new Vector2D(ptr2);
        ptr[2] = new Vector2D(ptr3);
        updateEdges();
        isHighlighted = false;
        calculateCircle();
    }
    
    Triangle(Triangle* triangle) {
        ptr[0] = new Vector2D(*(triangle->ptr[0]));
        ptr[1] = new Vector2D(*(triangle->ptr[1]));
        ptr[2] = new Vector2D(*(triangle->ptr[2]));
        updateEdges();
        isHighlighted = false;
        calculateCircle();
    }
    
    void updateVertices(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3) {
        ptr[0] = ptr1;
        ptr[1] = ptr2;
        ptr[2] = ptr3;
        isHighlighted = false;
        calculateCircle();
    }
    
    void calculateCircle() {
        Vector2D AB=*ptr[1]-*ptr[0];
        Vector2D AC=*ptr[2]-*ptr[0];
        Vector2D V=AC.ortho();
        // calculate circle parameters
        float k = 0.5f*(AB*AB - AC*AB)/(V*AB);
        
        Vector2D u = (AC*0.5f)+V*k;
        
        circumCenter=*ptr[0]+u;
        circumRadius = (float)(u.norm());
    }
    
    void onMouseMove(const Vector2D& pos) {
        isHighlighted=isInside(pos);
    }
    
    bool isOnTheLeft(const Vector2D *P,const Vector2D *P1,const Vector2D *P2) {
        Vector2D AB = *P2-*P1,
        AP = *P-*P1;
        
        return (AB.x*AP.y - AB.y*AP.x)>=0;
    }
    
    bool isEmpty(const vector<Vector2D*> &tmp,int n) {
        auto p=tmp.begin()+n;
        while (p!=tmp.end() && (!isOnTheLeft(*p,ptr[0],ptr[1]) || !isOnTheLeft(*p,ptr[1],ptr[2]) || !isOnTheLeft(*p,ptr[2],ptr[0]))) {
            p++;
        }
        return p==tmp.end();
    }
    
    bool operator !=(const Triangle& other) const{
        int count = 0;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(edge[i] == other.edge[j])
                    ++count;
            }
        }
        return count != 3;
    }
    
    bool operator ==(const Triangle& other) const{
        int count = 0;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(edge[i] == other.edge[j])
                    ++count;
            }
        }
        return count == 3;
    }
    
    bool isInside(const Vector2D &P) {
        return isOnTheLeft(&P,ptr[0],ptr[1]) && isOnTheLeft(&P,ptr[1],ptr[2]) && isOnTheLeft(&P,ptr[2],ptr[0]);
    }
    
    bool isNotVertice(const Vector2D &P){
        for(int i = 0; i < 3; ++i){
            if(!edge[i].isNotVertice(P))
                return false;
        }
        return true;
    }
    
    
    void updateEdges(){
        edge[0] = Edge(*ptr[0], *ptr[1]);
        edge[1] = Edge(*ptr[1], *ptr[2]);
        edge[2] = Edge(*ptr[0], *ptr[2]);
    }
    
    void draw() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glColor3fv(isHighlighted?ORANGE:(isDelaunay?GREY:YELLOW));
        glBegin(GL_TRIANGLES);
        glVertex2f(ptr[0]->x,ptr[0]->y);
        glVertex2f(ptr[1]->x,ptr[1]->y);
        glVertex2f(ptr[2]->x,ptr[2]->y);
        glEnd();
        glColor3fv(BLACK);
        glBegin(GL_LINE_LOOP);
        glVertex2f(ptr[0]->x,ptr[0]->y);
        glVertex2f(ptr[1]->x,ptr[1]->y);
        glVertex2f(ptr[2]->x,ptr[2]->y);
        glEnd();
    }
    
    void drawCircle() {
        // draw circle
        if(circumRadius == -1){
            calculateCircle();
        }
        if (isHighlighted) {
            glColor3fv(RED);
            glPushMatrix();
            glTranslatef(circumCenter.x,circumCenter.y,0);
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            float a=0.0;
            for (int i=0; i<40; i++) {
                glVertex2f(circumRadius*cos(a),circumRadius*sin(a));
                a+=(float)(M_PI/20.0);
            }
            glEnd();
            glLineWidth(1);
            glPopMatrix();
        }
    }
    
    bool isInsideCircle(const Vector2D &p) {
        if (p!=*ptr[0] && p!=*ptr[1] && p!=*ptr[2])
            return ((p-circumCenter).norm()<circumRadius);
        return false;
    }
    
    // check if the current triangle follow the Delaunay rule
    // i.e. there is no point inside the Circumcircle
    bool checkDelauney(const vector<Vector2D> &vertices) {
        auto p=vertices.begin();
        while (p!=vertices.end() && !isInsideCircle(*p)) {
            p++;
        }
        isDelaunay = (p==vertices.end());
        return isDelaunay;
    }
    
    bool checkLocalDelauney(Triangle* neighbor){
        auto vertices = neighbor -> getVertices();
        for(int i = 0; i < vertices.size();++i){
            if(isInsideCircle(vertices[i]))
                return true;
        }
        return false;
    }
    
    vector<Vector2D> getVertices(){
        vector<Vector2D> verts;
        verts.push_back(Vector2D(ptr[0]->x, ptr[0]->y));
        verts.push_back(Vector2D(ptr[1]->x, ptr[1]->y));
        verts.push_back(Vector2D(ptr[2]->x, ptr[2]->y));
        return verts;
    }
    
    bool isNeighbor(Triangle* other){
        int count = 0;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(edge[i] == other->edge[j])
                    ++count;
            }
        }
        return count == 1;
    }
    
    bool isCounterClockwise(){
        double sum = 0;
        sum += (ptr[1]->x - ptr[0]->x) * (ptr[1]->y + ptr[0]->y);
        sum += (ptr[2]->x - ptr[1]->x) * (ptr[2]->y + ptr[1]->y);
        sum += (ptr[0]->x - ptr[2]->x) * (ptr[0]->y + ptr[2]->y);
        return sum < 0.0;
    }
    
    void flipClockWise(){
        while(!isCounterClockwise()){
            random_shuffle(std::begin(ptr), std::end(ptr));
        }
    }
    
    void flip(Triangle* neighbor){
        Edge shared;
        int shared_ind = -1;
        int shared_ind_n = -1;
        for(int i = 0; i < 3; i++){
            if(shared_ind > -1)
                break;
            for(int  k = 0; k < 3; ++k){
                if(edge[i] == neighbor->edge[k]){
                    shared = edge[i];
                    shared_ind = i;
                    shared_ind_n = k;
                    break;
                }
            }
        }
        
        Vector2D* flip_a = nullptr;
        Vector2D* flip_b = nullptr;
        
        for(int i = 0; i < 3; ++i){
            if(shared.isNotVertice(*ptr[i])){
                flip_a = new Vector2D(ptr[i]->x, ptr[i]->y);
                break;
            }
        }
        
        for(int i = 0; i < 3; ++i){
            if(neighbor->edge[shared_ind_n].isNotVertice(*(neighbor->ptr[i]))){
                flip_b = new Vector2D(*neighbor->ptr[i]);
                break;
            }
        }
        
        
        ptr[0] = new Vector2D(shared.B);
        ptr[1] = new Vector2D(*flip_a);
        ptr[2] = new Vector2D(*flip_b);
        flipClockWise();
        updateEdges();
        
        
        neighbor->ptr[0] = new Vector2D(shared.A);
        neighbor->ptr[1] = new Vector2D(*flip_a);
        neighbor->ptr[2] = new Vector2D(*flip_b);
        neighbor->flipClockWise();
        neighbor->updateEdges();
        
        calculateCircle();
        neighbor->calculateCircle();
    }
    
    
};

#endif //TRIANGLE_H
