#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "vector.h"
#define PI 3.14159265359

float infoCollision = false;
int vidas;
vector3d characterDirection;
vector3d characterPosition;
vector3d initialFront;
vector3d origin;
vector3d upDirection;
vector3d newDirection;
vector3d newPosition;
vector3d wallDir;
vector3d wallPos;
vector3d wallNew;
wallDistance wall;
premio p[7];
float initialDist;
// angle of rotation for the camera direction
float dangle = 0.0f;
float gangle = 0.0f;
int camera;

// actual vector representing the camera's direction
float lx=0.0f,ly=0.0f,lz=-1.0f;

// XZ position of the camera
float x=0.0f, y=1.0f, z=5.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float gammaAngle = 0.0f;
float charMove = 0;
float charRotate = 0;
int xOrigin = -1;
int yOrigin = -1;

void changeSize(int w, int h) {

    
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
    
	float ratio =  w * 1.0 / h;
    
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
    
	// Reset Matrix
	glLoadIdentity();
    
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
    
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
    
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan() {
    
glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body	
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);


// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);
    
    // Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();
    
    // Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);
}

float collision(vector3d* v1, vector3d* v2, vector3d* p){
    //printf("v1 = %f %f %f\n", v1->x, v1->y, v1->z);
    //printf("v2 = %f %f %f\n", v2->x, v2->y, v2->z);
    //printf("p = %f %f %f\n\n", p->x, p->y, p->z);
    //hallar la pendiente
    float m;
    if(v2->x - v1->x==0){
        //la pendiente es infinita
        //printf("aca");
        return p->x - v1->x;
    }else{
        m=(v2->z - v1->z)/(v2->x - v1->x);
        //printf("m = %f\n",m);
        //printf("v2: %f v1: %f \n", v2->z,v1->z);
        //hallar la pendiente de la normal
        if(m==0){
            return p->z - v1->z;
        }else{
            //float mp = -1/m;
            //printf("mp = %f\n",m);
            //hallar b de la ecuacion de la recta
            float b = (v1->z) - (v1->x) * m;
            //printf("b=%f-%f*%f=%f\n", p->z, p->x, mp, b);
            //hallar la distancia del punto a la recta
            float calc1 = (p->z)-m*(p->x)-b;
            float calc2 = pow(m,2)+1;
            float calc3 =fabsf((p->z)-m*(p->x)-b);
            float distancia = fabsf((p->z)-m*(p->x)-b)/sqrt(pow(m,2)+1);
            //printf("dist=fabsf(%f-%f*%f-%f)/sqrt(pow(%f,2)+1)=%f\n", p->z, mp, p->x, b, mp, distancia);
            if(infoCollision){
                printf("dist=fabsf(%f)/sqrt(%f)=%f\n", (p->z)-m*(p->x)-b, pow(m,2)+1, distancia);
                printf("dist= %f / %f = %f \n", fabsf((p->z)-m*(p->x)-b), sqrt(pow(m,2)+1), distancia);
            }
            return distancia;
        }
    }


}

void renderCharacter(void){ //personaje
	float anguloARotar = getAnguloEntreVectores(&initialFront, &characterDirection);
	float anguloARotar2 = characterDirection.x < 0?radiansToDegrees(anguloARotar):-radiansToDegrees(anguloARotar);
		
    glPushMatrix();
    glColor3f(1.0f, 1.0f , 0.0f);
    glTranslatef(characterPosition.x,0.1,characterPosition.z);
	glRotatef(anguloARotar2, 0.0f, 1.0f,0.0f);
	glutWireTeapot(2.0f);
    glPopMatrix();

	


}

void renderWall(vector3d *wallDir, float angle, float distance){
	float volume=2.0f;
	float floor=volume/2.0f;
    float anguloARotar = getAnguloEntreVectores(&initialFront, wallDir);
	float anguloARotar2;
	glPushMatrix();
    //glColor3f(wallDir->x, wallDir->y, wallDir->z);
	traslateVector(wallDir, wallDir, &wallPos, distance);
	//glTranslatef(wallPos.x,floor,wallPos.z);
	vector3d wall45grados1, wallVertice45gradosInt1, wallVertice45gradosExt1;
	vector3d wall45grados2, wallVertice45gradosInt2, wallVertice45gradosExt2;
	getRotatedVector(&upDirection, wallDir, &wall45grados1, degreesToRadians(angle/2));
	getRotatedVector(&upDirection, wallDir, &wall45grados2, -degreesToRadians(angle/2));
	glPushMatrix();
	glBegin(GL_LINES);
		//glVertex3d(-wallPos.x*20, -wallPos.y*20, -wallPos.z*20);
		//glVertex3d(wallPos.x*20, wallPos.y*20, wallPos.z*20);	
		//glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3d(wall45grados1.x*20, wall45grados1.y*20, wall45grados1.z*20);
		glVertex3d(0, 0, 0);
		//glColor3f(0.0f, 0.7f, 0.5f);
		glVertex3d(wall45grados2.x*20, wall45grados2.y*20, wall45grados2.z*20);
		glVertex3d(0, 0, 0);
	glEnd();
	glPopMatrix();
	glPushMatrix();
		glBegin(GL_LINES);
			//glVertex3d(-wallPos.x*20, -wallPos.y*20, -wallPos.z*20);
			//glVertex3d(wallPos.x*20, wallPos.y*20, wallPos.z*20);	
			//glColor3f(0.2f, 0.1f, 0.5f);
			glVertex3d(wallDir->x*20, wallDir->y*20, wallDir->z*20);
			glVertex3d(-wallDir->x*20, -wallDir->y*20, -wallDir->z*20);	
		glEnd();
	glPopMatrix();
	glPushMatrix();
		//glColor3f(0.2f, 0.1f, 0.7f);
		//glRotatef(0, 0.0f, 1.0f, 0.0f);
		//traslateVector(&wallPos, wallDir, &wallDirNew, 1);
		//glTranslatef(wallDir->x,0,wallDir->z);
		//anguloARotar2 = wallPos.x < 0? radiansToDegrees(anguloARotar) : -radiansToDegrees(anguloARotar);
		//glRotatef(anguloARotar2, 0.0f, 1.0f, 0.0f);
		traslateVector(&wall45grados1, &wall45grados1, &wallVertice45gradosInt1, getMagnitudVector(&wallPos));
		traslateVector(&wall45grados1, &wall45grados1, &wallVertice45gradosExt1, getMagnitudVector(&wallPos)+3);
		traslateVector(&wall45grados2, &wall45grados2, &wallVertice45gradosInt2, getMagnitudVector(&wallPos));
		traslateVector(&wall45grados2, &wall45grados2, &wallVertice45gradosExt2, getMagnitudVector(&wallPos)+3);
		glBegin(GL_QUADS); //6 caras en total
            //a b           e f
            //d c           h g
			vector3d a, b, c, d, e, f, g, h;
			/*a.x=-1;  a.y=1; a.z=1; //funciona
			b.x=1;   b.y=1; b.z=1;
			c.x=1;   c.y=-1; c.z=1;
			d.x=-1;  d.y=-1; d.z=1;
			e.x=-1;  e.y=1; e.z=-1;
			f.x=1;   f.y=1; f.z=-1;
			g.x=1;   g.y=-1; g.z=-1;
			h.x=-1;   h.y=-1; h.z=-1;*/
			a.x=wallVertice45gradosInt1.x; 	a.y=3; a.z=wallVertice45gradosInt1.z;
			b.x=wallVertice45gradosInt2.x; 	b.y=3; b.z=wallVertice45gradosInt2.z;
			c.x=wallVertice45gradosInt2.x; 	c.y=0; c.z=wallVertice45gradosInt2.z;
			d.x=wallVertice45gradosInt1.x; 	d.y=0; d.z=wallVertice45gradosInt1.z;
			e.x=wallVertice45gradosExt1.x; 	e.y=3; e.z=wallVertice45gradosExt1.z;
			f.x=wallVertice45gradosExt2.x; 	f.y=3; f.z=wallVertice45gradosExt2.z;
			g.x=wallVertice45gradosExt2.x; 	g.y=0; g.z=wallVertice45gradosExt2.z;
			h.x=wallVertice45gradosExt1.x; 	h.y=0; h.z=wallVertice45gradosExt1.z;
            //aca colision!!!!
    vector3d h2, g2;
    float c1, c2, c3, c4, c5, c6, doh, dog;
    c1=collision(&d, &c, &characterPosition);
    c2=collision(&h, &g, &characterPosition);
    c3=collision(&h, &d, &characterPosition);
    c4=collision(&g, &c, &characterPosition);
    doh=distancePointToPoint(&origin, &h);
    dog=distancePointToPoint(&origin, &g);
    traslateVector(&h, &g, &g2, dog);
    traslateVector(&g, &h, &h2, doh);
    c5=collision(&h, &h2, &characterPosition);
    c6=collision(&g, &g2, &characterPosition);
    
    if(     c1 < 5      && c2 < 5
       &&   c3 < doh     && c5 < doh
       &&   c4 < dog    && c6 < dog         ){
				glColor3f(1.0f, 1.0f, 1.0f);
        
        //printf("distancia de la colision %f\n", c1);
        //printf("distancia de la colision %f\n", c2);
        //printf("distancia de la colision %f\n", c3);
        //printf("distancia de la colision %f\n", c4);
        }else{
            	//glColor3f(1.0f, 1.0f, 0.0f);
            }
			//glVertex3f(0.0f,1.0f, 2.0f); glVertex3f(1.0f,1.0f, 2.0f);
			//glVertex3f(0.0f,0.0f, 2.0f); glVertex3f(1.0f,0.0f, 2.0f); 
			//Pared Interna
			//glVertex3f(0,1,1); glVertex3f(1,1,1); //0,1 1,1
			//glVertex3f(1,0,1); glVertex3f(0,0,1); //1,0 0,0
			glNormal3f(-wallDir->x, 0.0, -wallDir->z);
            glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z); //0,1 1,1

			glVertex3f(c.x,c.y,c.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0
			//Pared Externa
			glNormal3f(wallDir->x, wallDir->y, wallDir->z);
            //glVertex3f(0,1,0); glVertex3f(1,1,0); //0,1 1,1
			//glVertex3f(1,0,0); glVertex3f(0,0,0); //1,0 0,0
			glVertex3f(e.x,e.y,e.z); glVertex3f(f.x,f.y,f.z); //0,1 1,1
			glVertex3f(g.x,g.y,g.z); glVertex3f(h.x,h.y,h.z); //1,0 0,0
			//Pared costado
			//glColor3f(0.0f,1.0f, 0.0f);
			//glVertex3f(1.0f,0.0f, 1.0f); glVertex3f(1.0f,1.0f, 1.0f);
			//glVertex3f(1.0f,1.0f, 0.0f); glVertex3f(1.0f,0.0f, 0.0f);
			glNormal3f(-wallVertice45gradosInt1.z, wallVertice45gradosInt1.y, wallVertice45gradosInt1.x);
            glVertex3f(b.x,b.y,b.z); glVertex3f(f.x,f.y,f.z); //0,1 1,1
			glVertex3f(g.x,g.y,g.z); glVertex3f(c.x,c.y,c.z);//1,0 0,0
			
			//Pared costado
			//glVertex3f(0.0f,0.0f, 1.0f); glVertex3f(0.0f,1.0f, 1.0f);
			//glVertex3f(0.0f,1.0f, 0.0f); glVertex3f(0.0f,0.0f, 0.0f);
			//glColor3f(1.0f, 0.0f, 0.0f);
            glNormal3f(wallVertice45gradosInt2.z, wallVertice45gradosInt2.y, -wallVertice45gradosInt2.x);
            glVertex3f(a.x,a.y,a.z); glVertex3f(e.x,e.y,e.z); //0,1 1,1
			glVertex3f(h.x,h.y,h.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0
			//Base de abajo
			//glColor3f(0.0f,0.0f, 1.0f);
			//glVertex3f(0.0f,0.0f, 1.0f); glVertex3f(1.0f,0.0f, 1.0f);
			//glVertex3f(1.0f,0.0f, 0.0f); glVertex3f(0.0f,0.0f, 0.0f);	
			glVertex3f(c.x,c.y,c.z); glVertex3f(g.x,g.y,g.z); //0,1 1,1
			glVertex3f(h.x,h.y,h.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0			
			//Base de arriba
			glNormal3f(0.0, 1.0, 0.0);
            glVertex3f(b.x,b.y,b.z); glVertex3f(a.x,a.y,a.z);  //0,1 1,1
			glVertex3f(e.x,e.y,e.z); glVertex3f(f.x,f.y,f.z);  //1,0 0,0		

		glEnd(); 
		//glutSolidCube(volume);
	glPopMatrix();
    
	
	//glRotatef(0, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void makeGeometricShape(vector3d* direc, float distancia, int cantidadDeLados){
	vector3d direcUnitary;
	vector3d newDirec;
	//float length;
	float geometricAngle=360/cantidadDeLados;
	glPushMatrix();
	getUnitaryVector(direc, &direcUnitary);
    //cantidadDeLados = 1;
    //renderWall(&direcUnitary, geometricAngle, distancia);
	for(int i=0;i<cantidadDeLados-1;i++){
		getRotatedVector(&upDirection, &direcUnitary, &newDirec, degreesToRadians(geometricAngle));
		//length=2;
		renderWall(&newDirec, geometricAngle, distancia);
		copyVectorValues(&newDirec, &direcUnitary);
	}
	glPopMatrix();
}



void processNormalKeys(unsigned char key, int xx, int yy) {
    
    if (key == 27)
        exit(0);
    else if (key == 'r' || key == 'R')
        infoCollision = !infoCollision;
    else if (key == 'c' || key == 'C')
        camera++;
}

void pressKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP : 
			charMove = 0.5f; 
			break;
        case GLUT_KEY_DOWN : 
			charMove = -0.5f;
			break;
        case GLUT_KEY_LEFT : 
			charRotate=1.0f;
			break;
        case GLUT_KEY_RIGHT : 
			charRotate=-1.0f;
			break;
    }
}

void releaseKey(int key, int x, int y) {
    
    switch (key) {
        case GLUT_KEY_UP :
        case GLUT_KEY_DOWN : charMove = 0;  break;
        case GLUT_KEY_LEFT :
        case GLUT_KEY_RIGHT : charRotate = 0; break;
    }
}

void mouseMove(int x, int y) {
    
    // this will only be true when the left button is down
    if (xOrigin >= 0) {
        
		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		// update camera's direction
		lx = sin(dangle + deltaAngle);
        lz = -cos(dangle + deltaAngle);
	}
    if (yOrigin >=0) {
        gammaAngle = (y - yOrigin) * 0.02f;
        ly = -sin(gangle + gammaAngle);
        //lz = -cos(gangle + gammaAngle);
    }
}

void mouseButton(int button, int state, int x, int y) {
    
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
        
		// when the button is released
		if (state == GLUT_DOWN) {
			dangle += deltaAngle;
			gangle += gammaAngle;
            xOrigin = -1;
            yOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
            yOrigin = y;
		}
	}
}

void drawWalls(int value){
	int maxrand=100;
	float step=0.6;
	if(wall.w1<0){
		wall.w1=initialDist;
		srand(time(NULL));
		wall.w1dir.x=rand()%maxrand-maxrand/2;
		wall.w1dir.y=0;
		wall.w1dir.z=rand()%maxrand-maxrand/2;
		}else{
		wall.w1=wall.w1-step;}
	if(wall.w2<0){wall.w2=initialDist;
		srand(time(NULL));
		wall.w2dir.x=rand()%maxrand-maxrand/2;
		wall.w2dir.y=0;
		wall.w2dir.z=rand()%maxrand-maxrand/2;
	}else{wall.w2=wall.w2-step;}
	if(wall.w3<0){wall.w3=initialDist;
			srand(time(NULL));
		wall.w3dir.x=rand()%maxrand-maxrand/2;
		wall.w3dir.y=0;
		wall.w3dir.z=rand()%maxrand-maxrand/2;
		}else{wall.w3=wall.w3-step;}
	if(wall.w4<0){wall.w4=initialDist;
			srand(time(NULL));
		wall.w4dir.x=rand()%maxrand-maxrand/2;
		wall.w4dir.y=0;
		wall.w4dir.z=rand()%maxrand-maxrand/2;
		}else{wall.w4=wall.w4-step;}
	if(wall.w5<0){wall.w5=initialDist;
			srand(time(NULL));
		wall.w5dir.x=rand()%maxrand-maxrand/2;
		wall.w5dir.y=0;
		wall.w5dir.z=rand()%maxrand-maxrand/2;
	}else{wall.w5=wall.w5-step;}
	if(wall.w6<0){wall.w6=initialDist;
			srand(time(NULL));
		wall.w6dir.x=rand()%maxrand-maxrand/2;
		wall.w6dir.y=0;
		wall.w6dir.z=rand()%maxrand-maxrand/2;
	}else{wall.w6=wall.w6-step;}
		
	glutTimerFunc(1, drawWalls, 0);
    
}

void renderPremio(int cualPremio){
    glPushMatrix();
                glColor3d(p[cualPremio].color.x, p[cualPremio].color.y, p[cualPremio].color.z);
                glTranslatef(p[cualPremio].pos.x, 1.0, p[cualPremio].pos.z);
                glutSolidSphere(2.0, 20.0, 20.0);
    glPopMatrix();
   
    
}

void drawPremios(){ //maneja cuando se pinta el premio o no
    srand(time(NULL));
    int maxrand=200;
	for(int i=0; i<8; i++){
		if(p[i].tiempo==6){ //cuando el tiempo es 6, genera una posicion random del premio
			p[i].pos.x=rand()%maxrand-maxrand/2;
			p[i].pos.y=1.0;
			p[i].pos.z=rand()%maxrand-maxrand/2;
		}else if(p[i].tiempo<6 && p[i].tiempo>0){ //
			renderPremio(i);
			if(distancePointToPoint(&p[i].pos,&characterPosition)<3.5){
				p[i].tiempo=10;
			}
		}
	}
}

void premiosTimer(int value){ //maneja la variable de tiempo
	for(int i=0;i<8;i++){
		if(p[i].tiempo==0) p[i].tiempo=10; //reset cuando llega a 0
		else if(p[i].tiempo <= 10 && p[i].tiempo > 6){ //idle time
			p[i].tiempo--;	
		}
		else if(p[i].tiempo == 6){ //hay un 10% de probabilidades que pueda ser dibujado
			if((rand()%100)<= p[i].probabilidad*100) { 
				p[i].tiempo--;
			}else{
				p[i].tiempo=7;
			}
		}		//idle time
		else if(p[i].tiempo < 6) {
			p[i].tiempo--;
		}
		printf("%d ", p[i].tiempo);

	}
	printf("\n");
	glutTimerFunc(5000, premiosTimer, value);
}

void renderScene(void) { //maneja el entorno grafico
    
//	if (deltaMove)
//		computePos(deltaMove);
//   if (gammaMove)
//        computePosGamma(gammaMove);
    if(charMove){
			traslateVector(&characterPosition, &characterDirection, &newPosition, charMove);
			copyVectorValues(&newPosition, &characterPosition);
			//traslateVector(&wallPos, &wallDir, &wallNew, charMove);
			//copyVectorValues(&wallNew, &wallPos);
			//makeGeometricShape(&wallDir, 7, 10);
	}
	if(charRotate){
			getRotatedVector(&upDirection, &characterDirection, &newDirection, charRotate*PI/100);
			copyVectorValues(&newDirection, &characterDirection);
	}
	
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Reset transformations
	glLoadIdentity();
	// Set the camera
    
    //gluLookAt(	x, y, z,
    //          x+lx, y+ly,  z+lz,
    //         0.0f, 1.0f,  0.0f);
    
	if(camera%3==0)
        gluLookAt(	0, 200, 0,
                  0.0, 0.0,  0.0,
                  1.0f, 0.0f,  1.0f);
    else if(camera%3==1){
        gluLookAt(	0, 50, 50,
                0.0, 0.0,  0.0,
                  0.0f, 1.0f,  0.0f);
        gluLookAt(	characterPosition.x, 2.0, characterPosition.z,
                  characterDirection.x+characterPosition.x, 2.0, characterDirection.z+characterPosition.z,
                  0.0f, 1.0f,  0.0f);
    }
    else if(camera%3==2)
        gluLookAt(	characterPosition.x, 2.0, characterPosition.z,
                  characterDirection.x+characterPosition.x, 2.0, characterDirection.z+characterPosition.z,
                  0.0f, 1.0f,  0.0f);
    
    
    
	// Draw ground
	glPushMatrix();
		glBegin(GL_LINES);
    
            glColor3f(1.0f, 0.f, 0.f);
            glVertex3d(0., 0., 0.);
            glVertex3d(0., 0., 20.);
    
            glColor3f(0.0f, 1.f, 0.f);
            glVertex3d(0., 0., 0.);
            glVertex3d(0., 20., 0.);
    
            glColor3f(0.0f, 0.f, 1.f);
            glVertex3d(0., 0., 0.);
            glVertex3d(20., 0., 0.);
    
            glColor3f(0.0f, 0.f, 1.f);
            glVertex3d(0., 0., 0.);
            glVertex3d(characterDirection.x*20, characterDirection.y*20, characterDirection.z*20);
		glEnd();
		glColor3f(0.0f, 1.f,0.f);
	glPopMatrix();
    
    //dibuja las paredes
    			glColor3f(1.0f, 0.0f, 0.0f);
	makeGeometricShape(&wall.w1dir, wall.w1, 4);
    			glColor3f(0.0f, 1.0f, 0.0f);
	makeGeometricShape(&wall.w2dir, wall.w2, 4);
    			glColor3f(0.0f, 0.0f, 1.0f);
	makeGeometricShape(&wall.w3dir, wall.w3, 7);
    			glColor3f(1.0f, 0.0f, 0.0f);
	makeGeometricShape(&wall.w4dir, wall.w4, 4);
    			glColor3f(1.0f, 1.0f, 0.0f);
	makeGeometricShape(&wall.w5dir, wall.w5, 6);
    			glColor3f(1.0f, 0.0f, 1.0f);
	makeGeometricShape(&wall.w6dir, wall.w6, 9);
                glColor3f(1.0f, 1.0f, 0.0f);
	
	//glPushMatrix();
    //glTranslatef(x+lx*0.5, y+ly*0.5, z+lz*0.5-2);
    //glRotatef(deltaAngle, lx, ly, lz);
    renderCharacter();
    //glPopMatrix();
    
    //dibuja los premios
    drawPremios();
    
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
    
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    GLfloat lightColor0[] = {1.9f, 1.9f, 1.9f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {50, 10, 50, 0.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    //Add directed light
    GLfloat lightColor1[] = {1.9f, 1.9f, 1.9f, 1.0f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-50, 10.0f, -50, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    
	
    glutSwapBuffers();
}

void initValues(){
    
    //la distancia en la que empiezan a aparecer las paredes es 600
    initialDist=600;
    //camera inicial es la vista de hacia arriba
    camera = 0;
	//direccion inicial del personaje
    origin.x=0;
    origin.y=0;
    origin.z=0;
    
    //direccion inicial del personaje
    characterDirection.x = 1.0f;
	characterDirection.y = 0.0f;
	characterDirection.z = 1.0f;
    
	//se inicializa el frente por defecto del objeto
	initialFront.x = 0.0f;
	initialFront.y = 0.0f;
	initialFront.z = -1.0f;
    
	//se inicializa el frente por defecto del objeto
	upDirection.x = 0.0f;
	upDirection.y = 1.0f;
	upDirection.z = 0.0f;
	
    //posicion inicial del personaje
	characterPosition.x = 0.000000000001f;
	characterPosition.y = 0.0f;
	characterPosition.z = 0.000000000001f;
	
	//posicion inicial de la pared
	wallPos.x=-30.0f;
	wallPos.y=0.0f;
	wallPos.z=19.0f;
	
    //direccion inicial de la pared
	wallDir.x=-wallPos.x;
	wallDir.y=-wallPos.y;
	wallDir.z=-wallPos.z;
    //posiciones iniciales de cada una de las paredes
    wall.w1=20;
	wall.w2=500;
	wall.w3=400;
	wall.w4=300;
	wall.w5=200;
	wall.w6=100;
    //direccion inicial de la pared w1 (usado para debug)
    wall.w1dir.x=-1;
    wall.w1dir.y=0;
    wall.w1dir.z=1;
    
    //inicializacion de los premios
    p[vida].probabilidad = 0.1;
    p[vida].color.x=1.0f;
    p[vida].color.y=0.0f;
    p[vida].color.z=0.0f;
    p[vida].tiempo = 6;
    
    p[invencibilidad].probabilidad = 0.1;
    p[invencibilidad].color.x=1.0f;
    p[invencibilidad].color.y=1.0f;
    p[invencibilidad].color.z=1.0f;
    p[invencibilidad].tiempo = 6;
    
    p[velocidad2].probabilidad = 0.15;
    p[velocidad2].color.x=0.0f;
    p[velocidad2].color.y=1.0f;
    p[velocidad2].color.z=0.0f;
    p[velocidad2].tiempo = 6;
    
    p[velocidadPared].probabilidad = 0.15;
    p[velocidadPared].color.x=0.0f;
    p[velocidadPared].color.y=0.0f;
    p[velocidadPared].color.z=1.0f;
    p[velocidadPared].tiempo = 6;
    
    //1 0.752941 0.796078
    p[puntos1000].probabilidad = 0.5;
    p[puntos1000].color.x=1.0f;
    p[puntos1000].color.y=0.752941f;
    p[puntos1000].color.z=0.796078f;
    p[puntos1000].tiempo = 6;
    
    //0.603922 0.803922 0.196078
    p[cambiaSentido].probabilidad = 0.2;
    p[cambiaSentido].color.x=0.603922f;
    p[cambiaSentido].color.y=0.803922f;
    p[cambiaSentido].color.z=0.196078f;
    p[cambiaSentido].tiempo = 6;
    
    //0.627451 0.12549 0.941176
    p[iman].probabilidad = 0.1;
    p[iman].color.x=0.627451f;
    p[iman].color.y=0.12549f;
    p[iman].color.z=0.941176f;
    p[iman].tiempo = 6;
}

int main(int argc, char **argv) {
    initValues();
	// init GLUT and create window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800,640);
	glutCreateWindow("HexxTreMaze");
    
	// register callbacks
	glutDisplayFunc(renderScene);
    //glutDisplayFunc(renderCharacter);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
    
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	//glutTimerFunc(1, drawWalls, 0);
    glutTimerFunc(5000, premiosTimer, 0);
	// here are the two new functions
	//glutMouseFunc(mouseButton);
	//glutPassiveMotionFunc(mouseMove);
    
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
	// enter GLUT event processing cycle
	glutMainLoop();

    
	return 1;
}











