#ifndef _MYMATH
#define _MYMATH

#include <cstdlib>
#include <vector>

typedef struct{
    float x;
    float y;
    float z;
}vector3;

class Math{
	public:
		static vector3* newVector3();
		static vector3* newVector3(float xx, float yy, float zz);
		static void identity4(float matrix[]);
};

//vector3* newVector3();
//vector3* newVector3(float xx, float yy, float zz);
//void identity4(float matrix[]);


#endif