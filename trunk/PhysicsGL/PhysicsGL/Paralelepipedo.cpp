#include "Paralelepipedo.h"

#include "Paralelepipedo.h"


/*
 * Construtores
 */
Paralelepipedo::Paralelepipedo() : 
halfExtention(Math::newVector3(1,1,2))
{
    type = PARALELEPIPEDO;
	Math::identity4(this->objTransMat);
	this->setInitialColors();
	this->contructParalelepipedo(1, 1, 2);
 };

Paralelepipedo::Paralelepipedo(float largura, float altura, float profundidade ) :
halfExtention(Math::newVector3(largura,altura,profundidade))
{
    type = PARALELEPIPEDO;
	Math::identity4(this->objTransMat);
	this->setInitialColors();
	this->contructParalelepipedo(largura, altura,  profundidade);
}

Paralelepipedo::Paralelepipedo(vector3 *v1, vector3 *v2)
{
    float largura = abs(v1->x - v2->x);
    float altura = abs(v1->y - v2->y);
    float profundidade = abs(v1->z - v2->z);

    halfExtention = Math::newVector3(largura,altura, profundidade);
    type = PARALELEPIPEDO;
	Math::identity4(this->objTransMat);
	this->setInitialColors();
	this->contructParalelepipedo(largura,altura,profundidade);
}

Paralelepipedo::Paralelepipedo(Paralelepipedo *c) :
halfExtention(c->halfExtention)
{
    type = PARALELEPIPEDO;
	this->setOpenglMat(c->getOpenglMat());
	this->setVertices(c->getVertices());
	this->setNormals(c->getNormals());
	this->copyColors(c);
}

Paralelepipedo::~Paralelepipedo(){

}

 /*
  * Metodos
  */

/**
 *
 */
void Paralelepipedo::contructParalelepipedo(float largura, float altura , float  profundidade){
   
	if(largura == 0) largura = 1;
	if(largura < 0) largura = -largura;

	if(profundidade == 0) profundidade = 1;
	if(profundidade < 0) profundidade = -profundidade;

	if(altura == 0) altura = 1;
	if(altura < 0) altura = -altura;

	//Up Face
	this->vertices->push_back(Math::newVector3(largura,altura,-profundidade));
	this->vertices->push_back(Math::newVector3(-largura,altura,-profundidade));
	this->vertices->push_back(Math::newVector3(-largura,altura,profundidade));
	this->vertices->push_back(Math::newVector3(largura,altura,profundidade));
	//Down Face
	this->vertices->push_back(Math::newVector3(largura,-altura,profundidade));
	this->vertices->push_back(Math::newVector3(-largura,-altura,profundidade));
	this->vertices->push_back(Math::newVector3(-largura,-altura,-profundidade));
	this->vertices->push_back(Math::newVector3(largura,-altura,-profundidade));
	//Front Face
	this->vertices->push_back(Math::newVector3(largura,altura,profundidade));
	this->vertices->push_back(Math::newVector3(-largura,altura,profundidade));
	this->vertices->push_back(Math::newVector3(-largura,-altura,profundidade));
	this->vertices->push_back(Math::newVector3(largura,-altura,profundidade));
	//Back Face
	this->vertices->push_back(Math::newVector3(largura,-altura,-profundidade));
	this->vertices->push_back(Math::newVector3(-largura,-altura,-profundidade));
	this->vertices->push_back(Math::newVector3(-largura,altura,-profundidade));
	this->vertices->push_back(Math::newVector3(largura,altura,-profundidade));
	//Left Face
	this->vertices->push_back(Math::newVector3(-largura,altura,profundidade));
	this->vertices->push_back(Math::newVector3(-largura,altura,-profundidade));
	this->vertices->push_back(Math::newVector3(-largura,-altura,-profundidade));
	this->vertices->push_back(Math::newVector3(-largura,-altura,profundidade));
	//Right Face
	this->vertices->push_back(Math::newVector3(largura,altura,-profundidade));
	this->vertices->push_back(Math::newVector3(largura,altura,profundidade));
	this->vertices->push_back(Math::newVector3(largura,-altura,profundidade));
	this->vertices->push_back(Math::newVector3(largura,-altura,-profundidade));
}

 /**
  *
  */
void Paralelepipedo::setCorFace(int face, float r, float g, float b){
	this->cores[face].x = r;
	this->cores[face].y = g;
	this->cores[face].z = b;
}
/**
 *
 */
void Paralelepipedo::copyColors(Paralelepipedo *p){
	this->setCorFace(0,p->cores[0].x,p->cores[0].y,p->cores[0].z);
	this->setCorFace(1,p->cores[1].x,p->cores[1].y,p->cores[1].z);
	this->setCorFace(2,p->cores[2].x,p->cores[2].y,p->cores[2].z);
	this->setCorFace(3,p->cores[3].x,p->cores[3].y,p->cores[3].z);
	this->setCorFace(4,p->cores[4].x,p->cores[4].y,p->cores[4].z);
	this->setCorFace(5,p->cores[5].x,p->cores[5].y,p->cores[5].z);
}

/**
 *
 */
void Paralelepipedo::setInitialColors(){
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

 /**
  *
  */
void Paralelepipedo::Draw(){
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