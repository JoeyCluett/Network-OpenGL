#ifndef __JJC__OPENGL__INGAMEOBJECT__HPP__
#define __JJC__OPENGL__INGAMEOBJECT__HPP__

#include <iostream>

class ingameObj {
public:
    GLuint obj;
    char* name; // referenced by debugger

    // describes the objects location in the 3D environment
    GLfloat x = 0.0f;
    GLfloat y = 0.0f;
    GLfloat z = 0.0f;

    GLfloat x_rot = 0.0f;
    GLfloat y_rot = 0.0f;
    GLfloat z_rot = 0.0f;

    GLfloat x_scale = 0.0f;
    GLfloat y_scale = 0.0f;
    GLfloat z_scale = 0.0f;

    // default constructor
    ingameObj() {
        updateCallback   = defaultCallback;
        keyboardCallback = defaultKeyboard;
        specialCallback  = defaultSpecial;
    }

    // must be set to true before rendering
    bool updateCallbackSet = false;

    // functions to set function pointers
    void setUpdateCallback(void(*update_fptr)(ingameObj*)) {
        updateCallback = update_fptr;
    }
    void setKeyboardCallback(void(*keyboard_fptr)(ingameObj* u, unsigned char key, int x, int y)) {
        keyboardCallback = keyboard_fptr;
    }
    void setSpecialCallback(void(*special_fptr)(ingameObj* u, int key, int x, int y)) {
        specialCallback = special_fptr;
    }

    // functions called by game loop
    void update(void) {
        updateCallback(this);
    }
    void keyboard(unsigned char key, int x, int y) {
        keyboardCallback(this, key, x, y);
    }
    void special(int key, int x, int y) {
        specialCallback(this, key, x, y);
    }

protected:
    static void defaultCallback(ingameObj* u) {
        ;
    }
    static void defaultKeyboard(ingameObj* u, unsigned char key, int x, int y) {
        ;
    }
    static void defaultSpecial(ingameObj* u, int key, int x, int y) {
        ;
    }

    // callbacks used to perform operations on game objects
    void(*updateCallback)(ingameObj*);
    void(*keyboardCallback)(ingameObj* obj, unsigned char key, int x, int y);
    void(*specialCallback)(ingameObj* obj, int key, int x, int y);
};

#endif // __JJC__OPENGL__INGAMEOBJECT__HPP__
