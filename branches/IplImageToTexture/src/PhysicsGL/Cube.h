#ifndef _CUBE_H_
#define _CUBE_H_

#include "Primitiva.h"

class Cube : public Primitiva{
    // The eight corners of the cube
	public:
		Cube();
		Cube(float lado);
		Cube(vector3 *v1, vector3 *v2);
		Cube(Cube *c);
		~Cube();

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

		void contructCube(float size);
		void setCorFace(int face, float r, float g, float b);
		void setInitialColors();
		void copyColors(Cube *c);

};


#endif
