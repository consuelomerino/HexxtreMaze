#include "vector.h"

float getAnguloEntreVectores(vector3d* a, vector3d* b)
{
	float productoEscalar=getProductoEscalar(a, b);
}

float getMagnitudVector(vector3d *v)
{
	return sqrt((v->x)^2+(v->y)^2+(v->z)^2);
}
float getProductoEscalar(vector3d *a, vector3d *b)
{
	return a->x*b->x+a->y*b->y+a->z*b->z;
}