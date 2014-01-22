#include "vector.h"
#include <math.h>
#include <stdio.h>
#define PI 3.14159265359

float getMagnitudVector(vector3d *v)
{
	return sqrt(pow(v->x,2)+pow(v->y,2)+pow(v->z,2));
}

float getProductoPunto(vector3d *a, vector3d *b)
{
	return a->x*b->x+a->y*b->y+a->z*b->z;
}

void getProductoEscalar(vector3d *v, float escalar, vector3d *vr)
{
	vr->x=v->x*escalar;
	vr->y=v->y*escalar;
	vr->z=v->z*escalar;
}
void getProductoCruz1x3(vector3d *a, vector3d *b, vector3d *c)
{
	//ax ay az
	//bx by bz
	c->x=a->y*b->z-a->z*b->y;
	c->y=a->x*b->z-a->z*b->x;
	c->z=a->x*b->y-a->y*b->x;
}
float getAnguloEntreVectores(vector3d *a, vector3d *b)
{
	float productoPunto=getProductoPunto(a, b);
	float magnitudA=getMagnitudVector(a);
	float magnitudB=getMagnitudVector(b);
	printf("anguloARotar = %f\n",acos(productoPunto/(magnitudA*magnitudB)));
	return acos(productoPunto/(magnitudA*magnitudB));
}

float radiansToDegrees(float radian)
{
	return radian*180/PI;
}

float degreesToRadians(float degrees){
	return degrees*PI/180;
}

void getRotatedVector(vector3d *k, vector3d *v, vector3d *vrot, float angle)
{
	vector3d vesc, vesc2, vesc3;
	vector3d vcruz;
	vector3d vcruz2;
	float kvprodPunto;
	vector2d kescalarkvprodpunto;
	getProductoEscalar(v, cos(angle),&vesc); 	//v*cos(ang) --primer argumento vesc
	getProductoCruz1x3(k, v, &vcruz); 			//k x v
	getProductoEscalar(&vcruz, sin(angle), &vcruz2);	//k x v * sen(ang) --segundo argumento vcruz2
	kvprodPunto=getProductoPunto(k, v);		//k . v
	getProductoEscalar(k, kvprodPunto, &vesc2); //k (k.v)
	getProductoEscalar(&vesc2, 1-cos(angle), &vesc3); //k (k.v) * (1-cos(ang)) --tercer argumento vesc3
	vrot->x=vesc.x+vcruz2.x+vesc3.x;
	vrot->y=vesc.y+vcruz2.y+vesc3.y;
	vrot->z=vesc.z+vcruz2.z+vesc3.x;


}

void copyVectorValues(vector3d *orig, vector3d *dest)
{
	dest->x = orig->x;
	dest->y = orig->y;
	dest->z = orig->z;
}
void getUnitaryVector(vector3d *v, vector3d *unitary)
{
	unitary->x=v->x/sqrt(pow(v->x,2)+pow(v->y,2)+pow(v->z,2));
	unitary->y=v->y/sqrt(pow(v->x,2)+pow(v->y,2)+pow(v->z,2));
	unitary->z=v->z/sqrt(pow(v->x,2)+pow(v->y,2)+pow(v->z,2));
}

void traslateVector(vector3d *orig, vector3d *direc, vector3d *dest, float length)
{ //avanza en direccion del vector una longitud determinada
	vector3d unitary;
	getUnitaryVector(direc, &unitary);
	dest->x=orig->x+(unitary.x)*length;
	dest->y=orig->y+(unitary.y)*length;
	dest->z=orig->z+(unitary.z)*length;
} 
void traslateVectorNormal(vector3d *orig, vector3d *direc, vector3d *dest, float length)
{ //avanza en direccion del vector una longitud determinada
	vector3d unitary;
	vector3d normal;
	normal.x=-orig->z;
	normal.y=orig->y;
	normal.z=orig->x;
	getUnitaryVector(&normal, &unitary);
	dest->x=orig->x+(unitary.x)*length;
	dest->y=orig->y+(unitary.y)*length;
	dest->z=orig->z+(unitary.z)*length;
} 