#include "Cube.h"


/*
 * Construtores
 */
Cube::Cube(){
	Math::identity4(this->objTransMat);
	this->setInitialColors();
	this->contructCube(1);
 };

Cube::Cube(float lado){
	Math::identity4(this->objTransMat);
	this->setInitialColors();
	this->contructCube(lado);
}

Cube::Cube(vector3 *v1, vector3 *v2){
	Math::identity4(this->objTransMat);
	this->setInitialColors();
	this->contructCube(fabs(v1->x - v2->x));
}

Cube::Cube(Cube *c){
	this->setOpenglMat(c->getOpenglMat());
	this->setVertices(c->getVertices());
	this->setNormals(c->getNormals());
	this->copyColors(c);
}

Cube::~Cube(){

}

/*
 * Metodos
 */

/**
 *
 */
void Cube::contructCube(float size){
	if(size == 0) size = 1;
	if(size < 0) size = -size;
	//Up Face
	this->vertices->push_back(Math::newVector3(size,size,-size));
	this->vertices->push_back(Math::newVector3(-size,size,-size));
	this->vertices->push_back(Math::newVector3(-size,size,size));
	this->vertices->push_back(Math::newVector3(size,size,size));
	//Down Face
	this->vertices->push_back(Math::newVector3(size,-size,size));
	this->vertices->push_back(Math::newVector3(-size,-size,size));
	this->vertices->push_back(Math::newVector3(-size,-size,-size));
	this->vertices->push_back(Math::newVector3(size,-size,-size));
	//Front Face
	this->vertices->push_back(Math::newVector3(size,size,size));
	this->vertices->push_back(Math::newVector3(-size,size,size));
	this->vertices->push_back(Math::newVector3(-size,-size,size));
	this->vertices->push_back(Math::newVector3(size,-size,size));
	//Back Face
	this->vertices->push_back(Math::newVector3(size,-size,-size));
	this->vertices->push_back(Math::newVector3(-size,-size,-size));
	this->vertices->push_back(Math::newVector3(-size,size,-size));
	this->vertices->push_back(Math::newVector3(size,size,-size));
	//Left Face
	this->vertices->push_back(Math::newVector3(-size,size,size));
	this->vertices->push_back(Math::newVector3(-size,size,-size));
	this->vertices->push_back(Math::newVector3(-size,-size,-size));
	this->vertices->push_back(Math::newVector3(-size,-size,size));
	//Right Face
	this->vertices->push_back(Math::newVector3(size,size,-size));
	this->vertices->push_back(Math::newVector3(size,size,size));
	this->vertices->push_back(Math::newVector3(size,-size,size));
	this->vertices->push_back(Math::newVector3(size,-size,-size));

	this->hasvertices = true;
}

 /**
  *
  */
void Cube::setCorFace(int face, float r, float g, float b){
	this->cores[face].x = r;
	this->cores[face].y = g;
	this->cores[face].z = b;
}
/**
 *
 */
void Cube::copyColors(Cube *c){
	this->setCorFace(0,c->cores[0].x,c->cores[0].y,c->cores[0].z);
	this->setCorFace(1,c->cores[1].x,c->cores[1].y,c->cores[1].z);
	this->setCorFace(2,c->cores[2].x,c->cores[2].y,c->cores[2].z);
	this->setCorFace(3,c->cores[3].x,c->cores[3].y,c->cores[3].z);
	this->setCorFace(4,c->cores[4].x,c->cores[4].y,c->cores[4].z);
	this->setCorFace(5,c->cores[5].x,c->cores[5].y,c->cores[5].z);
}

/**
 *
 */
void Cube::setInitialColors(){
	this->setCorFace(0,0.0f,1.0f,0.0f);
	this->setCorFace(1,1.0f,0.5f,0.0f);
	this->setCorFace(2,1.0f,0.0f,0.0f);
	this->setCorFace(3,1.0f,1.0f,0.0f);
	this->setCorFace(4,0.0f,0.0f,1.0f);
	this->setCorFace(5,1.0f,0.0f,1.0f);
}

 /**
  *
  */
void Cube::Draw(){
	if(!this->hasVertices()) return ;
	//save current matrix
    glPushMatrix();
    //apply transformation matrix
    glLoadMatrixf(objTransMat);

    //draw cube
    glBegin(GL_QUADS);
	vector3 *aux;
	glColor3f(1,1,1);
	for(int i = 0 ; i < 6 ; i++){
		glColor3f(this->cores[i].x,this->cores[i].y,this->cores[i].z);
		aux = this->vertices->at(i * 4 + 0);
		glVertex3f(aux->x, aux->y,aux->z);
		aux = this->vertices->at(i * 4 + 1);
		glVertex3f(aux->x, aux->y,aux->z);
		aux = this->vertices->at(i * 4 + 2);
		glVertex3f(aux->x, aux->y,aux->z);
		aux = this->vertices->at(i * 4 + 3);
		glVertex3f(aux->x, aux->y,aux->z);
	}
    glEnd();

    //get last matrix
    glPopMatrix();
	
};
