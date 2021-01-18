#include <iostream>
#include <cstdlib>
#include <vector>
#include "glutWindow.h"

#include "TinyToys.h"
using namespace std;
#define INF  99999
/*class TexturedWindow: public GlutWindow {
private:
    GLuint park,stop;
    Car redcar;
    bool move = false;
public:

    TexturedWindow(const string &title,int argc,char **argv):
        GlutWindow(argc,argv,title,1024,640,FIXED) {
        park=0;

    };

    void onStart() override;
    void onDraw() override;
    void onQuit() override;
    void onKeyPressed(unsigned char c, double x,double y);
};
void TexturedWindow::onKeyPressed(unsigned char c, double x,double y) {
    redcar.move(10,100);
}
void TexturedWindow::onStart() {
    int lx,ly;
    park = loadTGATexture("../Textures/parks/carpark.tga",lx,ly);
    assert(park!=0);
    stop = loadTGATexture("../Textures/parks/carstop.tga",lx,ly);
    assert(stop!=0);
    redcar = Car(loadTGATexture("../Textures/cars/redcar.tga",lx,ly),Vector2D(40,250));
    assert(redcar.getTextureId()!=0);
    glClearColor(0.5,0.5,0.5,1.0); // background color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TexturedWindow::onDraw() {
    if(!move) {
        redcar.move(10,100);
        move = true;
    }


    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f,1.0f,1.0f);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D,park);
    glPushMatrix();
    glTranslatef(redcar.position.x,redcar.position.y,0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0);
    glVertex2f(-40.0,-20.0);
    glTexCoord2f(1.0,0.0);
    glVertex2f(40.0,-20.0);
    glTexCoord2f(1.0,1.0);
    glVertex2f(40.0,20.0);
    glTexCoord2f(0.0,1.0);
    glVertex2f(-40.0,20.0);
    glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,stop);
    glPushMatrix();
    glTranslatef(redcar.destination.x,redcar.destination.y,0.0);
    glRotatef(45,0,0,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0);
    glVertex2f(-40.0,-20.0);
    glTexCoord2f(1.0,0.0);
    glVertex2f(40.0,-20.0);
    glTexCoord2f(1.0,1.0);
    glVertex2f(40.0,20.0);
    glTexCoord2f(0.0,1.0);
    glVertex2f(-40.0,20.0);
    glEnd();
    glPopMatrix();



    redcar.draw();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPushMatrix();

    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<redcar.points.size();i++){
        glVertex2d(redcar.points[i].x,redcar.points[i].y);
    }
    glEnd();
    glPopMatrix();

}
void TexturedWindow::onQuit() {
    //glDeleteTextures(1,&textureid);
}

int main(int argc,char **argv) {
    TexturedWindow gui("Cars textures", argc,argv);
    gui.start();
    return 0;
}
*/

int main(int argc,char **argv) {
    
    vector<Room> rooms;
    rooms.push_back(Room("Alpha",  110, 450, 0xff0000));
    rooms.push_back(Room("Bravo", 400, 700, 0xff7700));
    rooms.push_back(Room("Charlie", 1100, 720, 0xffff00));
    rooms.push_back(Room("Delta", 510, 470, 0x00ff00));
    rooms.push_back(Room("Echo", 370, 120, 0x00ffff));
    rooms.push_back(Room("Foxrot", 680, 40, 0x0000ff));
    rooms.push_back(Room("Golf", 670, 230, 0x7700ff));
    rooms.push_back(Room("Hotel", 900, 280, 0x915136));
    rooms.push_back(Room("India", 1140, 350, 0x777777));
    rooms.push_back(Room("Juliett", 970, 80, 0xffffff));
    
    int V = 10;
    vector<vector<int> > graph
            = {         {INF, INF, 1, 1, INF, INF, INF, INF, INF, INF},
                        {INF, INF, INF, INF, INF, 1, INF, INF, INF, INF},
                        {1, INF, INF, INF, INF, INF, INF, 1, INF, INF},
                        {1, INF, INF, INF, 1, INF, INF, INF, INF, INF},
                        {INF, INF, INF, 1, INF, INF, INF, INF, INF, 1},
                        {INF, 1, INF, INF, INF, INF, INF, INF, INF, 1},
                        {INF, INF, 1, INF, INF, INF, INF, INF, INF, INF},
                        {INF, INF, INF, INF, 1, INF, INF, INF, 1, INF},
                        {INF, INF, INF, INF, INF, INF, 1, 1, INF, INF},
                        {INF, INF, INF, INF, INF, 1, INF, INF, 1, INF} };
    
    
   /* Polygon p(vertices);
    p.tri();
    Mesh m(p.getTriangulation(), vertices, argc, argv);
    m.start();*/
    TinyToys toys(rooms, graph, argc, argv);
    toys.start();
    return 0;
}
