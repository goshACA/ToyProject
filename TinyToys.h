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
#include <queue>
#define width 1280
#define height 840
using namespace std;

Vector2D und(-10000, -10000);
Edge undef(und, und);



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
    vector<Gate> gates;
    vector<CarPark> carparks;
    Mesh m;
    vector<Car*> queue;
    bool isVoronoiMode = false;
    GLuint park;
    GLuint gate;
    GLuint carId;
    bool move = false;
    double t = 0.005;
    map<string, int> colorCounts;
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
        carId= loadTGATexture("../Textures/cars/bluecar.tga",x,y);
    }
    
    void initFigures(){
        int x = 0, y = 0;
        
        
        
        for(int i = 0; i < adjacency.size(); ++i){
            for(int j = 0; j < adjacency.size(); ++j){
                auto midpoint = getMidPoint(adjacency[j][i].A, adjacency[j][i].B);
                double angle =  getAngle(adjacency[i][j].A, adjacency[i][j].B);
                if(graph[i][j] !=INF){
                    auto g = Gate(midpoint, adjacency[i][j], gate, angle);
                    gates.push_back(g);
                    rooms[i].gate.push_back(make_pair(j,g));
                }
            }
        }
        
        setCarParkBorders();
        
        
        
        for(int i = 0; i < adjacency.size(); ++i){
            auto midpoint = getMidPoint(adjacency[i][i].A, adjacency[i][i].B);
            double angle =  getAngle(adjacency[i][i].A, adjacency[i][i].B);
            
            if(midpoint.x == 0 || midpoint.y == height){
                angle = -angle + 90;
            }
            else if(midpoint.x == width || midpoint.y == 0 && angle == 0){
                angle += 90;
            }
            else if(midpoint.y == 0 && angle == 180){
                angle -=90;
            }
            else {
                angle += 90;
            }
            
            
            Vector2D point;
            int count = getCarParkCount(i);
            int k = 0;
            Edge e;
            while(k < count){
                int d = round(count/2), dist = 0;
                if(k <= d){
                    e = Edge(midpoint, adjacency[i][i].A);
                    dist = k;
                }else {
                    e = Edge(midpoint, adjacency[i][i].B);
                    dist = k - d;
                }
                point = pointOnDistance(park_w/2*dist, e);
                auto carpark = CarPark(point, adjacency[i][i], park,  angle, i);
                carpark.room_id = i;
                carparks.push_back(carpark);
                rooms[i].parks.push_back(carpark);
                ++k;
            }
        }
        fillColorMap();
        placeCars();
        
    }
    
   
    
    Vector2D pointOnDistance(float distance, Edge& e){
        Vector2D v = e.B - e.A;
        Vector2D u = v;
        u.normalize();
        return e.A + u*distance;
    }
    
    Vector2D pointOnDistance(float distance, Vector2D& a, Vector2D& v){
        Vector2D u = v;
        u.normalize();
        return a + u*distance;
    }
    
    
    void setTriangulation(){
        triangles.clear();
        auto res = m.getTriangulation();
        for(int i = 0; i < res.size(); ++i ){
            triangles.push_back(res[i]);
        }
    }
    void fillColorMap(){
        for(auto &room: rooms){
            colorCounts.insert({Room::getColorName(room.name), room.parks.size()});
        }
    }
    
    void setCarParkBorders(){
        for(int i = 0; i < polygons.size(); ++i){
            Edge maxBorder;
            if(adjacency[i][i] == undef){
                for(auto &e: polygons[i].edges){
                    int count = 0;
                    auto midpoint = getMidPoint(e.A, e.B);
                    for(auto &gate: gates){
                        if(gate.e == e){
                            count ++;
                        }
                    }if(count == 0){
                        if(maxBorder < e){
                            maxBorder = e;
                        }
                    }
                }
            }
            if(adjacency[i][i] == undef && !maxBorder.isZero())
                adjacency[i][i] = maxBorder;
            
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
    
    int getCarParkCount(int i){
        return floor(4*S[i]/(3*S[S.size()-1]));
    }

    vector<pair<int,pair<Gate,Vector2D>>> carPath(int s, int d){
        vector<int> path = constructPath(s, d);
        vector<pair<int,pair<Gate,Vector2D>>> result;
        for(int i = 0;i<path.size()-1;i++){
            for(int j=0;j<rooms[path[i]].gate.size();j++){
                if(rooms[path[i]].gate[j].first==path[i+1]){
                    result.push_back(make_pair(path[i+1],make_pair(rooms[path[i]].gate[j].second,(rooms[path[i]].gate[j].second.e.B - rooms[path[i]].gate[j].second.e.A).rightOrtho())));
                }
            }
        }
        return result;
    }
    
    void placeCars(){
        int count=0;
        for(int i = 0; i < polygons.size(); ++i){
            for(auto &carpark: carparks){
                if(carpark.polygon == i){
                    auto v0 = (carpark.e.B - carpark.e.A).rightOrtho();
                    auto v1 = Vector2D(400,700);
                    auto destPos = Vector2D(400,700);
                    auto startPos = getShiftedPoint(carpark);
                    auto car = new Car(getCarTextureByColor(rooms[i].name), startPos,destPos, v0, v1, i, carpark.rotateAngle);
                    car->color=color_car;
                    car->colorId=Room::getIndexByColor(car->color);
                    car->path = carPath(i,car->colorId);
                    car->init();
                    car->id=count++;
                    cars.push_back(*car);
                    int a = getCarparkByValue(i, carpark);
                    if(a != -1){
                        rooms[i].parks[a].setCar(car);
                    }
                    cout << "CAR belongs to "<<car->color<<" id "<<car->colorId<<" exists in "<<rooms[i].name<<" room number "<<i<<" carpark room id "<<carpark.room_id<<" gate room id"<<rooms[i].gate[0].first<<endl;
                    rooms[i].cars.push_back(car);
                }
            }
        }
    }
    int getCarparkByValue(int i, CarPark& carpark){
        for(int j = 0; j < rooms[i].parks.size(); ++j){
            if(carpark.pos == rooms[i].parks[j].pos){
                
                return j;
            }
        }return -1;
    }
    string color_car;
    GLuint getCarTextureByColor(string name){
        string res = Room::getWrongColorName(name, colorCounts);
        --colorCounts[res];
        int x = 0, y = 0;
        color_car=res;
        return loadTGATexture("../Textures/cars/" + res + "car.tga",x,y);
    }
    
    
    int cars_done=0;
    void onUpdate(double dt) override{
        if(move){
            if (!queue.empty()) {
                for (int i = 0; i < queue.size(); i++) {
                    if (!queue[i]->done) {
                        if (!queue[i]->reachedDesGate) {
                            queue[i]->move(dt, 200);
                        } else {
                            if (queue[i]->triggerOtherCar == true) {
                                queue[i]->triggerOtherCar = false;
                                CarPark *cp = rooms[queue[i]->goal].getFreePark();
                                auto V = (cp->e.B - cp->e.A).rightOrtho();
                                auto P = getLastPoint(*cp);
                                queue[i]->update(P, V);
                                cp->free = false;
                                queue.push_back(&cars[cp->car->id]);
                            } else {
                                if (queue[i]->t < 1)
                                    queue[i]->move(dt, 200);
                                else queue[i]->done = true;
                            }
                        }
                    } else {
                        cars_done++;
                        queue.erase(remove(queue.begin(), queue.end(), queue[i]), queue.end());
                    }
                }
            }
            else if(!cars_done < cars.size()){
                int i=0;bool alldone=true;
                while(i<cars.size()&&alldone){
                    if (cars[i].done!=true) {
                        queue.push_back(&cars[i]);
                        alldone=false;
                    }
                    i++;
                }
            }
        }
        
    }
    
    void onKeyPressed(unsigned char c, double x, double y) override{
        if(c == 'a'){
            m.solveDealunay();
            setTriangulation();
        } else if(c == 'd' && !isVoronoiMode){
            toVoronoiMode();
        }
        
        else if(c == 'g'){
            move = true;
            queue.push_back(&cars[2]);
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
        adjacency = m.getAdjacency();
        calculateSurface();
        initFigures();
    }
    
    void drawAdjacency(){
        for(auto &gate: gates){
            drawTexture(gate);
        }
        for(auto &carpark: carparks){
            drawTexture(carpark);
        }
        
        for(auto &car: cars){
            drawCar(car);
        }
        
        
    }
    
    Vector2D getLastPoint(CarPark& carpark){
        Vector2D c = getShiftedPoint(carpark);
        if(carpark.pos.x == width){
            c = c + Vector2D(carpark.f_width, -carpark.f_height);
        } else if(carpark.pos.x > 0 && carpark.pos.x < width && carpark.pos.y > 0 && carpark.pos.y < height){
            auto u = (carpark.e.B - carpark.e.A).ortho();
            u.normalize();
            int t = u.y > 0 ? 1 : -1;
            c = c + Vector2D(0, carpark.f_height/2) * t;
        }
        return c;
    }
    
    Vector2D getOrhoVector(Texture& txt){
        auto d = (txt.e.B - txt.e.A).ortho();
        auto midpoint = txt.pos;
        if(midpoint.y == 0){
            d.normalize();
            if((d.y > 0)) d = (txt.e.B - txt.e.A).ortho();
            else d = (txt.e.B - txt.e.A).rightOrtho();
        } else if(midpoint.x == 0){
            d.normalize();
            if((d.y < 0)) d = (txt.e.B - txt.e.A).ortho();
            else d = (txt.e.B - txt.e.A).rightOrtho();
        }return d;
    }
    
    Vector2D getShiftedPoint(Texture& txt){
        auto d = getOrhoVector(txt);
        return pointOnDistance(txt.f_width, txt.pos, d);
    }
    
    
    Vector2D getMidPoint(Vector2D& a, Vector2D& b){
        return (b + a)*0.5;
    }
    
    double getAngle(const Vector2D &a, const Vector2D &b)  {
        Vector2D u = b-a;
        if (u.x>0) {
            return radianstodegrees(asin(u.y/u.norm()));
        } else {
            return 180-radianstodegrees(asin(u.y/u.norm()));
        }
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
    
    
    void drawCar(Car& t){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //angle = 90;
        
        assert(t.textureId!=0);
        
        int xStart = 0, xEnd = 1, yStart = 0, yEnd = 1;
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, t.textureId);
        
        glTranslatef(t.position.x, t.position.y, 0);
        glRotatef(t.angle, 0, 0, 1);
        
        glBegin(GL_QUADS);
        glTexCoord2d(xStart,yStart);
        glVertex2f(-t.f_width, t.f_height);
        
        glTexCoord2d(xEnd,yStart);
        glVertex2f(0, t.f_height);
        
        glTexCoord2d(xEnd,yEnd);
        glVertex2f(0, 0);
        
        glTexCoord2d(xStart,yEnd);
        glVertex2f(-t.f_width, 0);
        
        glEnd();
        
        //Reset the rotation and translation
        glRotatef(-t.angle,0,0,1);
        glTranslatef(-t.position.x, -t.position.y, 0);
        
        glDisable(GL_TEXTURE_2D);
    }
    
    void drawTexture(Texture& t){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        assert(t.id!=0);
        
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,t.id);
        glPushMatrix();
        
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, t.id);
        
        glTranslatef(t.pos.x, t.pos.y, 0);
        glRotatef(t.rotateAngle, 0, 0, 1);
        
        glBegin(GL_QUADS);
        int xStart = 1, xEnd = 0, yStart = 0, yEnd = 1;
        glTexCoord2d(xEnd,yEnd);
        glVertex2f(0, 0);
        
        glTexCoord2d(xStart,yEnd);
        glVertex2f(t.f_width, 0);
        
        glTexCoord2d(xStart,yStart);
        glVertex2f(t.f_width,t.f_height);
        
        glTexCoord2d(xEnd,yStart);
        glVertex2f(0, t.f_height);
        
        glEnd();
        
        //Reset the rotation and translation
        glRotatef(-t.rotateAngle,0,0,1);
        glTranslatef(-t.pos.x, -t.pos.y, 0);
        
        glDisable(GL_TEXTURE_2D);
        
        
        
      
        glEnd();
        
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
        double min = width * height;
        for(auto &p: polygons){
            double si = p.calculateSurface();
            res += si;
            if(min > si)
                min = si;
            S.push_back(si);
        }
        S.push_back(min);
    }
    
    
};






#endif /* TinyToys_h */
