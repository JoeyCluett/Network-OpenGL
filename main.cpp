/*

Network enabled game powered by OpenGL

Players are simple 3D shapes. server
passes position info between all players

Each client has a thread devoted to
network communication. network msgs
are yet to be developed.

*/

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

// image import
#include <SDL/SDL.h>

// glCallBacks.hpp has acces to these already
#define WIDTH  640
#define HEIGHT 480

#include <glCallBacks.hpp>
#include <objectParser.hpp>
#include <gameObjectCallbacks.hpp>
#include <fileImporter.hpp>

// test importing image
//#include </home/joe/Pictures/joePic.c>

using namespace std;
using namespace _GL_;

GLuint texture[1];

int main(int argc, char* argv[]) {
    // initialize OpenGL
    initGL(argc, argv);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //------------------------------------------------------------------
    // import the image into memory
    SDL_Surface* textureImage = SDL_LoadBMP("joePic.bmp");
    if(textureImage == 0) {
        cerr << "[_main_]  Failed to load image" << endl;
    } else {
        cout << "[_main_]  Loaded image" << endl;
        glGenTextures(1, &texture[0]);
        // generate the texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage->w, textureImage->h, 0,
                GL_RGB, GL_UNSIGNED_BYTE, textureImage->pixels);

        // linear filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // free the memory associated with the image surface
        if(textureImage)
            SDL_FreeSurface(textureImage);
    }
    //------------------------------------------------------------------

    //------------------------------------------------------------------
    // import objects into the rendering environment/pipeline
    fileImporter fi;
    fi.setImportDest(&dlist, &slist, &klist);
    fi.import_("assets/sampleImport.xml");

    ingameObj* myimage = new ingameObj;
    myimage->obj = texture[0];
    myimage->setUpdateCallback(gObj::loadedImageUpdate);
    myimage->name = "imageImport";
    _GL_::dlist.push_back(myimage);
    //------------------------------------------------------------------

    //------------------------------------------------------------------
    // specify callbacks for object already imported into environment
    //fi.getObjByName("playerObject")->setUpdateCallback(gObj::playerUpdate);

    fi.getObjByName("cylObject")->setUpdateCallback(gObj::cylinderUpdate);
    fi.getObjByName("cylObject")->setKeyboardCallback(gObj::cylinderKeyboard);
    fi.getObjByName("cylObject")->setSpecialCallback(gObj::cylinderSpecial);

    fi.getObjByName("cylxray")->setUpdateCallback(gObj::xrayCylinderUpdate);
    fi.getObjByName("cylxray")->setKeyboardCallback(gObj::cylinderKeyboard);
    fi.getObjByName("cylxray")->setSpecialCallback(gObj::cylinderSpecial);

    cout << "[_main_]  cylxray config finished" << endl;

    fi.getObjByName("tankBottom")->setUpdateCallback(gObj::tankUpdate);
    fi.getObjByName("tankWire")->setUpdateCallback(gObj::tankUpdate);
    //------------------------------------------------------------------

    //camFocus = fi.getObjByName("cylObject");
    camFocus = fi.getObjByName("tankBottom");
    cameraUpdate = gObj::cameraUpdateCallback;

    cout << "[_main_]  Object(s) loaded" << endl;

    // start main game loop
    glutMainLoop();

    return 1;
}























