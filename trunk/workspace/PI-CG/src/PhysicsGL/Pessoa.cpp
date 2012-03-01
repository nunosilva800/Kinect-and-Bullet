#include "Pessoa.h"

/*
 * Construtores
 */
Pessoa::Pessoa(){
}


Pessoa::~Pessoa(){

}


/*
 * Metodos
 */

void Pessoa::draw(){
	std::vector<Primitiva*>::iterator it = this->getPrimitivas()->begin();

	for( ; it != this->getPrimitivas()->end() ; it++){
		//(*it)->draw();
	}
}
