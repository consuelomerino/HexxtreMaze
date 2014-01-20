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
vector3d initialFront;
vector3d upDirection;
vector3d newDirection;

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
float deltaMove = 0;
float gammaMove = 0;
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
	glutSolidCube(0.75f);
    
    // Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidCube(0.75f);
    
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

void computePos(float deltaMove) {
	x += deltaMove * lx * 0.1f;
  	//y += deltaMove * ly * 0.1f;
    z += deltaMove * lz * 0.1f;

}

void computePosGamma(float gammaMove) { //se mueve en perpendicular
	x += gammaMove * lz * 0.1f;
  	//y += gammaMove * ly * 0.1f;
    z += gammaMove * - lx * 0.1f;
}

void renderCharacter(void){ //personaje
	float anguloARotar = getAnguloEntreVectores(&initialFront, &characterDirection);
	float anguloARotar2 = characterDirection.x < 0?radiansToDegrees(anguloARotar):-radiansToDegrees(anguloARotar);
		
    glPushMatrix();
    glColor3f(1.0f, 1.0f , 0.0f);
	glRotatef(anguloARotar2, 0.0f, 1.0f,0.0f);
	glutWireTeapot(12.0f);
    glPopMatrix();
}

void renderScene(void) {
    
	if (deltaMove)
		computePos(deltaMove);
//    if (gammaMove)
//        computePosGamma(gammaMove);
    
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
    
	
	
	glPushMatrix();
    //glTranslatef(x+lx*0.5, y+ly*0.5, z+lz*0.5-2);
    glRotatef(deltaAngle, lx, ly, lz);
    glTranslatef(characterDirection.x,characterDirection.y,characterDirection.z);
    renderCharacter();
    glPopMatrix();
    
    
    
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
			//deltaMove = 0.5f; 
			traslateVector(&characterDirection, &newDirection, 0.1);
			copyVectorValues(&newDirection, &characterDirection);			
			break;
        case GLUT_KEY_DOWN : 
			traslateVector(&characterDirection, &newDirection, -0.1);
			copyVectorValues(&newDirection, &characterDirection);			
			break;
        case GLUT_KEY_LEFT : 
			getRotatedVector(&upDirection, &characterDirection, &newDirection, -PI/100);
			copyVectorValues(&newDirection, &characterDirection);
							break;
        case GLUT_KEY_RIGHT : 
			getRotatedVector(&upDirection, &characterDirection, &newDirection, PI/100);
			copyVectorValues(&newDirection, &characterDirection);
			//gammaMove = -0.5f; 
			break;
    }
}

void releaseKey(int key, int x, int y) {
    
    switch (key) {
        case GLUT_KEY_UP :
        case GLUT_KEY_DOWN : deltaMove = 0;  break;
        case GLUT_KEY_LEFT :
        case GLUT_KEY_RIGHT : gammaMove = 0; break;
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











