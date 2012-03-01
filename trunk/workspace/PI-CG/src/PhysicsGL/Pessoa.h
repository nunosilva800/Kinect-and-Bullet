#ifndef _PESSOA_H_
#define _PESSOA_H_

//using namespace std;
//#include <stdio.h>
#include "Primitiva.h"
#include <vector>

class Pessoa{
	public:
		Pessoa();
		~Pessoa();

		std::vector<Primitiva*>* getPrimitivas(){return this->primitivas;}
		void addPrimitiva(Primitiva *p){this->primitivas->push_back(p);}

		void draw();

	private:
		std::vector<Primitiva*> *primitivas;
};

#endif