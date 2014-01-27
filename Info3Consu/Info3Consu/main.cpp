#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//ver
#include "texture.h"
//#include "texturaPrueba.h"
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




float infoCollision = false;

// angle of rotation for the camera direction
//
int vidas; //vidas actuales del jugador
int tiempo; //tiempo de juego en multiplos de 10ms
int esInvencible; //0 si no es invencible, 1 si es invencible
int initVelocidadPersonaje; //velocidad inicial del personaje
int velocidadPersonaje; //velocidad actual del personaje
int banderaVelocidad2; //se usa para el timer de cuanto tiempo tiene que estar con la velocidad modificada
int velocidadParedes; //velocidad actual de las paredes
int puntos; //contador de puntos del jugador
int banderaPerderVidas; //bandera que se usa para el timer, cuanto tiempo tiene de invencibilidad antes de perder otra vida
vector3d characterDirection; //la direccion actual del personaje
vector3d characterPosition; //la posicion actual de la posicion del personaje
vector3d initialFront; //el frente inicial de los objetos
vector3d origin; 	//origen, 0,0,0
vector3d upDirection;	//direccion up (0,1,0)
vector3d newDirection; //direccion nueva calculada para la nueva direccion del personaje
vector3d newPosition;  //posicion nueva calculada para la nueva posicion del personaje
vector3d wallDir;		//la direccion de la pared
vector3d wallPos;		//la posicion de la pared
vector3d wallNew;		//la nueva posicion de la pared
wallDistance wall;		//struct con todos los datos de las paredes
premio p[7];			//struct con todos los datos de los premios
int camera; //guarda cual tipo de camara se va a usar (gluLookAt)
float initialDist;		//distancia inicial de las paredes del origen

//las variables que determinan si el personaje se mueve o rota con las flechas del teclado
float charMove = 0;
float charRotate = 0;



// angle of rotation for the camera direction [DEPRECATED]
////>>>>>>> origin/CM
float dangle = 0.0f;
float gangle = 0.0f;

// actual vector representing the camera's direction [DEPRECATED]
float lx=0.0f,ly=0.0f,lz=-1.0f;

// XZ position of the camera [DEPRECATED]
float x=0.0f, y=1.0f, z=5.0f;

// the key states. These variables will be zero
//when no key is being presses [DEPRECATED]
float deltaAngle = 0.0f;
float gammaAngle = 0.0f;
int xOrigin = -1;
int yOrigin = -1;

////<<<<<<< HEAD
//OpcionPantalla
int OpcionSelecionada = PRINCIPAL;

//To menu
int fillMenu, shrinkMenu, mainMenu, colorMenu;
//Menu status
int menuFlag = 0;
float scale = 1.0f;
// color for the nose
float red = 1.0f, blue=0.5f, green=0.5f;


////>>>>>>> origin/CM

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
//<<<<<<< HEAD

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

//<<<<<<< HEAD
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
void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, 800, 600, 0);
	
	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void renderBitmapString(float x, float y, void *font,char *string)
{
  
  char *c;
  glRasterPos2f(x, y);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}
/*
void DibujarPresentacion(int ind){
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[ind]);
   
    glBegin(GL_QUADS); 
    

        glTexCoord2d(-1.0,0.0);
        glVertex3f( -1.0f,1.0f,0.0f);		
        glTexCoord2d(0.0,0.0);
        glVertex3f(1.0f,1.0f,0.0f);		
        glTexCoord2d(0.0,-1.0);
        glVertex3f(1.0f, -1.0f,0.0f);	
        glTexCoord2d(-1.0,-1.0);
        glVertex3f( -1.0f, -1.0f,0.0f);


    glEnd();

    glDisable(GL_TEXTURE_2D);
	glFlush();

  //  if(ind == VecRac)
   // {
        //IngresarTexto();
   // }
	glutSwapBuffers();
     
}*/
void menuPrincipal(){
	// Declaracion
	setOrthographicProjection();

	// Declaracion
	textura arena;

	// carga
	if(!cargarTGA("images/C.tga", &arena))
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
    glTexCoord2f(50.0,50.0);
    glVertex3f(-50, -50,0);
    glTexCoord2f(50.0,50.0);
    glVertex3f(-50, 50,0);
    glTexCoord2f(50.0,50.0);
    glVertex3f(50, 50,0);
    glTexCoord2f(50.0,50.0);
    glVertex3f(50, -50,0);
    glEnd();

	//DibujarPresentacion(PORTADA);
	resetPerspectiveProjection();

}
//-----------------------------------------
//			DIBUJA OBJETO
//----------------------------------------


////>>>>>>> origin/CM
void renderCharacter(void){ //personaje

//void renderCharacter(void){ //dibuja al personaje
//>>>>>>> origin/CM
	float anguloARotar = getAnguloEntreVectores(&initialFront, &characterDirection);
	float anguloARotar2 = characterDirection.x < 0?radiansToDegrees(anguloARotar):-radiansToDegrees(anguloARotar);
		
    glPushMatrix();
////<<<<<<< HEAD
    glColor3f(1.0f, 1.0f , 0.0f);
    glTranslatef(characterPosition.x,0.1,characterPosition.z);
	glRotatef(anguloARotar2, 0.0f, 1.0f,0.0f);
//<<<<<<< HEAD
	//glutWireTeapot(6.0f);

	glutWireTeapot(2.0f);
//>>>>>>> origin/CM

		glColor3f(1.0f, 1.0f , 0.0f);
		glTranslatef(characterPosition.x,0.1,characterPosition.z);
		glRotatef(anguloARotar2, 0.0f, 1.0f,0.0f);
		glutWireTeapot(2.0f);
//>>>>>>> origin/CM
    glPopMatrix();
}

void loseLife(){ //maneja el perder las vidas, espera 3 segundos antes de perder una vida nuevamente
	printf("%d\n", banderaPerderVidas);
	if(banderaPerderVidas==0){
		vidas--;
		banderaPerderVidas=3000;
	}
}

void renderWall(vector3d *wallDir, float angle, float distance){ //dibuja un segmento de pared
	float volume=2.0f;
	float floor=volume/2.0f;
    float anguloARotar = getAnguloEntreVectores(&initialFront, wallDir);
	float anguloARotar2;
//<<<<<<< HEAD
//<<<<<<< HEAD
	glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.5f);
    glTranslatef(wallPos.x,floor,wallPos.z);
	for(int i = -3; i < 3; i++){
        glPushMatrix();
            vector3d wallDirNew;
			//glRotatef(0, 0.0f, 1.0f, 0.0f);
			traslateVectorNormal(&wallPos, wallDir, &wallDirNew, volume*i);
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
					//glColor3f(100.0f, 0.0f, 0.0f);
					glColor3f(25.0f, 25.0f, 25.0f);
					glVertex3f(-25.0f, 0.0f, -25.0f);
					glVertex3f(-25.0f, 0.0f,  25.0f);
					glVertex3f( 25.0f, 0.0f,  25.0f);
					glVertex3f( 25.0f, 0.0f, -25.0f);
				glEnd();
			glPopMatrix();
			
			glPushMatrix();
				glBegin(GL_QUADS);
					//glColor3f(0.0f, 25.0f, 0.0f);
					glColor3f(25.0f, 25.0f, 25.0f);
					glVertex3f(-50.0f, 0.0f, -50.0f);
					glVertex3f(-50.0f, 0.0f,  50.0f);
					glVertex3f( 50.0f, 0.0f,  50.0f);
					glVertex3f( 50.0f, 0.0f, -50.0f);
				glEnd();
			glPopMatrix();
				
		
			// Draw 36 SnowMen
			/*
			for(int i = -3; i < 3; i++)
				for(int j=-3; j < 3; j++) {
					glPushMatrix();
						glTranslatef(i*10.0,0,j * 10.0);
						drawSnowMan();
					glPopMatrix();
				}*/
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

	//glColor3f(wallDir->x, wallDir->y, wallDir->z);

//>>>>>>> origin/CM
	traslateVector(wallDir, wallDir, &wallPos, distance);
	vector3d wall45grados1, wallVertice45gradosInt1, wallVertice45gradosExt1;
	vector3d wall45grados2, wallVertice45gradosInt2, wallVertice45gradosExt2;
	getRotatedVector(&upDirection, wallDir, &wall45grados1, degreesToRadians(angle/2));
	getRotatedVector(&upDirection, wallDir, &wall45grados2, -degreesToRadians(angle/2));
	//para hacer debug
	/*glBegin(GL_LINES);
		//glVertex3d(-wallPos.x*20, -wallPos.y*20, -wallPos.z*20);
		//glVertex3d(wallPos.x*20, wallPos.y*20, wallPos.z*20);	
		//glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3d(wall45grados1.x*20, wall45grados1.y*20, wall45grados1.z*20);
		glVertex3d(0, 0, 0);
		//glColor3f(0.0f, 0.7f, 0.5f);
		glVertex3d(wall45grados2.x*20, wall45grados2.y*20, wall45grados2.z*20);
		glVertex3d(0, 0, 0);
	glEnd();
	*/
	glBegin(GL_LINES);
		glVertex3d(wallDir->x*20, wallDir->y*20, wallDir->z*20);
		glVertex3d(-wallDir->x*20, -wallDir->y*20, -wallDir->z*20);	
	glEnd();

	traslateVector(&wall45grados1, &wall45grados1, &wallVertice45gradosInt1, getMagnitudVector(&wallPos));
	traslateVector(&wall45grados1, &wall45grados1, &wallVertice45gradosExt1, getMagnitudVector(&wallPos)+3);
	traslateVector(&wall45grados2, &wall45grados2, &wallVertice45gradosInt2, getMagnitudVector(&wallPos));
	traslateVector(&wall45grados2, &wall45grados2, &wallVertice45gradosExt2, getMagnitudVector(&wallPos)+3);
	glBegin(GL_QUADS); //6 caras en total
		//a b           e f
		//d c           h g
		vector3d a, b, c, d, e, f, g, h;
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
		
		if(     	 c1 < 5      && c2 < 5
				&&   c3 < doh    && c5 < doh
				&&   c4 < dog    && c6 < dog && !esInvencible){
					glColor3f(1.0f, 1.0f, 1.0f);
					loseLife();
					printf("%d", vidas);
		}
		//Pared Interna
		glNormal3f(-wallDir->x, 0.0, -wallDir->z);
		glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z); //0,1 1,1
		glVertex3f(c.x,c.y,c.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0
		//Pared Externa
		glNormal3f(wallDir->x, wallDir->y, wallDir->z);
		glVertex3f(e.x,e.y,e.z); glVertex3f(f.x,f.y,f.z); //0,1 1,1
		glVertex3f(g.x,g.y,g.z); glVertex3f(h.x,h.y,h.z); //1,0 0,0
		//Pared costado
		glNormal3f(-wallVertice45gradosInt1.z, wallVertice45gradosInt1.y, wallVertice45gradosInt1.x);
		glVertex3f(b.x,b.y,b.z); glVertex3f(f.x,f.y,f.z); //0,1 1,1
		glVertex3f(g.x,g.y,g.z); glVertex3f(c.x,c.y,c.z);//1,0 0,0		
		//Pared costado
		glNormal3f(wallVertice45gradosInt2.z, wallVertice45gradosInt2.y, -wallVertice45gradosInt2.x);
		glVertex3f(a.x,a.y,a.z); glVertex3f(e.x,e.y,e.z); //0,1 1,1
		glVertex3f(h.x,h.y,h.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0
		//Base de abajo
		glVertex3f(c.x,c.y,c.z); glVertex3f(g.x,g.y,g.z); //0,1 1,1
		glVertex3f(h.x,h.y,h.z); glVertex3f(d.x,d.y,d.z); //1,0 0,0			
		//Base de arriba
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(b.x,b.y,b.z); glVertex3f(a.x,a.y,a.z);  //0,1 1,1
		glVertex3f(e.x,e.y,e.z); glVertex3f(f.x,f.y,f.z);  //1,0 0,0		

	glEnd(); 
}

void makeGeometricShape(vector3d* direc, float distancia, int cantidadDeLados){
	vector3d direcUnitary;
	vector3d newDirec;
	float geometricAngle=360/cantidadDeLados;
	
	glPushMatrix(); //dibuja todas las paredes con una figura (pentagono, cuadrado, otros)
		getUnitaryVector(direc, &direcUnitary);
		for(int i=0;i<cantidadDeLados-1;i++){
			getRotatedVector(&upDirection, &direcUnitary, &newDirec, degreesToRadians(geometricAngle));
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
//>>>>>>> origin/CM
}

void pressKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP : 
			printf("%d", velocidadPersonaje);
			charMove = 0.5; 
			break;
        case GLUT_KEY_DOWN : 
			charMove = -0.5;
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

void mouseMove(int x, int y) { //[DEPRECATED]
    
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

void mouseButton(int button, int state, int x, int y) { //[DEPRECATED]
    
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

void drawWalls(int value){ //determina la direccion y cuantas paredes va a tener cada pared
	int maxrand=100;
	float step=0.6;
	if(wall.w1<0){
		wall.w1=initialDist;
		srand(time(NULL));
		wall.w1dir.x=rand()%maxrand-maxrand/2;
		wall.w1dir.y=0;
		wall.w1dir.z=rand()%maxrand-maxrand/2;
		wall.w1pared=rand()%5+4;
		}else{
		wall.w1=wall.w1-step;}
	if(wall.w2<0){wall.w2=initialDist;
		srand(time(NULL));
		wall.w2dir.x=rand()%maxrand-maxrand/2;
		wall.w2dir.y=0;
		wall.w2dir.z=rand()%maxrand-maxrand/2;
		wall.w2pared=rand()%5+4;
	}else{wall.w2=wall.w2-step;}
	if(wall.w3<0){wall.w3=initialDist;
			srand(time(NULL));
		wall.w3dir.x=rand()%maxrand-maxrand/2;
		wall.w3dir.y=0;
		wall.w3dir.z=rand()%maxrand-maxrand/2;
		wall.w3pared=rand()%5+4;
		}else{wall.w3=wall.w3-step;}
	if(wall.w4<0){wall.w4=initialDist;
			srand(time(NULL));
		wall.w4dir.x=rand()%maxrand-maxrand/2;
		wall.w4dir.y=0;
		wall.w4dir.z=rand()%maxrand-maxrand/2;
		wall.w4pared=rand()%5+4;
		}else{wall.w4=wall.w4-step;}
	if(wall.w5<0){wall.w5=initialDist;
			srand(time(NULL));
		wall.w5dir.x=rand()%maxrand-maxrand/2;
		wall.w5dir.y=0;
		wall.w5dir.z=rand()%maxrand-maxrand/2;
		wall.w5pared=rand()%5+4;
	}else{wall.w5=wall.w5-step;}
	if(wall.w6<0){wall.w6=initialDist;
			srand(time(NULL));
		wall.w6dir.x=rand()%maxrand-maxrand/2;
		wall.w6dir.y=0;
		wall.w6dir.z=rand()%maxrand-maxrand/2;
		wall.w6pared=rand()%5+4;
	}else{wall.w6=wall.w6-step;}
		
	//glutTimerFunc(50, drawWalls, 0);
    
}

void efectoVida(int value){ //agrega una vida
	vidas++;
}

void efectoInvencibilidad(int value){ //hace que el personaje sea invencible
	esInvencible=1;
	printf("sos invencible!");
}

void efectoVelocidad2(int value){ //duplica la velocidad del personaje
	if(value==1){
		velocidadPersonaje=initVelocidadPersonaje/2;
		glutTimerFunc(5000, efectoVelocidad2, 0);
	}else{
		velocidadPersonaje=initVelocidadPersonaje*2;
	}
}

void efectoVelocidadPared(int value){ //duplica la velocidad de la pared, falta hacer
	if(value==1){
		velocidadParedes=velocidadParedes/2;
		glutTimerFunc(5000, efectoVelocidadPared, 0);
	}else{
		velocidadParedes=velocidadParedes*2;
	}
}

void efectoPremio(int cualPremio){ //llama a cada una de las funciones, dependiendo de que premio comio, falta hacer
	switch (cualPremio){
		case vida: efectoVida(1); break;
		case invencibilidad: efectoInvencibilidad(1); break;
		case velocidad2: //efectoVelocidad2(1); break;
		case velocidadPared: //efectoVelocidadPared(1); break;
		case puntos1000: //efectoPuntos1000(1); break;
		case cambiaSentido: //efectoCambiaSentido(1); break;
		case iman: //efectoIman(1); break;
		break;
	}
}

void renderPremio(int cualPremio){ //dibuja el premio
    glPushMatrix();
                glColor3d(p[cualPremio].color.x, p[cualPremio].color.y, p[cualPremio].color.z);
                glTranslatef(p[cualPremio].pos.x, 1.0, p[cualPremio].pos.z);
                glutSolidSphere(2.0, 20.0, 20.0);
    glPopMatrix();
   
    
}

void drawPremios(){ //maneja cuando se pinta el premio o no
    srand(time(NULL));
    int maxrand=200;
	for(int i=0; i<7; i++){
		if(p[i].tiempo==6){ //cuando el tiempo es 6, genera una posicion random del premio
			p[i].pos.x=rand()%maxrand-maxrand/2;
			p[i].pos.y=1.0;
			p[i].pos.z=rand()%maxrand-maxrand/2;
		}else if(p[i].tiempo<6 && p[i].tiempo>0){ //
			renderPremio(i);
			if(distancePointToPoint(&p[i].pos,&characterPosition)<3.5){
				p[i].tiempo=10;
				efectoPremio(i);
			}
		}
	}
}

void premiosTimer(int value){ //maneja la variable de tiempo
	for(int i=0;i<7;i++){
		if(p[i].tiempo==0) p[i].tiempo=10; //reset cuando llega a 0
		else if(p[i].tiempo <= 10 && p[i].tiempo > 6){ //idle time
			p[i].tiempo--;	
		}
		else if(p[i].tiempo == 6){ //hay un 10% de probabilidades que pueda ser dibujado
			if((rand()%100)<= p[i].probabilidad*100) { 
				p[i].tiempo--;
			}else{
				p[i].tiempo=10;
			}
		}		//idle time
		else if(p[i].tiempo < 6) {
			p[i].tiempo--;
		}
		printf("%d ", p[i].tiempo);

	}
	if(p[invencibilidad].tiempo=6) esInvencible=0;
	printf("\n");
	//if(p[velocidad2].tiempo=6) //velocidadPersonaje=initVelocidadPersonaje;
	//glutTimerFunc(5000, premiosTimer, value);
}

void renderScene(void) { //maneja el entorno grafico
    
    if(charMove){ //calcula la nueva posicion del personaje
			traslateVector(&characterPosition, &characterDirection, &newPosition, charMove);
			copyVectorValues(&newPosition, &characterPosition);
	}
	if(charRotate){ //calcula la nueva direccion del personaje
			getRotatedVector(&upDirection, &characterDirection, &newDirection, charRotate*PI/100);
			copyVectorValues(&newDirection, &characterDirection);
	}
	
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Reset transformations
	glLoadIdentity();
	// Set the camera
        
	if(camera%3==0) //mirada desde arriba
        gluLookAt(	0, 400, 0,
                  0.0, 0.0,  0.0,
                  1.0f, 0.0f,  1.0f);
    else if(camera%3==1){ //mirada que le sigue al personaje desde una distancia
        gluLookAt(	0, 50, 50,
                0.0, 0.0,  0.0,
                  0.0f, 1.0f,  0.0f);
        gluLookAt(	characterPosition.x, 2.0, characterPosition.z,
                  characterDirection.x+characterPosition.x, 2.0, characterDirection.z+characterPosition.z,
                  0.0f, 1.0f,  0.0f);
    }
    else if(camera%3==2) //first person
        gluLookAt(	characterPosition.x, 2.0, characterPosition.z,
                  characterDirection.x+characterPosition.x, 2.0, characterDirection.z+characterPosition.z,
                  0.0f, 1.0f,  0.0f);
    
    
    
	// Dibuja debug de las direcciones
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
	glPopMatrix();
    
    
	//dibuja las paredes
    glPushMatrix();
				glColor3f(1.0f, 0.0f, 0.0f);
	makeGeometricShape(&wall.w1dir, wall.w1, wall.w1pared);
    			glColor3f(0.0f, 1.0f, 0.0f);
	makeGeometricShape(&wall.w2dir, wall.w2, wall.w2pared);
    			glColor3f(0.0f, 0.0f, 1.0f);
	makeGeometricShape(&wall.w3dir, wall.w3, wall.w3pared);
    			glColor3f(1.0f, 0.0f, 0.0f);
	makeGeometricShape(&wall.w4dir, wall.w4, wall.w4pared);
    			glColor3f(1.0f, 1.0f, 0.0f);
	makeGeometricShape(&wall.w5dir, wall.w5, wall.w5pared);
    			glColor3f(1.0f, 0.0f, 1.0f);
	makeGeometricShape(&wall.w6dir, wall.w6, wall.w6pared);
                glColor3f(1.0f, 1.0f, 0.0f);
	glPopMatrix();
	
	//dibuja al personaje
    renderCharacter();
    //dibuja los premios
    drawPremios();
    
	//dibuja el piso
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
    
	//dibuja las luces
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    GLfloat lightColor0[] = {1.9f, 1.9f, 1.9f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {50, 10, 50, 0.0f}; //Positioned at (50, 10, 50)
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

void timerGlobal(int value){ //maneja la llamada de las funciones que deben ser llamadas luego de un tiempo en especifico
	tiempo++;
	if(tiempo%110==0){
		premiosTimer(value);
	}
	if(tiempo%10==0){
		drawWalls(value);
	}
	if(banderaPerderVidas>0){
		banderaPerderVidas=banderaPerderVidas-10;
	}
	if(banderaVelocidad2>0){
		banderaVelocidad2=banderaVelocidad2-10;
	}
	glutTimerFunc(10, timerGlobal, 1);
}

void initValues(){
    
    //la distancia en la que empiezan a aparecer las paredes es 600
    initialDist=300;
    //camera inicial es la vista de hacia arriba
    camera = 0;

	vidas=5;
	tiempo=0;
	esInvencible=0;
	initVelocidadPersonaje=0.5;
	banderaVelocidad2=0;
	velocidadPersonaje=0.5;
	velocidadParedes=0;
	puntos=0;
	banderaPerderVidas=0;

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
    wall.w1=300;
	wall.w2=250;
	wall.w3=200;
	wall.w4=150;
	wall.w5=100;
	wall.w6=50;
    //direccion inicial de la pared w1 (usado para debug)
    wall.w1dir.x=-1;
    wall.w1dir.y=0;
    wall.w1dir.z=1;
    wall.w2dir.x=-1;
    wall.w2dir.y=0;
    wall.w2dir.z=1;
	wall.w3dir.x=-1;
    wall.w3dir.y=0;
    wall.w3dir.z=1;
    wall.w4dir.x=-1;
    wall.w4dir.y=0;
    wall.w4dir.z=1;
	wall.w5dir.x=-1;
    wall.w5dir.y=0;
    wall.w5dir.z=1;
	wall.w6dir.x=-1;
    wall.w6dir.y=0;
    wall.w6dir.z=1;
	wall.w1pared=4;
	wall.w2pared=4;
	wall.w3pared=4;
	wall.w4pared=4;
	wall.w5pared=4;
	wall.w6pared=4;
	
	//inicializacion de los premios
    //0.1
	p[vida].probabilidad = 0.9;
    p[vida].color.x=1.0f; //rojo
    p[vida].color.y=0.0f;
    p[vida].color.z=0.0f;
    p[vida].tiempo = 6;
    //0.1
    p[invencibilidad].probabilidad = 0.1;
    p[invencibilidad].color.x=1.0f; //blanco
    p[invencibilidad].color.y=1.0f;
    p[invencibilidad].color.z=1.0f;
    p[invencibilidad].tiempo = 6;
    
    p[velocidad2].probabilidad = 0.15;
    p[velocidad2].color.x=0.0f;	//azul
    p[velocidad2].color.y=1.0f;
    p[velocidad2].color.z=0.0f;
    p[velocidad2].tiempo = 6;
    
    p[velocidadPared].probabilidad = 0.15;
    p[velocidadPared].color.x=0.0f; //verde
    p[velocidadPared].color.y=0.0f;
    p[velocidadPared].color.z=1.0f;
    p[velocidadPared].tiempo = 6;
    
    //1 0.752941 0.796078 //prob 0.5
    p[puntos1000].probabilidad = 0.9;
    p[puntos1000].color.x=1.0f;			//rosado
    p[puntos1000].color.y=0.552941f;
    p[puntos1000].color.z=0.596078f;
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
//<<<<<<< HEAD
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH) ;
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800,600);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800,640);
//>>>>>>> origin/CM
	glutCreateWindow("HexxTreMaze");
    
	//LoadGLTextures();
	
	// register callbacks
	glutDisplayFunc(renderScene); //mostrar lo que esta en la funicon
    glutDisplayFunc(renderCharacter);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene); //cuando esta en espera llama funcion
    
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutTimerFunc(0, timerGlobal, 1);

    //glutTimerFunc(5000, premiosTimer, 0);
	// here are the two new functions
	//glutMouseFunc(mouseButton);
	//glutPassiveMotionFunc(mouseMove);
    
	// OpenGL init
//<<<<<<< HEAD
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	//Menu
	createPopupMenus();

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
//>>>>>>> origin/CM
	// enter GLUT event processing cycle
	glutMainLoop();

	
    
	return 1;
}











