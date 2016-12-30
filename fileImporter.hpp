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

        ingameObj* myObj;

        do {
            _type     = importFile->first_attribute("type");
            _path     = importFile->first_attribute("path");
            _name     = importFile->first_attribute("name");
            _behavior = importFile->first_attribute("behavior");
            _keyboard = importFile->first_attribute("keyboard");
            _model    = importFile->first_attribute("model");

            if(_type == NULL) {
                cerr << "[impErr]  File type not found." << endl;
            } else {
                cout << "[import]  File type: " << _type->value() << endl;
            }

            if(_path == NULL) {
                cerr << "[impErr]  File name not found" << endl;
                return;
            } else {
                cout << "[import]  Parsing file: " << _path->value() << endl;
                if(strcmp(_type->value(), "xml") == 0) {
                    useXML = true;
                    objParse::parseBotFile(_path->value());
                    myObj = new ingameObj; // dont need to delete this one because we still need to access memory associated with the first one

                    if(strcmp(_model->value(), "solid") == 0) {
                        myObj->obj = objParse::getBot(objParse::GLfloatVec);
                        cout << "[import]  Model type: solid" << endl;
                    } else if(strcmp(_model->value(), "wireframe") == 0) {
                        myObj->obj = objParse::getWireframe(objParse::GLfloatVec);
                        cout << "[import]  Model type: wireframe" << endl;
                    } else {
                        cerr << "[impErr]  Invalid model type value" << endl;
                    }

                } else if(strcmp(_type->value(), "stlbinary") == 0) {
                    stl::openFile(_path->value());
                    stl::Model* tempModel = stl::parseFileBinary(); // this should be deleted immediately after use to avoid huge memory leak

                    if(strcmp(_model->value(), "solid") == 0) {
                        myObj->obj = stl::getBot(tempModel);
                        cout << "[import]  Model type: solid" << endl;
                        delete tempModel;
                    } else if(strcmp(_model->value(), "wireframe") == 0) {
                        myObj->obj = stl::getWireframe(tempModel);
                        cout << "[import]  Model type: wireframe" << endl;
                        delete tempModel;
                    } else {
                        cerr << "[impErr]  Invalid model type value" << endl;
                    }

                } else {
                    cerr << "[impErr]  Invalid model file type" << endl;
                }

                if(_name == NULL) {
                    cerr << "[impErr]  Name not given" << endl;
                } else {
                    // need to make sure name is somewhere on the heap and not released when rapidxml object is deleted
                    uint32_t stringLength = _name->value_size() + 1;
                    char* __name = new char[stringLength];
                    for(uint32_t c = 0; c < stringLength; c++) {
                        __name[c] = 0; // ensure space for null-terminated c-style string
                    }
                    for(uint32_t c = 0; c < _name->value_size(); c++) {
                        __name[c] = _name->value()[c]; // copy string into global heap memory
                    }
                    myObj->name = __name;
                }

                if(strcmp(_behavior->value(), "static") == 0) {
                    staticObjs->push_back(myObj);
                } else if(strcmp(_behavior->value(), "dynamic") == 0){
                    dest->push_back(myObj); // put object on dynamic list

                    // test keyboard input attribute
                    if(strcmp(_keyboard->value(), "true") == 0) {
                        keyboardObjs->push_back(myObj); // put object on keyboard list
                    } else if(strcmp(_keyboard->value(), "false") != 0) {
                        cerr << "[impErr]  keyboard undefined" << endl;
                    } else {
                        cout << "[import]  object has no keyboard input" << endl;
                    }

                } else {
                    cerr << "[impErr]  object behavior undefined" << endl;
                }
                if(useXML)
                    delete objParse::GLfloatVec; // only delete if actually used in parsing file
                useXML = false;
            }

        } while(importFile = importFile->next_sibling("file"));

    }

    // this should not be called in time sensitive code
    ingameObj* getObjByName(char* objname) {
        for(uint32_t c = 0; c < dest->size(); c++) {
            if(strcmp(objname, dest->at(c)->name) == 0)
                return dest->at(c);
        }
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
