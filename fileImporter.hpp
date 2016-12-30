#ifndef __JJC__OPENGL__FILEIMPORTER__HPP__
#define __JJC__OPENGL__FILEIMPORTER__HPP__

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

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <STL-Parser.hpp>

#include <glInGameObject.hpp>
#include <objectParser.hpp>

// macros to shorten up the code needed to do string comparisons
#define BEHAVIOR_IS(w) strcmp(_behavior->value(),w)==0
#define KEYBOARD_IS(w) strcmp(_keyboard->value(),w)==0
#define TYPE_IS(w)     strcmp(_type->value(),w)==0
#define MODEL_IS(w)    strcmp(_model->value(),w)==0

// log and error output macros
#define ERROR1(w)       cerr << "[impErr]  " << w << endl
#define ERROR2(w, d)    cerr << "[impErr]  " << w << d << endl
#define ERROR3(w, d, x) cerr << "[impErr]  " << w << d << x << endl

#define LOG1(w)         cout << "[import]  " << w << endl
#define LOG2(w, d)      cout << "[import]  " << w << d << endl
#define LOG3(w, d, x)   cout << "[import]  " << w << d << x << endl
class fileImporter {
public:
    void setImportDest(std::vector<ingameObj*>* dest,
            std::vector<ingameObj*>* staticObjs,
            std::vector<ingameObj*>* keyboardObjs) {

        this->dest = dest;
        destSet = true;

        this->staticObjs = staticObjs;
        staticSet = true;

        this->keyboardObjs = keyboardObjs;
        keyboardSet = true;
    }

    void import_(char* filename) { // file describing which other assets to bring into the OpenGL environment

        if(!destSet || !staticSet || !keyboardSet) {
            cerr << "Storage not given" << endl;
            return; // if there is no place to put parsed objects, dont parse them to begin with
        }

        bool useXML = false;

        rapidxml::xml_document<> doc; // create xml document object
        ifstream myfile(filename);
        vector<char> fileBuffer((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>( ));
        fileBuffer.push_back('\0');

        doc.parse<rapidxml::parse_trim_whitespace>(&fileBuffer[0]); // parse the contents of the file
        rapidxml::xml_node<>* root = doc.first_node("root"); // find our root node

        rapidxml::xml_node<>* importFile = root->first_node("file");

        if(importFile == NULL) {
            cerr << "No 'file' node found" << endl;
            return;
        }

        rapidxml::xml_attribute<>* _type;
        rapidxml::xml_attribute<>* _path;
        rapidxml::xml_attribute<>* _name;
        rapidxml::xml_attribute<>* _behavior;
        rapidxml::xml_attribute<>* _keyboard;
        rapidxml::xml_attribute<>* _model;

        ingameObj* myObj; // value of this pointer changes with every object

        do {
            _type     = importFile->first_attribute("type");
            _path     = importFile->first_attribute("path");
            _name     = importFile->first_attribute("name");
            _behavior = importFile->first_attribute("behavior");
            _keyboard = importFile->first_attribute("keyboard");
            _model    = importFile->first_attribute("model");

            myObj = new ingameObj; // constant across all file types

            char* nameStore = new char[_name->value_size() + 1];
            memset(nameStore, '\0', _name->value_size());
            strcpy(nameStore, _name->value());
            myObj->name = nameStore; // object names are needed for all file types

            LOG2("Name: ", myObj->name);

            if(BEHAVIOR_IS("dynamic")) {
                LOG1("Behavior is dynamic");
                dest->push_back(myObj);
            } else if(BEHAVIOR_IS("static")) {
                LOG1("Behavior is static");
                staticObjs->push_back(myObj);
            } else {
                ERROR1("Behavior is undefined");
            }

            if(KEYBOARD_IS("true")) {
                LOG1("Object uses keyboard");
                keyboardObjs->push_back(myObj);
            } else if(KEYBOARD_IS("false")) {
                LOG1("Object does not use keyboard");
            } else {
                ERROR1("Keyboard usage is invalid");
            }

            if(TYPE_IS("stlbinary")) {

                LOG1("File type is binary .stl");
                stl::openFile(_path->value());
                stl::Model* m = stl::parseFileBinary();

                m->r_ = 1.0f;
                m->g_ = 1.0f;
                m->b_ = 1.0f;

                // get the colors from file
                rapidxml::xml_attribute<>* _color = importFile->first_attribute("r");
                if(_color != 0)
                    m->r_ = (GLfloat)atof(_color->value());
                _color = importFile->first_attribute("g");
                if(_color != 0)
                    m->g_ = (GLfloat)atof(_color->value());
                _color = importFile->first_attribute("b");
                if(_color != 0)
                    m->b_ = (GLfloat)atof(_color->value());

                if(MODEL_IS("solid")) {
                    myObj->obj = stl::getBot(m);
                } else if(MODEL_IS("wireframe")) {
                    myObj->obj = stl::getWireframe(m);
                } else {
                    ERROR1("Model type is invalid");
                }

                delete m;

            } else if(TYPE_IS("xml")) {

                LOG1("FIle type is XML");
                objParse::parseBotFile(_path->value());

                if(MODEL_IS("solid")) {
                    myObj->obj = objParse::getBot(objParse::GLfloatVec);
                } else if(MODEL_IS("wireframe")) {
                    myObj->obj = objParse::getWireframe(objParse::GLfloatVec);
                } else {
                    ERROR1("Model type is invalid");
                }

                delete objParse::GLfloatVec;

            } else {
                ERROR1("File type is invalid");
            }

        } while(importFile = importFile->next_sibling("file"));

    }

    // this should not be called in time sensitive code
    ingameObj* getObjByName(char* objname) {
        LOG2("Searching for: ", objname);
        for(uint32_t c = 0; c < dest->size(); c++) {
            cout << "\t" << dest->at(c)->name << endl;
            if(strcmp(objname, dest->at(c)->name) == 0)
                return dest->at(c);
        }
        ERROR3("Object name: ", objname, " not found");
        return 0;
    }

private:

    bool destSet     = false;
    bool staticSet   = false;
    bool keyboardSet = false;

    std::vector<ingameObj*>* dest;
    std::vector<ingameObj*>* staticObjs;
    std::vector<ingameObj*>* keyboardObjs;
};

#endif // __JJC__OPENGL__FILEIMPORTER__HPP__
