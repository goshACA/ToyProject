//
//  TinyToys.h
//  dealunay
//
//  Created by Gohar Kunjiyan on 1/17/21.
//  Copyright © 2021 Gohar Kunjiyan. All rights reserved.
//

#ifndef TinyToys_h
#define TinyToys_h

#include "Car.h"
#include "Mesh.h"
#include "glutWindow.h"
#define width 1280
#define height 840
using namespace std;

Vector2D und(-10000, -10000);
Edge undef(und, und);

struct Texture {
    GLuint id;
    int f_width;
    int f_height;
    string name;
    Texture(GLuint id,
            int f_width,
            int f_height,
            string name){
        this->id = id;
        this->f_width = f_width;
        this->f_height = f_height;
        this->name = name;
    }
};

#define MAXN 100
// Infinite value for array
#define INF  99999

int dis[MAXN][MAXN];
int Next[MAXN][MAXN];
const string parkName = "PARK";
const string gateNAME = "GATE";
const string carNAME = "CAR";

// Initializing the distance and
// Next array
void initialise(int V,
                vector<vector<int> >& graph)
{
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dis[i][j] = graph[i][j];
            
            // No edge between node
            // i and j
            if (graph[i][j] == INF)
                Next[i][j] = -1;
            else
                Next[i][j] = j;
        }
    }
}

// Function construct the shotest
// path between u and v
vector<int> constructPath(int u,
                          int v)
{
    // If there's no path between
    // node u and v, simply return
    // an empty array
    if (Next[u][v] == -1)
        return {};
    
    // Storing the path in a vector
    vector<int> path = { u };
    while (u != v) {
        u = Next[u][v];
        path.push_back(u);
    }
    return path;
}

// Standard Floyd Warshall Algorithm
// with little modification Now if we find
// that dis[i][j] > dis[i][k] + dis[k][j]
// then we modify next[i][j] = next[i][k]
void floydWarshall(int V)
{
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                
                // We cannot travel through
                // edge that doesn't exist
                if (dis[i][k] == INF
                    || dis[k][j] == INF)
                    continue;
                
                if (dis[i][j] > dis[i][k]
                    + dis[k][j]) {
                    dis[i][j] = dis[i][k]
                    + dis[k][j];
                    Next[i][j] = Next[i][k];
                }
            }
        }
    }
}

// Print the shortest path
void printPath(vector<int>& path)
{
    int n = path.size();
    for (int i = 0; i < n - 1; i++)
        cout << path[i] << " -> ";
    cout << path[n - 1] << endl;
}

#define PI 3.14159265358 //Go as long as you want, the longer the more accurate
#define radianstodegrees(R)((180.0 * R) / PI)//Converts Radians to Degrees
#define degreestorad(R)(R* PI /  180.0)




class TinyToys: public GlutWindow {
private:
    vector<vector<Edge>> adjacency;
    vector<vector<int> > graph;
    vector<Room> rooms;
    vector<Vector2D> vertices;
    vector<Polygon> polygons;
    vector<Triangle*> triangles;
    vector<double> S;
    vector<Car> cars;
    vector<Texture> figures;
    Mesh m;
    bool isVoronoiMode = false;
    GLuint park;
    GLuint gate;
public:
    
    TinyToys(vector<Room> &rooms, vector<vector<int> > &graph, int argc,char **argv) :GlutWindow(argc,argv,"TinyToys", width, height, FIXED) {
        sort(rooms.begin(),rooms.end(), [] (const Room& p1, const Room& p2) {
            return p1.point < p2.point;
        });
        for(int i = 0; i < rooms.size(); ++i){
            this->rooms.push_back(rooms[i]);
            vertices.push_back(rooms[i].point);
        }
        this->graph = graph;
        int V = vertices.size();
        initialise(vertices.size(), graph);
        floydWarshall(V);
        vector<int> path;
        for (int i = 0; i <= V-1; i++){
            for(int j = 0; j <= V-1; j++){
                cout << "Shortest path from " << i <<" to " <<j << ":"<<endl;
                path = constructPath(i, j);
                printPath(path);
            }
        }
        Polygon p(vertices);
        p.tri();
        m = Mesh(p.getTriangulation(), vertices);
        setTriangulation();
        int x = 0, y = 0;
        park = loadTGATexture("../Textures/parks/carpark.tga",x,y);
        gate = loadTGATexture("../Textures/parks/carstop.tga",x,y);
        initFigures();
    }
    
    void initFigures(){
        int park_w = 80, park_h = 40, car_w = 32, car_h = 32;
        int x = 0, y = 0;
        GLuint park = loadTGATexture("../Textures/parks/carpark.tga",x,y);
        GLuint gate = loadTGATexture("../Textures/parks/carstop.tga",x,y);
        //TODO: add random + check
        GLuint car = loadTGATexture("../Textures/cars/bluecar.tga",x,y);
        figures.push_back(Texture(park, park_w, park_h, parkName));
        figures.push_back(Texture(gate, park_w, park_h, gateNAME));
        figures.push_back(Texture(car, car_w, car_h, carNAME));
        
    }
    
    void setTriangulation(){
        triangles.clear();
        auto res = m.getTriangulation();
        for(int i = 0; i < res.size(); ++i ){
            triangles.push_back(res[i]);
        }
    }
    
    void setVoronoiPolygons(){
        polygons.clear();
        auto res = m.getVoronoiPolygons();
        for(int i = 0; i < res.size(); ++i ){
            res[i].name = rooms[i].name;
            res[i].defineColor(rooms[i].hexValue);
            polygons.push_back(res[i]);
        }
    }
    
    void onDraw() override {
        if(isVoronoiMode){
            drawPolygons();
            drawAdjacency();
        } else {
            drawTriangles();
        }
    }
    
    
    void onUpdate(double dt) override{
        
    }
    
    void onKeyPressed(unsigned char c, double x, double y) override{
        if(c == 'a'){
            m.solveDealunay();
            setTriangulation();
        } else if(c == 'd' && !isVoronoiMode){
            toVoronoiMode();
        }
    }
    
    void onMouseMove(double x, double y) override{
        Vector2D v((float)x,(float)y);
        for (auto &t:triangles) {
            t->onMouseMove(v);
        }
    }
    
    void toVoronoiMode(){
        isVoronoiMode = true;
        setVoronoiPolygons();
        calculateSurface();
        adjacency = m.getAdjacency();
    }
    
    void drawAdjacency(){
        for(int i = 0; i < adjacency.size(); ++i){
            for(int j = 0; j < adjacency.size(); ++j){
                //if(adjacency[i][j] != undef){
                if(graph[i][j] !=INF){
                    auto midpoint = (adjacency[i][j].A + adjacency[i][j].B) * 0.5;
                    int delta = 0;
                    if(graph[j][i] != INF){
                        delta = figures[1].f_width/2;
                        drawTexture(midpoint.x - delta, midpoint.y, figures[1], getAngle(adjacency[j][i].A, adjacency[j][i].B));
                    }
                    drawTexture(midpoint.x + delta, midpoint.y, figures[1], getAngle(adjacency[i][j].A, adjacency[i][j].B));
                }
                // }
            }
        }
    }
    
    double getAngle(Vector2D& a, Vector2D& b){
        double angle = 0;
        double l = (b - a).norm();
        double c = Vector2D(b.x - a.x, a.y).norm();
        return radianstodegrees(acos(c/l));
    }
    
    void drawTriangles(){
        for(auto &p: triangles){
            p->draw();
        }
        
        for(auto &p: triangles){
            p->drawCircle();
        }
        for(auto &p: triangles){
            p->drawCenter();
        }
    }
    
    void drawPolygons(){
        
        for(auto &p: polygons){
            
            p.draw();
            
        }
        drawText();
        
        
    }
    
    
    void drawTexture(int lx, int ly, Texture& t, double angle){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        
        assert(t.id!=0);
        
       glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,t.id);
        glPushMatrix();
        
        glTranslatef(lx,ly,0.0);
       // glRotatef(angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex2f(0.0,-t.f_height);
        glTexCoord2f(1.0,0.0);
        glVertex2f(t.f_width,-t.f_height);
        glTexCoord2f(1.0,1.0);
        glVertex2f(t.f_width,t.f_height);
        glTexCoord2f(0.0,1.0);
        glVertex2f(0.0,t.f_height);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        
        
        
        
       /* // bind to the appropriate texture for this image
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D,t.id);
        glPushMatrix();
        // translate to the right location and prepare to draw
       // glColor3f(1, 1, 1);
        glTranslatef(lx + (t.f_width / 2), ly + (t.f_height / 2), 0);
        
        glRotated(angle, 1, 1, 0);
        glTranslatef(-t.f_width / 2, -t.f_height / 2, 0);
        // draw a quad textured to match the sprite
        glBegin(GL_QUADS);
        
       glTexCoord2f(0, 0);
        glVertex2f(0, 0);
        glTexCoord2f(0, 1);
        glVertex2f(0, t.f_height);
        glTexCoord2f(1, 1);
        glVertex2f(t.f_width, t.f_height);
        glTexCoord2f(1, 0);
        glVertex2f(t.f_height, 0);
        glEnd();
        glPopMatrix();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);*/
    glEnd();
    
    // restore the model view matrix to prevent contamination
    glPopMatrix();
}

void drawText(){
    for(int i = 0; i < vertices.size(); ++i){
        glColor3d(0.0, 0.0, 0.0);
        GlutWindow::drawText(vertices[i].x, vertices[i].y, polygons[i].name, GlutWindow::ALIGN_CENTER);
    }
}
void onStart() override {}
void onQuit() override {}


void calculateSurface(){
    double res = 0;
    for(auto &p: polygons){
        double si = p.calculateSurface();
        res += si;
        S.push_back(si);
    }
    sort(S.begin(), S.end());
    
    //cout << "SURFACE = " << res << endl;
}


};






#endif /* TinyToys_h */
