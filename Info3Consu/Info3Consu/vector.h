#ifndef VECTOR_H
#define VECTOR_H

typedef struct{
	float x;
	float y;
	float z;
}vector3d;

typedef struct{
	float w1;
	float w2;
	float w3;
	float w4;
	float w5;
	float w6;
}wallDistance;

typedef struct{
	float x;
	float y;
}vector2d;

float getMagnitudVector(vector3d *v);
float getProductoPunto(vector3d *a, vector3d *b);
void getProductoEscalar(vector3d* v, float escalar, vector3d* vr);
float getAnguloEntreVectores(vector3d* a, vector3d* b);
float radiansToDegrees(float radian);
float degreesToRadians(float degrees);
void getRotatedVector(vector3d* k, vector3d* v, vector3d* vrot, float angle);
void copyVectorValues(vector3d *orig, vector3d *dest);
void getUnitaryVector(vector3d *v, vector3d *unitary);
void traslateVector(vector3d *orig, vector3d *direc, vector3d *dest, float length);
void traslateVectorNormal(vector3d *orig, vector3d *direc, vector3d *dest, float length);
#endif