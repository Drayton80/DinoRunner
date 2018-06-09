#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>

#ifndef BITMAP_H
#define BITMAP_H

void setOrtho(){
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();

	glLoadIdentity();
	glOrtho(0.0f, IMAGE_WIDTH, IMAGE_HEIGHT, 0.0f, 0.0f, 1.0f);
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
}

void restoreView(){
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

void drawString(string s, float x, float y){
	glColor3f(1,1,1);
	glRasterPos2f(x, y);

	for (int i = 0; i < s.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, s[i]);
	}
}

void gameover(){

	drawString("GAME OVER", 0, 0);
}	

#endif