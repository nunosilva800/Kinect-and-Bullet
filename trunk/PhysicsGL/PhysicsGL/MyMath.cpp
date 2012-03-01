#include "MyMath.h"

vector3* Math::newVector3(){
	vector3* vec = (vector3*)malloc(sizeof(vector3));
	vec->x = 0.0;
	vec->y = 0.0;
	vec->z = 0.0;

	return vec;
}

vector3* Math::newVector3(float xx, float yy, float zz){
	vector3* vec = (vector3*)malloc(sizeof(vector3));
	vec->x = xx;
	vec->y = yy;
	vec->z = zz;

	return vec;
}


//vector3* newVector3(){
//	vector3* vec = (vector3*)malloc(sizeof(vector3));
//	vec->x = 0.0;
//	vec->y = 0.0;
//	vec->z = 0.0;
//
//	return vec;
//}
//
//vector3* newVector3(float xx, float yy, float zz){
//	vector3* vec = (vector3*)malloc(sizeof(vector3));
//	vec->x = xx;
//	vec->y = yy;
//	vec->z = zz;
//
//	return vec;
//}

void Math::identity4(float matrix[]){

}