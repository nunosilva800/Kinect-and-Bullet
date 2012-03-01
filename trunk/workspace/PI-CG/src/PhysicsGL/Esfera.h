#ifndef _ESFERA_H_
#define _ESFERA_H_

#include "Primitiva.h"
#include <GL/glut.h>

class Esfera : public Primitiva{
	public:
		Esfera();
		Esfera(float raio, vector3 *centro);
		Esfera(float raio, float xx, float yy, float zz);
		Esfera(Esfera *e);
		~Esfera();


		float getRaio(){return this->raio;}
		vector3 *getCentro(){return this->centro;}

		void setRaio(float r){this->raio = r;}
		void setCentro(vector3 *c){this->centro = c;}
		void setCentro(float xx, float yy, float zz);

		void Draw();

	private:
		float raio;
		vector3 *centro;
};

#endif