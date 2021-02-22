//
// Created by Kiui on 04/01/2021.
//

#include "Car.h"

Car::Car(){
    textureId = 0;
}
Car::Car(GLuint texture, Vector2D& position){
    textureId = texture;
    this->position = position;
}

Car::Car(GLuint texture, Vector2D& source, Vector2D& destination, Vector2D& V0, Vector2D& V1,  int polygon, double angle): source(source), destination(destination), V0(V0), V1(V1) {
    textureId = texture;
    this->angle = angle;
    this->position = source;

}
void Car::draw(){
    glBindTexture(GL_TEXTURE_2D,textureId);
    glPushMatrix();
    glTranslatef(position.x,position.y,0.0);
    glRotatef(angle,0,0,1);
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
    a = (source - destination)*2+V0+V1;
    b = (destination - source)*3-(V0*2)-V1;
    c = V0;
    d = source;
    return a*pow(t,3)+b*pow(t,2)+c*t+d;
}

float Car::move(double dt, float velocity) {
    if(progress<max){
        if(t>1){
            progress++;
            if (progress!=max) {
                Vector2D P = path[progress].second.first.pos;
                Vector2D V = (path[progress].second.first.e.B - path[progress].second.first.e.A).rightOrtho();
                update(P, V);
            }
        }
        Vector2D Pi= traj(t),Pj;
        points.push_back(Pi);
        cout <<"( "<<Pi.x<<" , "<<Pi.y<<")"<<endl;
        cout <<"SOURCE ( "<<source.x<<" , "<<source.y<<")"<<endl;
        double d=dt*velocity;
        while(d>0 && t<1.0){
            elapsedTime += dt;
            t+=epsilon;
            Pj = traj(t);
            angle = getAngle(Pi,Pj);
            points.push_back(Pj);
            cout <<"( "<<Pj.x<<" , "<<Pj.y<<")"<<endl;
            position.x=Pj.x;
            position.y=Pj.y;
            d-= (Pj-Pi).norm();
            Pi = Pj;
        }
    }
    else {
        cout<<"end move " << elapsedTime  <<endl;

        if(!reachedDesGate){
            reachedDesGate = true;
            triggerOtherCar = true;
            goal = path[max-1].first;
            cout <<"goal"<<goal<<endl;
            progress--;
        }
    }
    return t;
}
