//
// Created by bpiranda on 18/06/2019.
//
#include <fstream>
#include <cstring>
#include "glutWindow.h"

static void drawFunc();
static void reshapeFunc(int w,int h);
static void idleFunc();
static void keyFunc(unsigned char c, int x, int y);
static void mouseFunc(int button,int state,int x,int y);
static void motionFunc(int x,int y);
static void passiveMotionFunc(int x,int y);
static void quitFunc();

static GlutWindow *singlePtr= nullptr;

GlutWindow::GlutWindow(int argc,char **argv,const string &title,unsigned int w,unsigned int h,coordinateMode cm)
:width(w),height(h),currentCM(cm) {
    assert(singlePtr==nullptr);

    singlePtr=this;
    glutInit(&argc, argv);
    // create the window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width,height);
    glutCreateWindow(title.c_str());

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0,0.0,0.0,1.0); // background color

    glutDisplayFunc(drawFunc);
    glutReshapeFunc(reshapeFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutKeyboardFunc(keyFunc);
#ifdef USE_FREEGLUT
    glutCloseFunc(quitFunc);
    glutWMCloseFunc(quitFunc);
#endif
}

void GlutWindow::start() {
    singlePtr->onStart();
    glutMainLoop();
}

unsigned int GlutWindow::getWindowWidth() {
    return singlePtr->width;
}

unsigned int GlutWindow::getWindowHeight() {
    return singlePtr->height;
}

void GlutWindow::mouseToScreenCoordinates(int mx, int my, double &sx, double &sy) {
    switch (currentCM) {
        case X_FIRST: {
            sx = mx/(double)singlePtr->width;
            sy = (singlePtr->height-my)/(double)singlePtr->width;
        }
        break;
        case Y_FIRST: {
            sx = mx/(double)singlePtr->height;
            sy = (singlePtr->height-my)/(double)singlePtr->height;
        }
        break;
        case PROPORTIONAL: {
            sx = mx/(double)singlePtr->width;
            sy = (singlePtr->height-my)/(double)singlePtr->height;
        }
        break;
        case LONGEST_FIRST: {
            if (singlePtr->width>singlePtr->height) {
                sx = mx/(double)singlePtr->height;
                sy = (singlePtr->height-my)/(double)singlePtr->height;
            } else {
                sx = mx/(double)singlePtr->width;
                sy = (singlePtr->height-my)/(double)singlePtr->width;
            }
        }
        break;
        case FIXED: {
            sx = mx;
            sy = singlePtr->height-my;
        }
    }
}

void GlutWindow::line(double x0,double y0, double x1, double y1) {
    glBegin(GL_LINES);
    glVertex2d(x0,y0);
    glVertex2d(x1,y1);
    glEnd();
}

void GlutWindow::box(double x, double y, double lx, double ly) {
    glBegin(GL_QUADS);
    glVertex2d(x,y);
    glVertex2d(x+lx,y);
    glVertex2d(x+lx,y+ly);
    glVertex2d(x,y+ly);
    glEnd();
}

void GlutWindow::rect(double x, double y, double lx, double ly) {
    glBegin(GL_LINE_LOOP);
    glVertex2d(x,y);
    glVertex2d(x+lx,y);
    glVertex2d(x+lx,y+ly);
    glVertex2d(x,y+ly);
    glEnd();
}

void GlutWindow::ellipse(double cx, double cy, double lx, double ly,int nsub) {
    double x,y,a;
    glBegin(GL_LINE_LOOP);
    for (int i=0;i<nsub; i++) {
        a = 2.0*M_PI*i/nsub;
        x = cx+lx*cos(a);
        y = cy+ly*sin(a);
        glVertex2d(x,y);
    }
    glEnd();
}

void GlutWindow::fillEllipse(double cx, double cy, double lx, double ly,int nsub) {
    double x,y,a;
    glBegin(GL_POLYGON);
    for (int i=0;i<nsub; i++) {
        a = 2.0*M_PI*i/nsub;
        x = cx+lx*cos(a);
        y = cy+ly*sin(a);
        glVertex2d(x,y);
    }
    glEnd();
}

void GlutWindow::drawText(double x, double y, const string &str, GlutWindow::textAlign align,void *font) {
    switch (align) {
        case ALIGN_LEFT : break;
        case ALIGN_CENTER: {
            x -= singlePtr->currentCM==FIXED? 0.5 * glutBitmapLength(font, (unsigned char *) str.c_str()):
                 0.5 * glutBitmapLength(font, (unsigned char *) str.c_str()) / singlePtr->width;
        } break;
        case ALIGN_RIGHT:
            x-= singlePtr->currentCM==FIXED? glutBitmapLength(font,(unsigned char*)str.c_str()):
                    glutBitmapLength(font,(unsigned char*)str.c_str()) / (double)singlePtr->width;
        break;
    }

    glRasterPos2d(x,y);
    for (auto c :str) {
        glutBitmapCharacter(font, c);
    }
}

/*********************************************************/
/* frame drawing function                                */
static void drawFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    singlePtr->onDraw();
    glutSwapBuffers();
}

/*********************************************************/
/* Window size update function                           */
/* width: width of the drawing area                      */
/* height: width of the drawing area                     */
static void reshapeFunc(int w,int h) {
    singlePtr->width=(unsigned int)w;
    singlePtr->height=(unsigned int)h;
    glViewport(0,0,w,h);
    // initialize Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (singlePtr->currentCM)  {
        case GlutWindow::coordinateMode::X_FIRST: {
            double r = (double)h/(double)w;
            gluOrtho2D(0,1.0,0,r);
        } break;
        case GlutWindow::coordinateMode::Y_FIRST: {
            double r = (double)w/(double)h;
            gluOrtho2D(0,r,0,1.0);
        } break;
        case GlutWindow::coordinateMode::LONGEST_FIRST: {
            if (w>h) {
                double r = (double)w/(double)h;
                gluOrtho2D(0,r,0,1.0);
            } else {
                double r = (double)h/(double)w;
                gluOrtho2D(0,1.0,0,r);
            }
        } break;
        case GlutWindow::coordinateMode::PROPORTIONAL: {
            gluOrtho2D(0,1.0,0,1.0);
        } break;
        case GlutWindow::coordinateMode ::FIXED: {
            gluOrtho2D(0,w,0,h);
        } break;
    }

    // initialize ModelView matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    singlePtr->onReshape(w,h);
}

/*********************************************************/
/* Animation function                                    */
static void idleFunc() {
    static int initTime = glutGet(GLUT_ELAPSED_TIME); // ms
    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    double dt=(currentTime-initTime)/1000.0;

    initTime = currentTime;
    singlePtr->onUpdate(dt);
    glutPostRedisplay();
}

/*********************************************************/
/* Key pressed function                                  */
/* c: key pressed character                              */
/* x,y: mouse coordinates                                */
static void keyFunc(unsigned char c, int x, int y) {
    switch(c) {
        case 27:
        case 'q' : // quit
#ifdef USE_FREEGLUT
            glutLeaveMainLoop();
#else
            singlePtr->onQuit();
            exit(1);
#endif
            break;
        default: {
            double wx,wy;
            singlePtr->mouseToScreenCoordinates(x,y,wx,wy);
            singlePtr->onKeyPressed(c, wx, wy);
        }
    }
    glutPostRedisplay();
}

/*********************************************************/
/* Mouse clicked function                                */
/* button: sum of pressed buttons id                     */
/* state: action                                         */
/* x,y: mouse coordinates                                */
static void mouseFunc(int button,int state,int x,int y) {
    double wx,wy;
    singlePtr->mouseToScreenCoordinates(x,y,wx,wy);
    if (state==GLUT_DOWN) {
        singlePtr->onMouseDown(button,wx,wy);
    } else {
        singlePtr->onMouseUp(button,wx,wy);
    }
}

/*********************************************************/
/* Mouse move function (with button pressed)             */
/* x,y: mouse coordinates                                */
static void motionFunc(int x,int y) {
    double wx,wy;
    singlePtr->mouseToScreenCoordinates(x,y,wx,wy);
    singlePtr->onMouseDrag(wx,wy);
}

/*********************************************************/
/* Mouse move function (without button pressed)          */
/* x,y: mouse coordinates                                */
static void passiveMotionFunc(int x,int y) {
    double wx,wy;
    singlePtr->mouseToScreenCoordinates(x,y,wx,wy);
    singlePtr->onMouseMove(wx,wy);
}

static void quitFunc() {
    singlePtr->onQuit();
}

GLuint GlutWindow::loadTGATexture(const string &title,int &tw,int &th) {
    unsigned char *image;
    GLuint id=0;

    if (!(image=lectureTGA(title,tw,th))) {
        cerr << "Error : can't open " << title << endl;
    } else {
        glGenTextures(1,&id);
        glBindTexture(GL_TEXTURE_2D,id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,tw,th,GL_RGBA,GL_UNSIGNED_BYTE,image);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        delete [] image;
    }
    return id;
}

unsigned char *GlutWindow::lectureTGA(const string &title, int&tw, int&th ,bool flip) {
    const int DEF_targaHeaderLength=12;
    const char DEF_targaHeaderContent[]="\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00";

    ifstream fin;
    char *pData;
    streampos maxLen=0;

    fin.open(title,ios::binary);
    if (!fin.is_open()) return nullptr;

    // calc the length of the file.
    fin.seekg (0, ios::end);
    maxLen = fin.tellg();
    fin.seekg (0, ios::beg);

    // allocate enough memory for the file image
    pData = new char [int(maxLen)];

    // read data
    fin.read(pData,maxLen);

    fin.close();

    int commentOffset = int( (unsigned char)*pData );
    if( memcmp( pData + 1, DEF_targaHeaderContent, DEF_targaHeaderLength - 1 ) != 0 ) {
        cerr << "Not TGA image file format: " << title << endl;
        return nullptr;
    }
    unsigned char smallArray[ 2 ];

    memcpy( smallArray, pData + DEF_targaHeaderLength + 0, 2 );
    tw = smallArray[ 0 ] + smallArray[ 1 ] * 0x0100;

    memcpy( smallArray, pData + DEF_targaHeaderLength + 2, 2 );
    th = smallArray[ 0 ] + smallArray[ 1 ] * 0x0100;

    memcpy( smallArray, pData + DEF_targaHeaderLength + 4, 2 );
    int depth = smallArray[ 0 ];
//	int pixelBitFlags = smallArray[ 1 ];

    if( ( tw <= 0 ) || ( th <= 0 ) )
        return nullptr;

    // Only allow 24-bit and 32-bit!
    bool is24Bit( depth == 24 );
    bool is32Bit( depth == 32 );
    if( !( is24Bit || is32Bit ) )
        return nullptr;

    // Make it a BGRA array for now.
    int bodySize(tw*th*4);
    unsigned char * pBuffer = new unsigned char[ bodySize ];
    if( is32Bit ) {
        // Easy, just copy it.
        memcpy( pBuffer, pData + DEF_targaHeaderLength + 6 + commentOffset, bodySize );
    } else if( is24Bit ) {
        int bytesRead = DEF_targaHeaderLength + 6 + commentOffset;
        for( int loop = 0; loop < bodySize; loop += 4, bytesRead += 3 ) {
            memcpy( pBuffer + loop, pData + bytesRead, 3 );
            pBuffer[ loop + 3 ] = 255;			// Force alpha to max.
        }
    }
    else return nullptr;

    // Swap R & B (convert to RGBA).
    for( int loop = 0; loop < bodySize; loop += 4 ) {
        unsigned char tempC = pBuffer[ loop + 0 ];
        pBuffer[ loop + 0 ] = pBuffer[ loop + 2 ];
        pBuffer[ loop + 2 ] = tempC;
    }

    delete [] pData;

    if (flip) {
        unsigned char * pBufferRet = new unsigned char[ bodySize ], *ptr1=pBuffer+tw*(th-1)*4,*ptr2=pBufferRet;
        for (int loop=0; loop<th; loop++) {
            memcpy(ptr2,ptr1,tw*4);
            ptr2+=tw*4;
            ptr1-=tw*4;
        }
        delete [] pBuffer;
        return pBufferRet;
    }
    // Ownership moves out.
    return pBuffer;
}

