#include <string.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include <stdio.h>

#define TEXTPRINCIPAL 1

GLuint texture[1];

AUX_RGBImageRec *TextureImage[4];

AUX_RGBImageRec *LoadBMP(char *Filename)       //carga un bitmap
{
	FILE *File=NULL;
	if (!Filename) return NULL;
	File=fopen(Filename,"r");
	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

void LiberarTextura(int aux)
{
	if (TextureImage[aux]) 
	{
		if (TextureImage[aux]->data)
		{
			free(TextureImage[aux]->data);
		}
		free(TextureImage[aux]);
	}
     
}


int  CargarTextura (int aux){
     int Status=FALSE;
	
        Status=TRUE;
		glGenTextures(1, &texture[aux]);
		glBindTexture(GL_TEXTURE_2D, texture[aux]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3,TextureImage[aux]-> sizeX, TextureImage[aux]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[aux] ->data);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		LiberarTextura(aux);
		
	return Status;	
}
int LoadGLTextures()            // Convierte el bitmap a textura
{

	int Status=FALSE;
	
	memset(TextureImage,0,sizeof(void *)*1);

if (TextureImage[TEXTPRINCIPAL]=LoadBMP("images/portada.bmp")){ CargarTextura(TEXTPRINCIPAL);}
/*if (TextureImage[TexMont]=LoadBMP("mountains.bmp")){    CargarTextura(TexMont); }
if (TextureImage[VecRac]=LoadBMP("VectorRacing.bmp")){    CargarTextura(VecRac); }  	
if (TextureImage[VecRac01]=LoadBMP("VectorRacing01.bmp")){    CargarTextura(VecRac01); }  	
if (TextureImage[GameOver]=LoadBMP("game-over.bmp")){    CargarTextura(GameOver); }
if (TextureImage[Ranking]=LoadBMP("menu.bmp")){    CargarTextura(Ranking); }*/
	return Status;
}
