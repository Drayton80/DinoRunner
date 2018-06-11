#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>

#ifndef BITMAP_H
#define BITMAP_H

void setOrtho(int width, int height){
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();

	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
}

void restoreView(){
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

void drawString(string s, float x, float y, string type){
	glColor3f(1,1,1);
	glRasterPos2f(x, y);

	if(type == "TYPE_SCORE")
		for (int i = 0; i < s.length(); i++) 
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);

	else if(type == "TYPE_GAME_OVER")
		for (int i = 0; i < s.length(); i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);

	else if(type == "TYPE_GAME_OVER_INFORMATION")
		for (int i = 0; i < s.length(); i++) 
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);

	else if(type == "TYPE_MENU_TITLE")
		for (int i = 0; i < s.length(); i++) 
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
		
	else if(type == "TYPE_MENU_INFORMATION")
		for (int i = 0; i < s.length(); i++)
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);	
}

void drawMenuTitle(float width, float height){
	glPushMatrix();
		glLoadIdentity();

		string title = ".:| DINO RUNNER |:.";
		string information = "Aperte qualquer tecla para prosseguir";

		unsigned short int titleLength = title.length();
		unsigned short int informationLength = information.length();

		drawString(title, (glutGet(GLUT_SCREEN_WIDTH) - width)/2 - titleLength, height/2, "TYPE_MENU_TITLE");
		drawString(information, (glutGet(GLUT_SCREEN_WIDTH) - width)/2 - informationLength, height/2 + 10, "TYPE_MENU_INFORMATION");
	glPopMatrix();
}

void drawLoading(float width, float height){
	glPushMatrix();
		glLoadIdentity();

		string information = "   CARREGANDO...";

		unsigned short int informationLength = information.length();

		drawString(information, (glutGet(GLUT_SCREEN_WIDTH) - width)/2 - informationLength, height/2 + 10, "TYPE_MENU_INFORMATION");
	glPopMatrix();
}

void drawScore(string score, int positionX){
	glPushMatrix();
		glLoadIdentity();
		
		//Bitmap da string "SCORE:" e o score
		drawString("SCORE: ", positionX - 100, 20, "TYPE_SCORE");
		drawString(score, positionX - 30, 20, "TYPE_SCORE");
	glPopMatrix();
}

void drawGameOver(float width, float height){
	glPushMatrix();
		glLoadIdentity();

		string title = "GAME OVER";
		string information = "Aperte Enter para jogar outra vez";

		unsigned short int titleLength = title.length();
		unsigned short int informationLength = information.length();

		drawString(title, 
                  (glutGet(GLUT_SCREEN_WIDTH) - width)/2 - titleLength, 
                  (glutGet(GLUT_SCREEN_HEIGHT) - height)/2, "TYPE_GAME_OVER");
		drawString(information, 
                  (glutGet(GLUT_SCREEN_WIDTH) - width)/2 - informationLength, 
                  (glutGet(GLUT_SCREEN_HEIGHT) - height)/2 + 15, "TYPE_GAME_OVER_INFORMATION");
	glPopMatrix();
}	

#endif