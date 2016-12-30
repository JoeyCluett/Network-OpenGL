#ifndef __JJC__OPENGL__GAMEOBJECT__BEHAVIOR__HPP__
#define __JJC__OPENGL__GAMEOBJECT__BEHAVIOR__HPP__

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

namespace gObj {

void playerUpdate(ingameObj* obj) {

    static int frames = 0;
    frames++;

    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 5.0f, 0.0f);
    glRotatef((float)frames, 1.0f, 0.0f, 0.0f);
    glCallList(obj->obj);

}

void cylinderUpdate(ingameObj* obj) {
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(obj->x, obj->y, obj->z);
    glRotatef(obj->y_rot, 0.0f, 1.0f, 0.0f);
    glCallList(obj->obj);

/*
    // render xray for object
    glDepthFunc(GL_GREATER);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCallList(obj->obj);
*/
}

void xrayCylinderUpdate(ingameObj* obj) {

    glDepthFunc(GL_ALWAYS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLoadIdentity();
    glTranslatef(obj->x, obj->y, obj->z);
    glRotatef(obj->y_rot, 0.0f, 1.0f, 0.0f);
    glCallList(obj->obj);

/*    glDepthFunc(GL_GREATER);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(obj->x, obj->y, obj->z);
    glRotatef(obj->y_rot, 0.0f, 1.0f, 0.0f);
    glCallList(obj->obj);
*/
}

void cylinderKeyboard(ingameObj* obj, unsigned char key, int x, int y) {
    //cout << "cylinder callback made" << endl;

    switch(key) {
        case 'w':
            obj->z += 2.3f;
            break;
        case 'a':
            obj->x += 2.3f;
            break;
        case 's':
            obj->z -= 2.3f;
            break;
        case 'd':
            obj->x -= 2.3f;
            break;
        case 'q':
            obj->y_rot += 1.0f;
            break;
        case 'e':
            obj->y_rot -= 1.0f;
            break;
        default:
            break;
    }
}

void cylinderSpecial(ingameObj* obj, int key, int x, int y) {
    //std::cout << "cylinder special callback" << std::endl;
    switch(key) {
        case GLUT_KEY_LEFT:
            obj->y_rot += 2.0f;
            break;
        case GLUT_KEY_RIGHT:
            obj->y_rot -= 2.0f;
            break;
        default:
            break;
    }
}

void environmentUpdate(ingameObj* obj) {
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glLoadIdentity();
    glCallList(obj->obj); // render flat environment, grey
}

void cameraUpdateCallback(ingameObj* obj) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1, 0.01, 400.0);
    gluLookAt(0.0f, 20.0f, -20.0f,         // location of camera, center of area, looking down
                  obj->x, obj->y, obj->z, // point camera is looking at
                  0.0f, 1.0f, 0.0f);      // 'up' is... up
}

void loadedImageUpdate(ingameObj* obj) {
    //std::cout << "image callback" << std::endl;
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 100.0f);

    //Select the texture.
    glBindTexture(GL_TEXTURE_2D , obj->obj);

    glBegin(GL_QUADS);

        //Bottom left of the texture and quad.
        glTexCoord2f(0.0f , 0.0f); glVertex3f(-100.0f, 0.0f,  0.0f);
        //Bottom right fo the texture and quad.
        glTexCoord2f(1.0f , 0.0f); glVertex3f(100.0f, 0.0f,   0.0f);
        //Top right of the texture and quad.
        glTexCoord2f(1.0f , 1.0f); glVertex3f(100.0f, 100.0f,  0.0f);
        //Top left of the texture and quad.
        glTexCoord2f(0.0f , 1.0f); glVertex3f(-100.0f, 100.0f, 0.0f);

    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    //glutSwapBuffers();
}

void tankUpdate(ingameObj* obj) {
    static int zz = 0;
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glLoadIdentity();
    glRotatef((GLfloat)zz, 0.0f, 1.0f, 0.0f);
    glCallList(obj->obj); // render flat environment, grey

    glDepthFunc(GL_GREATER);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCallList(obj->obj);

    zz++;
}

void tankXray(ingameObj* obj) {
    static int zz = 0;
    glDepthFunc(GL_ALWAYS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLoadIdentity();
    glRotatef((GLfloat)zz, 0.0f, 1.0f, 0.0f);
    glCallList(obj->obj); // render flat environment, grey
    zz++;
}

}

#endif // __JJC__OPENGL__GAMEOBJECT__BEHAVIOR__HPP__
