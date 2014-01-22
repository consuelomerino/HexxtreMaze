#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "vector.h"
#define PI 3.14159265359

vector3d characterDirection;
vector3d characterPosition;
vector3d initialFront;
vector3d upDirection;
vector3d newDirection;
vector3d newPosition;
vector3d wallDir;
vector3d wallPos;
vector3d wallNew;

// angle of rotation for the camera direction
float dangle = 0.0f;
float gangle = 0.0f;

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

/*void computePos(float deltaMove) {
	x += deltaMove * lx * 0.1f;
  	//y += deltaMove * ly * 0.1f;
    z += deltaMove * lz * 0.1f;

}*/

/*void computePosGamma(float gammaMove) { //se mueve en perpendicular
	x += gammaMove * lz * 0.1f;
  	//y += gammaMove * ly * 0.1f;
    z += gammaMove * - lx * 0.1f;
}
*/
void renderCharacter(void){ //personaje
	float anguloARotar = getAnguloEntreVectores(&initialFront, &characterDirection);
	float anguloARotar2 = characterDirection.x < 0?radiansToDegrees(anguloARotar):-radiansToDegrees(anguloARotar);
		
    glPushMatrix();
    glColor3f(1.0f, 1.0f , 0.0f);
    glTranslatef(characterPosition.x,characterPosition.y,characterPosition.z);
	glRotatef(anguloARotar2, 0.0f, 1.0f,0.0f);
	glutWireTeapot(12.0f);
    glPopMatrix();
}

void renderWall(vector3d *wallDir, float length, float distance){
	float volume=2.0f;
	float floor=volume/2.0f;
    float anguloARotar = getAnguloEntreVectores(&initialFront, wallDir);
	float anguloARotar2;
	glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.5f);
	traslateVector(wallDir, wallDir, &wallPos, distance);
	glTranslatef(wallPos.x,floor,wallPos.z);
	glPushMatrix();
		glBegin(GL_LINES);
			//glVertex3d(-wallPos.x*20, -wallPos.y*20, -wallPos.z*20);
			//glVertex3d(wallPos.x*20, wallPos.y*20, wallPos.z*20);	
			glColor3f(0.2f, 0.1f, 0.5f);
			glVertex3d(wallDir->x*20, wallDir->y*20, wallDir->z*20);
			glVertex3d(-wallDir->x*20, -wallDir->y*20, -wallDir->z*20);	
		glEnd();
	glPopMatrix();
/*	for(int i = -2; i < 3; i++){
        glPushMatrix();
            vector3d wallDirNew;
			glColor3f(0.2f, 0.1f, 0.7f);
			//glRotatef(0, 0.0f, 1.0f, 0.0f);
			traslateVectorNormal(&wallPos, wallDir, &wallDirNew, volume*i);
			glTranslatef(wallDirNew.x,0,wallDirNew.z);
            anguloARotar2 = wallPos.x < 0? radiansToDegrees(anguloARotar) : -radiansToDegrees(anguloARotar);
            glRotatef(anguloARotar2, 0.0f, 1.0f, 0.0f);
            glutSolidCube(volume);
			
        glPopMatrix();
    }*/
	glPushMatrix();
		glColor3f(0.2f, 0.1f, 0.7f);
		//glRotatef(0, 0.0f, 1.0f, 0.0f);
		//traslateVector(&wallPos, wallDir, &wallDirNew, 1);
		glTranslatef(wallDir->x,0,wallDir->z);
		anguloARotar2 = wallPos.x > 0? radiansToDegrees(anguloARotar) : -radiansToDegrees(anguloARotar);
		glRotatef(anguloARotar2, 0.0f, 1.0f, 0.0f);
		vector3d wallVertice1, wallVertice2;
		traslateVector(wallDir, wallDir, &wallVertice1, distance);
		traslateVector(wallDir, wallDir, &wallVertice2, distance+2);
		glBegin(GL_QUADS); //6 caras en total
			vector3d a, b, c, d, e, f, g, h;
			/*a.x=0; a.y=1; a.z=1;
			b.x=1; b.y=1; b.z=1;
			c.x=1; c.y=0; c.z=1;
			d.x=0; d.y=0; d.z=1;
			e.x=0; e.y=1; e.z=0;
			f.x=1; f.y=1; f.z=0;
			g.x=1; g.y=0; g.z=0;
			h.x=0; h.y=0; h.z=0;
			a.x=wallVertice2.x; a.y=1; a.z=wallVertice2.z;
			b.x=1; b.y=1; b.z=1;
			c.x=1; c.y=0; c.z=1;
			d.x=wallVertice2.x; d.y=0; d.z=wallVertice2.z;
			e.x=wallVertice1.x; e.y=1; e.z=wallVertice1.z;
			f.x=1; f.y=1; f.z=0;
			g.x=1; g.y=0; g.z=0;
			h.x=wallVertice1.x; h.y=0; h.z=wallVertice1.z;*/
			b.x=wallVertice2.x; b.y=1; b.z=wallVertice2.z;
			a.x=0; a.y=1; a.z=1;
			d.x=0; d.y=0; d.z=1;
			c.x=wallVertice2.x; c.y=0; c.z=wallVertice2.z;
			f.x=wallVertice1.x; f.y=1; f.z=wallVertice1.z;
			e.x=0; e.y=1; e.z=0;
			h.x=0; h.y=0; h.z=0;
			g.x=wallVertice1.x; g.y=0; g.z=wallVertice1.z;
			glColor3f(1.0f, 0.0f, 0.0f);
			//glVertex3f(0.0f,1.0f, 2.0f); glVertex3f(1.0f,1.0f, 2.0f);
			//glVertex3f(0.0f,0.0f, 2.0f); glVertex3f(1.0f,0.0f, 2.0f); 
			//Pared Externa
			//glVertex3f(0,1,1); glVertex3f(1,1,1); //0,1 1,1
			//glVertex3f(1,0,1); glVertex3f(0,0,1); //1,0 0,0
			glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z); //0,1 1,1
			glVertex3f(c.x,c.y,c.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0
			//Pared Interna
			//glVertex3f(0,1,0); glVertex3f(1,1,0); //0,1 1,1
			//glVertex3f(1,0,0); glVertex3f(0,0,0); //1,0 0,0
			glVertex3f(e.x,e.y,e.z); glVertex3f(f.x,f.y,f.z); //0,1 1,1
			glVertex3f(g.x,g.y,g.z); glVertex3f(h.x,h.y,h.z); //1,0 0,0
			//Pared costado
			glColor3f(0.0f,1.0f, 0.0f);
			//glVertex3f(1.0f,0.0f, 1.0f); glVertex3f(1.0f,1.0f, 1.0f);
			//glVertex3f(1.0f,1.0f, 0.0f); glVertex3f(1.0f,0.0f, 0.0f);
			glVertex3f(b.x,b.y,b.z); glVertex3f(f.x,f.y,f.z); //0,1 1,1
			glVertex3f(g.x,g.y,g.z); glVertex3f(c.x,c.y,c.z);//1,0 0,0
			
			//Pared costado
			//glVertex3f(0.0f,0.0f, 1.0f); glVertex3f(0.0f,1.0f, 1.0f);
			//glVertex3f(0.0f,1.0f, 0.0f); glVertex3f(0.0f,0.0f, 0.0f);
			glVertex3f(d.x,d.y,d.z); glVertex3f(a.x,a.y,a.z); //0,1 1,1
			glVertex3f(e.x,e.y,e.z); glVertex3f(h.x,h.y,h.z); //1,0 0,0
			//Base de abajo
			glColor3f(0.0f,0.0f, 1.0f);
			//glVertex3f(0.0f,0.0f, 1.0f); glVertex3f(1.0f,0.0f, 1.0f);
			//glVertex3f(1.0f,0.0f, 0.0f); glVertex3f(0.0f,0.0f, 0.0f);	
			glVertex3f(c.x,c.y,c.z); glVertex3f(g.x,g.y,g.z); //0,1 1,1
			glVertex3f(h.x,h.y,h.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0			
			//Base de arriba
			glVertex3f(a.x,a.y,a.z); glVertex3f(e.x,e.y,e.z); //0,1 1,1
			glVertex3f(f.x,f.y,f.z); glVertex3f(b.x,b.y,b.z); //1,0 0,0		

		glEnd(); 
		//glutSolidCube(volume);
	glPopMatrix();
    
	
	//glRotatef(0, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void makeGeometricShape(vector3d* direc, float distancia, int cantidadDeLados){
	vector3d direcUnitary;
	vector3d newDirec;
	float length;
	float geometricAngle=360/cantidadDeLados;
	glPushMatrix();
	getUnitaryVector(direc, &direcUnitary);
	for(int i=0;i<cantidadDeLados-1;i++){
		getRotatedVector(&upDirection, &direcUnitary, &newDirec, degreesToRadians(geometricAngle));
		length=2;
		renderWall(&newDirec, length, distancia);
		copyVectorValues(&newDirec, &direcUnitary);
	}
	glPopMatrix();
}

void renderScene(void) {
    
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
    
	gluLookAt(	30, 30, 30,
                0.0, 0.0,  0.0,
                0.0f, 1.0f,  0.0f);
    
	// Draw ground
	glPushMatrix();
		glBegin(GL_LINES);
		  glVertex3d(0., 0., 0.);
		  glVertex3d(0., 0., -20.);
		  /*glVertex3d(0., 0., 0.);
		  glVertex3d(0., 12., 0.);
		  glVertex3d(0., 0., 0.);
		  glVertex3d(0., 0., 12.);*/
		  glColor3f(0.0f, 0.f, 1.f);
		  glVertex3d(0., 0., 0.);
		  glVertex3d(characterDirection.x*20, characterDirection.y*20, characterDirection.z*20);	
		glEnd();
		glColor3f(0.0f, 1.f,0.f);
	glPopMatrix();
    
	makeGeometricShape(&wallDir, characterPosition.x, 4);
	
	//glPushMatrix();
    //glTranslatef(x+lx*0.5, y+ly*0.5, z+lz*0.5-2);
    //glRotatef(deltaAngle, lx, ly, lz);
    renderCharacter();
    //glPopMatrix();
    //glPushMatrix();
	//glPopMatrix();
    
    
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
    
    // Draw 36 SnowMen
    
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
            glPushMatrix();
				glTranslatef(i*10.0,0,j * 10.0);
				drawSnowMan();
            glPopMatrix();
        }
    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    
    if (key == 27)
        exit(0);
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
			charRotate=-1.0f;
			break;
        case GLUT_KEY_RIGHT : 
			charRotate=1.0f;
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

int main(int argc, char **argv) {

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
	
	characterPosition.x = 0.000000000001f;
	characterPosition.y = 0.0f;
	characterPosition.z = 0.000000000001f;    
	
	
	wallPos.x=-30.0f;
	wallPos.y=0.0f;
	wallPos.z=19.0f;
	
	wallDir.x=-wallPos.x;
	wallDir.y=-wallPos.y;
	wallDir.z=-wallPos.z;
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
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
    
	// here are the two new functions
	//glutMouseFunc(mouseButton);
	//glutPassiveMotionFunc(mouseMove);
    
	// OpenGL init
	glEnable(GL_DEPTH_TEST);    
	// enter GLUT event processing cycle
	glutMainLoop();

    
	return 1;
}











