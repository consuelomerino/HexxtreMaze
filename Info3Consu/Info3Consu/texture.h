/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Tutorial 4: 3d engine - 3ds models loader
 *
 * File header: texture.h
 *
 */



/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/

// Counter to keep track of the last loaded texture
extern int num_texture;

typedef struct
{
    GLubyte *dibujo;        // Un puntero a los datos de la imagen
    GLuint	bpp;		// bpp significa bits per pixel (bits por punto) es la calidad en palabras sensillas
    GLuint largo;		// Largo de la textura
    GLuint ancho;		// Ancho de la textura
    GLuint ID;		// ID de la textura, es como su nombre para opengl
} textura;

/**********************************************************
 *
 * FUNCTION LoadBitmap(char *)
 *
 * This function loads a bitmap file and return the OpenGL reference ID to use that texture
 *
 *********************************************************/

extern int LoadBitmap(char *filename);
extern int cargarTGA( char *nombre, textura *imagen);
