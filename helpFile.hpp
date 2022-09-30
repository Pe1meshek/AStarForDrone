#ifndef HELPFILE_H
#define HELPFILE_H

#define GL_CLAMP_TO_EDGE 0x812F
#include <SFML/OpenGL.hpp>
// #include <ctime>
// #include <vector>
using namespace sf;
// using namespace std;


GLuint LoadTexture(String name)
{
	Image image;
	image.loadFromFile(name);
	// image.flipVertically();	// переворачивает текстуру
	
	GLuint texture = 0;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,image.getSize().x,image.getSize().y,GL_RGBA,GL_UNSIGNED_BYTE,image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	// задаём: цвет текселя полностью замещает цвет фрагмента фигуры
	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	return texture;
}

void createBox(GLuint box[], float size)
{
	// srand(time(0));
	// vector<int> base = {0,1,2,3,4,5};
	// vector<int> res;
	// int a;
	// while(base.size())
	// {
		// a = rand()%base.size();
		// res.push_back(base[a]);
		// base.erase(base.begin()+a);
	// }

	glBindTexture(GL_TEXTURE_2D,box[0]);
	glBegin(GL_QUADS);
		//Back
		glTexCoord2f(0, 0); glVertex3f(size, size, -size);
		glTexCoord2f(1, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 1); glVertex3f(-size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(size, -size, -size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,box[1]);
	glBegin(GL_QUADS);
		//Front
		glTexCoord2f(0, 0); glVertex3f(-size, size, size);
		glTexCoord2f(1, 0); glVertex3f(size, size, size);
		glTexCoord2f(1, 1); glVertex3f(size, -size, size);
		glTexCoord2f(0, 1); glVertex3f(-size, -size, size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,box[2]);
	glBegin(GL_QUADS);
		//Left
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f(-size, size, size);
		glTexCoord2f(1, 1); glVertex3f(-size, -size, size);
		glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,box[3]);
	glBegin(GL_QUADS);
		//Right
		glTexCoord2f(0, 0); glVertex3f(size, size, size);
		glTexCoord2f(1, 0); glVertex3f(size, size, -size);
		glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(size, -size, size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,box[4]);
	glBegin(GL_QUADS);
		//Bottom
		glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
		glTexCoord2f(1, 0); glVertex3f(size, -size, size);
		glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,box[5]);
	glBegin(GL_QUADS);
		//Top
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f(size, size, -size);
		glTexCoord2f(1, 1); glVertex3f(size, size, size);
		glTexCoord2f(0, 1); glVertex3f(-size, size, size);
	glEnd();
}


#endif