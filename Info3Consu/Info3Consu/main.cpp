#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//ver
#include "texture.h"
#define  TexMont 2 
#define PRINCIPAL 0
#define JUEGO  1
#define TEXTPRINCIPAL 1

#define FILL 1
#define LINE 2

#define SHRINK 1
#define NORMAL 2
//
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

//OpcionPantalla
int OpcionSelecionada = PRINCIPAL;

//To menu
int fillMenu, shrinkMenu, mainMenu, colorMenu;
//Menu status
int menuFlag = 0;
float scale = 1.0f;
// color for the nose
float red = 1.0f, blue=0.5f, green=0.5f;

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
// -----------------------------------
//             MENUS
// -----------------------------------

void processMenuStatus(int status, int x, int y) {

	if (status == GLUT_MENU_IN_USE)
		menuFlag = 1;
	else
		menuFlag = 0;
}


void processMainMenu(int option) {

	// nothing to do in here
	// all actions are for submenus
}

void processFillMenu(int option) {

	switch (option) {

		case FILL: glPolygonMode(GL_FRONT, GL_FILL); break;
		case LINE: glPolygonMode(GL_FRONT, GL_LINE); break;
	}
}

void processShrinkMenu(int option) {

	switch (option) {

		case SHRINK: scale = 0.5f; break;
		case NORMAL: scale = 1.0f; break;
	}
}
void createPopupMenus() {

	shrinkMenu = glutCreateMenu(processShrinkMenu);

	glutAddMenuEntry("Shrink",SHRINK);
	glutAddMenuEntry("NORMAL",NORMAL);

	fillMenu = glutCreateMenu(processFillMenu);

	glutAddMenuEntry("Fill",FILL);
	glutAddMenuEntry("Line",LINE);

	mainMenu = glutCreateMenu(processMainMenu);

	glutAddSubMenu("Polygon Mode", fillMenu);
	glutAddSubMenu("Color", colorMenu);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// this will allow us to know if the menu is active
	glutMenuStatusFunc(processMenuStatus);
}
///-------------------------
//		Presentacion
//--------------------------
void drawString (void * font, char *s, float x, float y, float z){
     unsigned int i;
     glRasterPos3f(x, y, z);

     for (i = 0; i < strlen (s); i++)
          glutBitmapCharacter (font, s[i]);
}


void menuPrincipal(){
	glPushMatrix();
               //eje X
               glColor3f(1,0,1);
               drawString(GLUT_BITMAP_HELVETICA_18, (char*) "MI SUPER JUEGO 3D", 0, 1, 0); 
            
            glPopMatrix();
			
	glutSwapBuffers();
	/*
	textura arena;

// carga
	if(!cargarTGA("images/dry_earth.tga", &arena))
	{
		printf("Error cargando textura\n");
		exit(0); // Cargamos la textura y chequeamos por errores
	}

	// uso
	glEnable(GL_TEXTURE_2D);
		// Arena
		glColor3ub(255,218,53);
		glBindTexture(GL_TEXTURE_2D,arena.ID);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0,0.0);
			glVertex3f(-50, -50,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(-50, 50,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(50, 50,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(50, -50,0);
		glEnd();
		*/
		//glutSwapBuffers();

	// con glTexCoord seleccionas que parte de la textura vas a usar
	// y con glTVertex pegas la textura sobre el objeto que se esta dibujando

}
//-----------------------------------------
//			DIBUJA OBJETO
//----------------------------------------
void renderCharacter(void){ //personaje
	float anguloARotar = getAnguloEntreVectores(&initialFront, &characterDirection);
	float anguloARotar2 = characterDirection.x < 0?radiansToDegrees(anguloARotar):-radiansToDegrees(anguloARotar);
		
    glPushMatrix();
    glColor3f(1.0f, 1.0f , 0.0f);
    glTranslatef(characterPosition.x,characterPosition.y,characterPosition.z);
	glRotatef(anguloARotar2, 0.0f, 1.0f,0.0f);
	glutWireTeapot(6.0f);
    glPopMatrix();
}

void renderWall(void){
	float volume=2.0f;
	float floor=volume/2.0f;
    float anguloARotar = getAnguloEntreVectores(&initialFront, &wallPos);
	float anguloARotar2;
	glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.5f);
    glTranslatef(wallPos.x,floor,wallPos.z);
	for(int i = -3; i < 3; i++){
        glPushMatrix();
            vector3d wallDirNew;
			//glRotatef(0, 0.0f, 1.0f, 0.0f);
			traslateVectorNormal(&wallPos, &wallDir, &wallDirNew, volume*i);
			glTranslatef(wallDirNew.x,0,wallDirNew.z);
            anguloARotar2 = wallPos.x < 0? radiansToDegrees(anguloARotar) : -radiansToDegrees(anguloARotar);
            glRotatef(anguloARotar2, 0.0f, 1.0f, 0.0f);
            glutSolidCube(volume);
        glPopMatrix();
    }
	//glRotatef(0, 0.0f, 0.0f, 0.0f);
	glPopMatrix();
}
void renderScene(void) {
    
//	if (deltaMove)
//		computePos(deltaMove);
//   if (gammaMove)
//        computePosGamma(gammaMove);

	 switch (OpcionSelecionada) {
        case PRINCIPAL:
            menuPrincipal();
            break;
        case JUEGO:             
           
			if(charMove){
					traslateVector(&characterPosition, &characterDirection, &newPosition, charMove);
					copyVectorValues(&newPosition, &characterPosition);
					traslateVector(&wallPos, &wallDir, &wallNew, charMove);
					copyVectorValues(&wallNew, &wallPos);
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
			
			gluLookAt(	50, 50, 50,
                0.0, 0.0,  0.0,
                0.0f, 1.0f,  0.0f);
			
			// Draw ground
			glPushMatrix();
				glBegin(GL_LINES);
				  glVertex3d(0., 0., 0.);
				  glVertex3d(0., 0., -50.);
				  /*glVertex3d(0., 0., 0.);
				  glVertex3d(0., 12., 0.);
				  glVertex3d(0., 0., 0.);
				  glVertex3d(0., 0., 12.);*/
				  glColor3f(0.0f, 0.f, 100.0f);
				  glVertex3d(0., 0., 0.);
				  glVertex3d(characterDirection.x*20, characterDirection.y*20, characterDirection.z*20);	
				glEnd();
				glColor3f(0.0f, 100.0f,0.f);
			glPopMatrix();
			
			
			
			//glPushMatrix();
			//glTranslatef(x+lx*0.5, y+ly*0.5, z+lz*0.5-2);
			//glRotatef(deltaAngle, lx, ly, lz);
			renderCharacter();
			//glPopMatrix();
			//glPushMatrix();
			renderWall();
			//glPopMatrix();
			glPushMatrix();
				glBegin(GL_QUADS);
					glColor3f(0.0f, 0.0f, 100.0f);
					glVertex3f(-20.0f, 0.0f, -20.0f);
					glVertex3f(-20.0f, 0.0f,  20.0f);
					glVertex3f( 20.0f, 0.0f,  20.0f);
					glVertex3f( 20.0f, 0.0f, -20.0f);
				glEnd();
				glColor3f(0.0f, 0.0f, 100.0f);
			glPopMatrix();
			
			glPushMatrix();
				glBegin(GL_QUADS);
					glColor3f(0.0f, 25.0f, 0.0f);
					glVertex3f(-50.0f, 0.0f, -50.0f);
					glVertex3f(-50.0f, 0.0f,  50.0f);
					glVertex3f( 50.0f, 0.0f,  50.0f);
					glVertex3f( 50.0f, 0.0f, -50.0f);
				glEnd();
				glColor3f(0.0f, 25.0f, 0.0f);
			glPopMatrix();
				
		
			// Draw 36 SnowMen
			
			for(int i = -3; i < 3; i++)
				for(int j=-3; j < 3; j++) {
					glPushMatrix();
						glTranslatef(i*10.0,0,j * 10.0);
						drawSnowMan();
					glPopMatrix();
				}
			glutSwapBuffers();
			
			
			 //DibujarFondo(TexMont);
			  break;
		}
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    glutSetMenu(mainMenu);
	switch (key) {
		case 27:
			glutDestroyMenu(mainMenu);
			glutDestroyMenu(fillMenu);
			glutDestroyMenu(colorMenu);
			glutDestroyMenu(shrinkMenu);
			exit(0);
			break;
		case 13:
			OpcionSelecionada = JUEGO;
			break;
		case 's':
			if (!menuFlag)
			  glutChangeToSubMenu(2,"Shrink",shrinkMenu);
			break;
		case 'c':
			if (!menuFlag)
				glutChangeToSubMenu(2,"Color",colorMenu);
			break;
	}
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
	
	characterPosition.x = 0.1f;
	characterPosition.y = 0.0f;
	characterPosition.z = 0.1f;    
	
	
	wallPos.x=10.0f;
	wallPos.y=0.0f;
	wallPos.z=19.0f;
	
	wallDir.x=-wallPos.x;
	wallDir.y=-wallPos.y;
	wallDir.z=-wallPos.z;
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800,600);
	glutCreateWindow("HexxTreMaze");
    
	// register callbacks
	glutDisplayFunc(renderScene); //mostrar lo que esta en la funicon
    glutDisplayFunc(renderCharacter);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene); //cuando esta en espera llama funcion
    
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
    
	// here are the two new functions
	//glutMouseFunc(mouseButton);
	//glutPassiveMotionFunc(mouseMove);
    
	// OpenGL init
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	//Menu
	createPopupMenus();
	// enter GLUT event processing cycle
	glutMainLoop();

	
    
	return 1;
}











