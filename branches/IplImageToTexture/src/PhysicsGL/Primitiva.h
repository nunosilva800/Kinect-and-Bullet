#ifndef _PRIMITIVA
#define _PRIMITIVA

#include "MyMath.h"

using namespace std;

//class MMotionState;
//class MApp;

class Primitiva{
	//friend MMotionState;
	//friend MApp;

	public:
		Primitiva(void);
		~Primitiva(void);

		
		vector<vector3*>* getVertices(){return this->vertices;}
		vector<vector3*>* getNormals(){return this->normais;}
		bool hasVertices(){return this->hasvertices;}
		bool hasNormais(){return this->hasnormais;}
		float* getOpenglMat(){return this->objTransMat;}
		
		void setOpenglMat(float* glMat);
		void setVertices(vector<vector3*>* vs);
		void setNormals(vector<vector3*>* ns);
		float objTransMat[16];

	protected:
		
		bool hasvertices;
		vector<vector3*> *vertices;
		bool hasnormais;
		vector<vector3*> *normais;

		virtual void transform(float xx, float yy, float zz);
		virtual void rotate(float axis, float xx, float yy, float zz);
		virtual void scale(float xx, float yy, float zz);

};

#endif