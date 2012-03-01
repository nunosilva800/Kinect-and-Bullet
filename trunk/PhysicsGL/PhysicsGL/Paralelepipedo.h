#ifndef _PARALELEPIPEDO_H_
#define _PARALELEPIPEDO_H_

#include <GL/glew.h>
#include "Primitiva.h"

class Paralelepipedo : public Primitiva{

public: 
    vector3 * halfExtention;

    // The eight corners of the cube
public:
    Paralelepipedo();
    Paralelepipedo(float largura, float altura, float profundidade );
    Paralelepipedo(vector3 *v1, vector3 *v2);
    Paralelepipedo(Paralelepipedo *p);
    ~Paralelepipedo();

    void Draw();


private:
    vector3 cores[6];
    enum {
        TOP_LEFT = 0,
        TOP_RIGHT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT
    };
    enum {
        FACE_FRONT,
        FACE_LEFT = 4,
        FACE_BACK = 8,
        FACE_RIGHT = 12,
        FACE_TOP = 16,
        FACE_BOTTOM = 20
    };

    void contructParalelepipedo(float largura, float profundidade, float altura);
    void setCorFace(int face, float r, float g, float b);
    void setInitialColors();
    void copyColors(Paralelepipedo *p);

};


#endif