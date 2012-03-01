#include "Esfera.h"

/*
 * Construtores
 */
Esfera::Esfera(){
	this->type = ESFERA;
	this->raio = 1.0;
	this->centro = new vector3();
	this->setCentro(0.0,0.0,0.0);
}

Esfera::Esfera(float raio, vector3 *centro){
	this->type = ESFERA;
	this->raio = raio;
	this->centro = centro;
}

Esfera::Esfera(float raio, float xx, float yy, float zz){
	this->type = ESFERA;
	this->raio = raio;
	this->centro = new vector3();
	this->setCentro(xx,yy,zz);
}

Esfera::Esfera(Esfera *e){
	this->type = ESFERA;
	this->raio = e->raio;
	this->centro = new vector3();
	this->setCentro(e->getCentro()->x,e->getCentro()->y,e->getCentro()->z);
}

Esfera::~Esfera(){

}


/*
 * Metodos
 */
inline void Esfera::setCentro(float xx, float yy, float zz){
	this->centro->x = xx;
	this->centro->y = yy;
	this->centro->z = zz;
}

void Esfera::Draw(){
	//save current matrix
    glPushMatrix();
    //apply transformation matrix
    glLoadMatrixf(objTransMat);

    glColor3f(75,75,75);

	glPointSize(this->raio*100);
	glBegin(GL_POINTS);

		glVertex3f(0, 0,0);
	glEnd();
    //draw cube
	//glutWireSphere(this->raio,16,16);

    //get last matrix
    glPopMatrix();
}
