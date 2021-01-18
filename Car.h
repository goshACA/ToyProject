//
// Created by Kiui on 04/01/2021.
//

#ifndef EXEMPLE_TEXTURE_GI_CAR_H
#define EXEMPLE_TEXTURE_GI_CAR_H
#include "glutWindow.h"
#include <vector>
#include "Vector2D.h"
#include "Polygon.h"


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
