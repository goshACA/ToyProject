//
// Created by Kiui on 04/01/2021.
//

#include "Car.h"
Car::Car(){
    textureId = 0;
}
Car::Car(GLuint texture, Vector2D position){
    textureId = texture;
    this->position = position;
}
void Car::draw(){
    glBindTexture(GL_TEXTURE_2D,textureId);
    glPushMatrix();
    glTranslatef(position.x,position.y,0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0);
    glVertex2f(-40.0,-16.0);
    glTexCoord2f(1.0,0.0);
    glVertex2f(40.0,-16.0);
    glTexCoord2f(1.0,1.0);
    glVertex2f(40.0,16.0);
    glTexCoord2f(0.0,1.0);
    glVertex2f(-40.0,16.0);
    glEnd();
    glPopMatrix();
}

Vector2D Car::traj(float t) {
    Vector2D a,b,c,d;
    a = (position - destination)*2+V0+V1;
    b = (destination - position)*3-(V0*2)-V1;
    c = V0;
    d = position;
    return a*pow(t,3)+b*pow(t,2)+c*t+d;
}

float Car::move(double dt, float velocity) {
    Vector2D Pi= traj(t),Pj;
    points.push_back(Pi);
    cout <<"( "<<Pi.x<<" , "<<Pi.y<<")"<<endl;

    double d=dt*velocity;
    while(d>0 && t<1.0){
        t+=epsilon;
        Pj = traj(t);
        points.push_back(Pj);
        cout <<"( "<<Pj.x<<" , "<<Pj.y<<")"<<endl;
        d-= (Pj-Pi).norm();
        Pi = Pj;
    }
    return t;
}



