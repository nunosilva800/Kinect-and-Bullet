#include "Primitiva.h"


/*
 * Construtores
 */
Primitiva::Primitiva(void){
	this->vertices = new vector<vector3*>();
	this->normais = new vector<vector3*>();
    Math::identity4(this->objTransMat);
}


Primitiva::~Primitiva(void){

}


/*
 * Metodos
 */

/**
 *
 */
void Primitiva::setOpenglMat(float* glMat){
    for(int i = 0 ; i < 16 ; i++){
		this->objTransMat[i] = glMat[i];
	}
}

/**
 *
 */
void Primitiva::setVertices(vector<vector3*>* vs){
	if(!vs) return ;
	for(std::vector<vector3*>::iterator it = vs->begin() ; it != vs->end() ; it++){
		this->vertices->push_back(Math::newVector3((*it)->x,(*it)->y,(*it)->z));
	}
	this->hasvertices = true;
}

/**
 *
 */
void Primitiva::setNormals(vector<vector3*>* ns){
	if(!ns) return ;
	for(std::vector<vector3*>::iterator it = ns->begin() ; it != ns->end() ; it++){
		this->vertices->push_back(Math::newVector3((*it)->x,(*it)->y,(*it)->z));
	}
	this->hasvertices = true;
}

/**
 *
 */
void Primitiva::transform(float xx, float yy, float zz){

}

/**
 *
 */
void Primitiva::rotate(float axis, float xx, float yy, float zz){

}

/**
 *
 */
void Primitiva::scale(float xx, float yy, float zz){

}

void Primitiva::setInitialPosition(float xx, float yy, float zz)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float coiso[16]; 

	glTranslatef(xx,yy,zz);
	glGetFloatv(GL_MODELVIEW_MATRIX, coiso); //
	this->setOpenglMat(&coiso[0]); //
}

