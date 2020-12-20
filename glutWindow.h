//
// Created by bpiranda on 18/06/2019.
//

#ifndef GLUTINTERFACE_GLUTWINDOW_H
#define GLUTINTERFACE_GLUTWINDOW_H
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

#ifndef M_PI
#define M_PI    3.1415926535897932384626433832795
#endif
#ifdef USE_FREEGLUT
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h> // for APPLE only
#endif


using namespace std;

class GlutWindow {
    public:
        enum textAlign {ALIGN_LEFT,ALIGN_CENTER,ALIGN_RIGHT};
        enum coordinateMode {X_FIRST,Y_FIRST,PROPORTIONAL,LONGEST_FIRST,FIXED};
        unsigned int width; //!< width of the client area
        unsigned int height; //!< height of the client area
        coordinateMode currentCM=PROPORTIONAL; //!< default coordinateMode is PROPORTIONAL
        /**
         * @brief Constuctor of GlutWindow object
         * @param argc: main function parameter (number of parameters)
         * @param argv: main function parameters (string data)
         * @param title: title of the window
         * @param w: initial width of the window
         * @param h: initial height of the window
         * @param cm: coordinalet Mode, defines inside coordinate system. Takes values in {X_FIRST,Y_FIRST,PROPORTIONAL,LONGEST_FIRST}.
         * - X_FIRST: 0<x<1 / 0<y<height/width
         * - Y_FIRST: 0<x<width/height / 0<y<1
         * - PROPORTIONAL: 0<x<1 / 0<y<1
         * - LONGEST_FIRST: is Y_FIRST if width>height, X_FIRST otherwise
         */
    GlutWindow(int argc,char **argv,const string &title="",unsigned int w=800,unsigned int h=800,coordinateMode cm=PROPORTIONAL);
    ~GlutWindow()= default;
    /**
     * @brief Start function that open the window and run the event manager, must be run by the user
     */
    void start();
    /**
     * A static function that gives the width of the window
     * @return the client area width
     */
    static unsigned int getWindowWidth();
    /**
     * A static function that gives the height of the window
     * @return the client area height
     */
    static unsigned int getWindowHeight();
    /**
     * @brief Draw a line from (x0,y0) to (x1,y1)
     * @param x0: x position first point
     * @param y0: y position first point
     * @param x1: x position second point
     * @param y1: y position second point
     */
    static void line(double x0,double y0, double x1, double y1);
    /**
     * @brief Draw a rectangle from the corner at (x,y)
     * @param x: x position of the corner
     * @param y: y position of the corner
     * @param lx: length along the x axis
     * @param ly: length along the y axis
     */
    static void rect(double x,double y, double lx, double ly);
    /**
     * @brief Draw a filled box from the corner at (x,y)
     * @param x: x position of the corner
     * @param y: y position of the corner
     * @param lx: length along the x axis
     * @param ly: length along the y axis
     */
    static void box(double x,double y, double lx, double ly);
    /**
     * @brief Draw a polyline on nsub point along an ellipse centered at (cx,cy) with (rx,ry) as radii.
     * @param cx: x position of the center
     * @param cy: y position of the center
     * @param rx: radius along the x axis
     * @param ry: radius along the y axis
     * @param nsub: number of points defining the shape
     */
    static void ellipse(double cx,double cy, double rx, double ry,int nsub=20);
    /**
     * @brief Draw a polygon on nsub point along an ellipse centered at (cx,cy) with (rx,ry) as radii.
     * @param cx: x position of the center
     * @param cy: y position of the center
     * @param rx: radius along the x axis
     * @param ry: radius along the y axis
     * @param nsub: number of points defining the shape
     */
    static void fillEllipse(double cx,double cy, double rx, double ry,int nsub=20);
    /**
     * @brief Draw a text in the client window
     * @param x: x text position
     * @param y: y text position
     * @param str: string to draw
     * @param align: alignment direction takes values in {ALIGN_LEFT,ALIGN_CENTER,ALIGN_RIGHT}. Default value is left alignment
     * @param font: GLUT bitmap font. Defautl value is a large font, GLUT_BITMAP_HELVETICA_18
     */
    static void drawText(double x,double y,const string &str,textAlign align=ALIGN_LEFT,void *font=GLUT_BITMAP_HELVETICA_18);
    /**
     * @fn void mouseToScreenCoordinates(int mx,int my,double &sx,double &sy)
     * @brief Conversion from mouse coordinates to screen coordinates
     * @param mx: x mouse position
     * @param my: y mouse position
     * @param sx: x screen coordinates (out)
     * @param sy: y screen coordinates (out)
     */
    void mouseToScreenCoordinates(int mx,int my,double &sx,double &sy);
 /**
 * @brief Must be overridden in the user code to initialise client variables.
 * Called just before opening the window.
 */
    virtual void onStart()=0;
/**
 * @brief Must be overridden in the user code to free the client memory.
 */
    virtual void onQuit()=0;
/**
 * @fn void onDraw()
 * @brief Must be overridden in the user code to draw the scene.
 * The coordinate system of the client area is:
 * -(0,0) at bottom left corner
 * -(width/height,1) at top right corner (if width>height)
 * -(height/width,1) at top right corner (if width<height)
 */
    virtual void onDraw()=0;
/**
 * @brief Called after each drawing of a frame and the treatment of all messages
 * @param w: new width of the window
 * @param h: new height of the window
 */
    virtual void onUpdate(double dt) {};
/**
 * @brief Called when the windows is opened or resized
 * @param w: new width of the window
 * @param h: new height of the window
 */
    virtual void onReshape(int w,int h) {};
/**
 * @brief Called when a keyboard key is pressed
 * @param c: key character ('q' and ESC key stop the program)
 * @param x: x scene coordinates of the mouse pointer
 * @param y: y scene coordinates of the mouse pointer
 */
    virtual void onKeyPressed(unsigned char c,double x,double y) {};
/**
 * @brief Called when one mouse button is pressed
 * @param button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON or GLUT_RIGHT_BUTTON
 * @param x: x scene coordinates of the mouse pointer
 * @param y: y scene coordinates of the mouse pointer
 */
    virtual void onMouseDown(int button,double x,double y) {};
/**
 * @brief Called when one mouse button is released
 * @param button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON or GLUT_RIGHT_BUTTON
 * @param x: x scene coordinates of the mouse pointer
 * @param y: y scene coordinates of the mouse pointer
 */
    virtual void onMouseUp(int button,double x,double y) {};
/**
 * @brief Called when the mouse is moved with button up
 * @param x: x scene coordinates of the mouse pointer
 * @param y: y scene coordinates of the mouse pointer
 */
    virtual void onMouseMove(double x,double y) {};
/**
 * @brief Called when the mouse is moved with button down
 * @param x: x scene coordinates of the mouse pointer
 * @param y: y scene coordinates of the mouse pointer
 */
    virtual void onMouseDrag(double x,double y) {};
/**
 * @brief static function that returns true if 2D point (px,py) is inside the rectangle (x0,y0)-(x0+lx,y0+ly)
 * @param px: x parameter of the tested point
 * @param py: y parameter of the tested point
 * @param x0: left corner position of the rectangle
 * @param y0: bottom corner position of the rectangle
 * @param lx: width of the rectangle
 * @param ly: height of the rectangle
 * @result true if 2D point (px,py) is inside the rectangle (x0,y0)-(x0+lx,y0+ly)
 */
    static bool isInRect(double px,double py,double x0,double y0,double lx,double ly) {
        return px>=x0 && py>=y0 && px<=x0+lx && py<=y0+ly;
    }
/**
* @brief static function that returns an OpenGL texture id for the new texture
* @param title: TGA image file name
* @param tw: width of the texture in file
* @param th: height of the texture in file
* @return OpenGL texture id
*/
    static GLuint loadTGATexture(const string &title,int &tw,int &th);
/**
 * @brief static function that returns TGA image data
 * @param title: TGA image file name
 * @param tw: width of the texture in file
 * @param th: height of the texture in file
 * @param flip: vertical flip of the image
 * @return image data
 */
    static unsigned char *lectureTGA(const string &title, int&tw, int&th ,bool flip=false);
};

#endif //GLUTINTERFACE_GLUTWINDOW_H
