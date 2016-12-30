#ifndef __JJC__OPENGL__CALLBACKS__HPP__
#define __JJC__OPENGL__CALLBACKS__HPP__

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <math.h>
#include <mutex>

#include <glInGameObject.hpp>
#include <gameObjectCallbacks.hpp>

#ifndef HEIGHT
    #define HEIGHT 480
#endif // HEIGHT

#ifndef WIDTH
    #define WIDTH 640
#endif // WIDTH

// conditional compilation for debugging purposes
//#define DEBUGGING_MODE // symbolic definition

namespace _GL_ {

// function declarations
void initGL(int, char*);
void drawScene(void);
void keyboardHandler(unsigned char, int, int);
void specialKeyHandler(int, int, int);

#ifdef DEBUGGING_MODE
bool freeloop = true;
bool dostep   = true;
#endif // DEBUGGING_MODE

// array of in-game objects
std::vector<ingameObj*> dlist; // dynamic list
std::vector<ingameObj*> slist; // static list
std::vector<ingameObj*> klist; // keyboard list

// callback and info controlling camera
ingameObj* camFocus;
void(*cameraUpdate)(ingameObj*);

// initialization function
void initGL(int argc_, char* argv_[]) {
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInit(&argc_, argv_);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow(argv_[0]);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // callbacks get specified here
    glutDisplayFunc(drawScene); // not calling this function, but specifying it as a callback
    glutIdleFunc(drawScene);
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);
}

// display function must return void and
// accept void arguments
void drawScene(void) {
#ifdef DEBUGGING_MODE
if(freeloop || dostep) {
#endif // DEBUGGING_MODE

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cameraUpdate(camFocus);

    uint32_t vecSize = dlist.size(); // to avoid calling the function more than once
    for(uint32_t c = 0; c < vecSize; c++) { // lol, get it? C++!
        dlist[c]->update();
    }

    vecSize = slist.size();
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glLoadIdentity();
    for(uint32_t c = 0; c < vecSize; c++) {
        glCallList(slist[c]->obj);
    }

    glutSwapBuffers();

#ifdef DEBUGGING_MODE
    dostep = false; // for stepping through the program, only one loop will go through before stopping,
                    // but the freeloop has precedence, conditional statement disappears when DEBUGGING_MODE is undefined
    }
#endif // DEBUGGING_MODE
}

void keyboardHandler(unsigned char key, int x, int y) {
    uint32_t vecSize = klist.size();
    for(uint32_t c = 0; c < vecSize; c++) {
        klist[c]->keyboard(key, x, y);
    }
    drawScene();
}

void specialKeyHandler(int key, int x, int y) {
    uint32_t vecSize = klist.size();
    for(uint32_t c = 0; c < vecSize; c++) {
        klist[c]->special(key, x, y);
    }
    drawScene();
}

}

#endif // __JJC__OPENGL__CALLBACKS__HPP__
