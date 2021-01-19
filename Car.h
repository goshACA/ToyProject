//
// Created by Kiui on 04/01/2021.
//

#ifndef EXEMPLE_TEXTURE_GI_CAR_H
#define EXEMPLE_TEXTURE_GI_CAR_H
#define PI 3.14159265358 //Go as long as you want, the longer the more accurate
#define radianstodegrees(R)((180.0 * R) / PI)//Converts Radians to Degrees
#define degreestorad(R)(R* PI /  180.0)
#include "glutWindow.h"
#include <vector>
#include "Vector2D.h"
#include "Polygon.h"

const int park_w = 80, park_h = 40, car_w = 32, car_h = 32;
struct Texture {
    GLuint id;
    int f_width;
    int f_height;
    Vector2D pos;
    string name;
    double rotateAngle;
    Edge e;
    Texture(){}
    Texture(Vector2D& pos, Edge& e, GLuint id,
            int f_width,
            int f_height,
            double rotateAngle,
            string name){
        this->pos = pos;
        this->e = e;
        this->id = id;
        this->f_width = f_width;
        this->f_height = f_height;
        this->rotateAngle = rotateAngle;
        this->name = name;
    }
};

struct Gate: Texture {
    Gate(): Texture(){}
    Gate(Vector2D& pos, Edge& e, GLuint id, double rotateAngle,
    int f_width = park_h,
    int f_height = park_w): Texture(pos, e, id,
                           f_width, f_height, rotateAngle, "GATE"){}
};

struct CarPark: Texture {
    int count;
    int polygon;
    CarPark(): Texture(){}
    CarPark(Vector2D& pos, Edge& e, GLuint id, double rotateAngle, int polygon, int count = 0): Texture(pos, e, id,
                           park_w, park_h, rotateAngle, "CARPARK"){
        this->count = count;
        this->polygon = polygon;
        
    }
    
};

class Car {

public:
    vector<Vector2D> points;
    GLuint textureId=0;
    float t=0, epsilon = 0.007;
    double f_width = 80;
    double f_height = 32;
   // double defAngle = 0;
    double angle = 0;
    Vector2D position,source= Vector2D(40,250),V0 = Vector2D(400,0),V1 = Vector2D(400,400);
    Vector2D destination = Vector2D(400,200);
    int polygon;
    Car();
    Car(GLuint texture, Vector2D& position);
    Car(GLuint texture, Vector2D& position,  int polygon);
    Car(GLuint texture, Vector2D& source, Vector2D& destination, Vector2D& V0, Vector2D& V1,  int polygon, double angle);
    GLuint getTextureId(){
        return textureId;
    }
    void setTexture(string txt_path, int lx, int ly){
        textureId = GlutWindow::loadTGATexture(txt_path,lx,ly);
    }
    void draw();
    Vector2D traj(float t);
    float move(double ft, float velocity);
    double getAngle(const Vector2D &a, const Vector2D &b)  {
        Vector2D u = b-a;
        if (u.x>0) {
            return radianstodegrees(asin(u.y/u.norm()));
        } else {
            return 180-radianstodegrees(asin(u.y/u.norm()));
        }
    }
};
const string nameColor[] = {"blue", "white", "cyan", "purple", "brown", "grey", "red", "green", "orange", "yellow"};

struct Room{
    string name = "";
    Vector2D point;
    int hexValue = -1;
    Gate gate;
    vector<CarPark> parks;
    vector<Car*> cars;
    Room(): point(Vector2D(-1, -1)) {
        
    }
    Room(string _name, float x, float y, int _hexValue): name(_name), point(Vector2D(x, y)){
        hexValue = _hexValue;
    }
   
private:
    static int getColorIndex(string& name){
        if(name == "Foxrot") return 0;
        if(name == "Juliett") return 1;
        if(name == "Echo") return 2;
        if(name == "Golf") return 3;
        if(name == "Hotel") return 4;
        if(name == "India") return 5;
        if(name == "Alpha") return 6;
        if(name == "Delta") return 7;
        if(name == "Bravo") return 8;
        if(name == "Charlie") return 9;
        return -1;
    }
public:
    
    static string getColorName(string& name){
        int ind = getColorIndex(name);
        if(ind != -1)
            return nameColor[ind];
        return "";
    }

    
    static string getWrongColorName(string& name, map<string, int>& counts) {
        int r = rand()% 10;
        int ind = getColorIndex(name);
        if(ind == -1)
        return "";
        while(r == ind || counts[nameColor[r]] <= 0){
            r = rand()% 10;
        }
        return nameColor[r];
    }
};



#endif //EXEMPLE_TEXTURE_GI_CAR_H
