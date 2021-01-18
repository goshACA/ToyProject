//
// Created by Kiui on 04/01/2021.
//

#ifndef EXEMPLE_TEXTURE_GI_CAR_H
#define EXEMPLE_TEXTURE_GI_CAR_H
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
    Texture(Vector2D& pos, GLuint id,
            int f_width,
            int f_height,
            double rotateAngle,
            string name){
        this->pos = pos;
        this->id = id;
        this->f_width = f_width;
        this->f_height = f_height;
        this->rotateAngle = rotateAngle;
        this->name = name;
    }
};

struct Gate: Texture {
    Gate(Vector2D& pos, GLuint id, double rotateAngle,
    int f_width = park_h,
    int f_height = park_w): Texture(pos, id,
                           f_width, f_height, rotateAngle, "GATE"){}
};

struct CarPark: Texture {
    int count;
    CarPark(Vector2D& pos, GLuint id, double rotateAngle, int count = 0): Texture(pos, id,
                           park_w, park_h, rotateAngle, "CARPARK"){
        this->count = count;
    }
};

class Car {

public:
    vector<Vector2D> points;
    GLuint textureId=0;
    float t=0, epsilon = 0.007;
    Vector2D position,V0 = Vector2D(360,-50),V1 = Vector2D(400,400);
    Vector2D destination = Vector2D(400,200);
    class Polygon area;
    Car();
    Car(GLuint texture, Vector2D position);
    Car(GLuint texture, Vector2D position, class Polygon area);
    GLuint getTextureId(){
        return textureId;
    }
    void setTexture(string txt_path, int lx, int ly){
        textureId = GlutWindow::loadTGATexture(txt_path,lx,ly);
    }
    void draw();
    Vector2D traj(float t);
    float move(double ft, float velocity);
};


#endif //EXEMPLE_TEXTURE_GI_CAR_H
