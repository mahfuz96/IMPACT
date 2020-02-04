

#include <SOIL.h>
#include <GLFW\glfw3.h>

#include<random>

#include "primitive.h"
#include "vertex2f.h"



#include <iostream>
#include <vector>

#define NUM_OF_CONSTENEMY 5
#define NUM_OF_POWERFULENEMY 15
#define triggereecountsize 1000
#define NUM_OF_PLATFORMS 20
#define MAX_CAVE 10
#define MAX_CACTUS 10


int WIDTH = 1366, HEIGHT = 768;
int triggercount = 0, triggerencount = 0, constenemycount = 0, gamescore = 0, score;
int triggercount1 = 0;

float xspeed = 0.9f, curTime, prevTime = 0.0f, dt;
float playerypos, playerxpos;
float trigyspeed = 0.5f;

bool rightBullet = false;
bool  playmovexright = false, playmovexleft = false, trigger = false, gameend = false, bulletmovementx = true,playerJump=false,playerBack=false;
bool collid[5] = { false, false, false, false, false };
bool collid1[5] = { false, false, false, false, false };
bool stop = false;
bool menubool = false;

primitive constenemy[NUM_OF_CONSTENEMY];
//primitive powerfulenemy[NUM_OF_POWERFULENEMY];
primitive background,background2, gameover;
primitive mainplayer;
primitive platforms[NUM_OF_PLATFORMS];
primitive cave[MAX_CAVE];
primitive cactus[MAX_CACTUS];
primitive number;
primitive menu;



int size = 10;
primitive *triggershape = new primitive[size];
primitive *triggershape1 = new primitive[size];

primitive triggerenemyshape[1000][5];


GLint loadTexture(const char * filename)
{
	GLuint textureID;
	unsigned char * image;
	int width, height;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
 	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

 	 SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}


void Menu()
{
	menu = primitive(vertex2f(-1.0f, -1.0f), vertex2f(-1.0f, 1), vertex2f(1, 1), vertex2f(1, -1));
	
	GLuint texID2 = loadTexture("./asset/play_game.png");
	menu.setTextureID(texID2);

}
void createmainbackground() {
	

	background = primitive(vertex2f(-1.0f, -1.0f), vertex2f(-1.0f, 1), vertex2f(1, 1), vertex2f(1, -1));
	background2 = primitive(vertex2f(1.0f, 1.0f), vertex2f(3.0f, 1), vertex2f(1, 1), vertex2f(1, 1));
	GLuint texID1 = loadTexture("./asset/star.png");
	GLuint texID2= loadTexture("./asset/star2.png");
	background.setTextureID(rand() % 2 == 0 ? texID1 : texID2);

}
void createPlatform() {
	vertex2f position(-1, -1.0f);
	GLuint texID = loadTexture("./asset/platform.png");
	for (int i = 0; i < NUM_OF_PLATFORMS; i++) {
		platforms[i] = primitive(vertex2f(-0.2f, -0.9), vertex2f(-0.2f, -0.7f), vertex2f(0, -0.7f), vertex2f(0, -0.9f));
		platforms[i].setTextureID(texID);
		platforms[i].setPosition(position);
		position.x += 0.2f;
	}
}
void createNumber()
{
	vertex2f position(-0.8, .8f);
	
	GLuint texID2 = loadTexture("./asset/numbers/0.png");
	
		number= primitive(vertex2f(-0.7f, .8f), vertex2f(-0.7f, .8f), vertex2f(-0.6, .8f), vertex2f(-0.6, 0.8f));
		number.setTextureID(texID2);
		number.setPosition(position);

		

	
}

void createBox() {
	vertex2f position(0.5, -.8f);
	GLuint texID1 = loadTexture("./asset/pipe.png");
	GLuint texID2 = loadTexture("./asset/box.png");
	for (int i = 0; i < MAX_CAVE; i++) {
		cave[i] = primitive(vertex2f(-0.2f, -0.2f), vertex2f(-0.2f, .1f), vertex2f(0, .1f), vertex2f(0, -0.2f));
		cave[i].setTextureID(rand() % 2 == 0 ? texID1 : texID2);
		cave[i].setPosition(position);
		
		position.x += (rand() % 250) * 0.01f;
		
	}  
}

void createBarrel() {
	vertex2f position(0.2, -0.8);
	GLuint texID1 = loadTexture("./asset/barrel1.png");
	GLuint texID2 = loadTexture("./asset/barrel2.png");
	for (int i = 0; i < MAX_CACTUS; i++) {
		cactus[i] = primitive(vertex2f(-0.1f, 0), vertex2f(-0.2f, 0.2f), vertex2f(0, 0.1f), vertex2f(0, 0));
		cactus[i].setTextureID(rand() % 2 == 0 ? texID1 : texID2);
		cactus[i].setPosition(position);
		position.x += (rand() % 200) * 0.01f;
		std::cout << "barrel : " << position.x << std::endl;
		
	}
	
}





void createConstEnemy() {
	vertex2f position(-2, 0.6);
	for (int i = 0; i < NUM_OF_CONSTENEMY; i++) {
		constenemy[i] = primitive(vertex2f(-0.1f, 0.1f
		), vertex2f(-0.1f, 0.2f), vertex2f(-0.0f, 0.2f), vertex2f(-0.0f, 0));
		GLuint texID2 = loadTexture("./asset/enemy.png");
		constenemy[i].setTextureID(texID2);
		position.x += (rand() % 250) * 0.01f;
		constenemy[i].setPosition(position);
	}

}






void createmainPlayer() {


	mainplayer = primitive(vertex2f(-0.2f, -0.8f), vertex2f(-0.2f, -0.6f), vertex2f(0.0f, -0.6f), vertex2f(0.0, -0.80f));
	GLuint texID2 = loadTexture("./asset/player.png");
	mainplayer.setTextureID(texID2);

}




void createTrigger() {
	vertex2f position1;
	triggershape[triggercount] = primitive(vertex2f(-0.2f, 0), vertex2f(-0.2f, 0.05f), vertex2f(-0.19f, 0.05f), vertex2f(-0.19f, 0));
	GLuint texID2 = loadTexture("./asset/bullet1.png");
	triggershape[triggercount].setTextureID(texID2);

	position1.y = mainplayer.v2.y - 0.09f;
	position1.x = mainplayer.v2.x + 0.1f;

	triggershape[triggercount].setPosition(position1);

}
void createTriggerRight()
{
	vertex2f position1;
	triggershape1[triggercount1] = primitive(vertex2f(-0.2f, 0), vertex2f(-0.2f, 0.05f), vertex2f(-0.19f, 0.05f), vertex2f(-0.19f, 0));
	GLuint texID2 = loadTexture("./asset/bullet1.png");
	triggershape1[triggercount1].setTextureID(texID2);

	position1.y = mainplayer.v2.y - 0.09f;
	position1.x = mainplayer.v2.x + 0.1f;

	triggershape1[triggercount1].setPosition(position1);
}




void createEnemyTrigger(int m) {


	vertex2f position1;
	triggerencount = 1;
	while (triggerencount % 2 != 0)
	{
		triggerenemyshape[constenemycount][triggerencount] = primitive(vertex2f(-0.2f, 0), vertex2f(-0.2f, 0.05f), vertex2f(-0.19f, 0.05f), vertex2f(-0.19f, 0));
		GLuint texID2 = loadTexture("./asset/bullet.png");
		triggerenemyshape[constenemycount][triggerencount].setTextureID(texID2);

		constenemy[m].v1.y -= 0.15;

		position1.y = constenemy[m].v1.y;
		position1.x = constenemy[m].v1.x;

		triggerenemyshape[constenemycount][triggerencount].setPosition(position1);

		triggerencount++;
	}



}




void gameoverscreen()
{
	gameover = primitive(vertex2f(-1.0f, -1.0f), vertex2f(-1.0f, 1), vertex2f(1, 1), vertex2f(1, -1));
	GLuint texID1 = loadTexture("./asset/game_over.png");
	gameover.setTextureID(texID1);
}



void update1()
{
	for (int i = 0; i < NUM_OF_CONSTENEMY; i++)
	{
		if (!collid[i]) {
			if (constenemy[i].v1.x > -1 || constenemy[i].v1.x < 1)
			{

				if (rand() % 2 == 1)
				{

					createEnemyTrigger(i);
					constenemycount++;
				}

			}

		}


	}
	bulletmovementx = false;
}

void backgroundscroll() 
{
	if (background.v2.x < -0.99)
	{

		background.v1.x = 1;
		background.v1.x = -1;
		background.v2.x = 3;
		background.v2.x = -1;
		background.v3.x = 3;
		background.v3.x = 1;
		background.v4.x = 1;
		background.v4.x = 1;

	}
	if (background2.v2.x < -0.99)
	{
		background2.v1.x = 1;
		background2.v1.x = -1;
		background2.v2.x = 3;
		background2.v2.x = -1;
		background2.v3.x = 3;
		background2.v3.x = 1;
		background2.v4.x = 1;
		background2.v4.x = 1;


	}
}





	void update()
	{

	background;
	background2;
	backgroundscroll();
	vertex2f position;



	curTime = glfwGetTime();

	dt = curTime - prevTime;

	prevTime = curTime;


	
	if (playmovexright)
	{
		mainplayer.v1.x += xspeed*dt;


		mainplayer.v2.x += xspeed*dt;


		mainplayer.v3.x += xspeed*dt;
		mainplayer.v4.x += xspeed*dt;
		if (mainplayer.v3.x > 1.0f)
			playmovexright = false;

	}
	

	if (playmovexleft)
	{
		mainplayer.v1.x -= xspeed*dt;


		mainplayer.v2.x -= xspeed*dt;


		mainplayer.v3.x -= xspeed*dt;
		mainplayer.v4.x -= xspeed*dt;

		if (mainplayer.v1.x < -1.0f)
			playmovexleft = false;
	}
	if (playerJump)
	{
		mainplayer.v1.y += xspeed * dt;


		mainplayer.v2.y += xspeed * dt;


		mainplayer.v3.y += xspeed * dt;
		mainplayer.v4.y += xspeed * dt;
	}
	if (playerBack && !playerJump)

	{
		mainplayer.v1.y -= xspeed * dt;


		mainplayer.v2.y -= xspeed * dt;


		mainplayer.v3.y -= xspeed * dt;
		mainplayer.v4.y -= xspeed * dt;

		
		
	}
	if (mainplayer.v3.y <= -0.6f)
	{
		playerBack = false;
		
	}

	

	for (int z = 0; z < constenemycount; z++)
	{
		triggerencount = 1;

		while (triggerencount % 5 != 0)
		{

			triggerenemyshape[z][triggerencount].v1.y -= trigyspeed*dt;


			triggerenemyshape[z][triggerencount].v2.y -= trigyspeed*dt;


			triggerenemyshape[z][triggerencount].v3.y -= trigyspeed*dt;

			triggerenemyshape[z][triggerencount].v4.y -= trigyspeed*dt;


			
			if (((mainplayer.v1.y < triggerenemyshape[z][triggerencount].v3.y) && (mainplayer.v1.x < triggerenemyshape[z][triggerencount].v3.x)) && ((mainplayer.v3.y  > triggerenemyshape[z][triggerencount].v1.y) && (mainplayer.v3.x >triggerenemyshape[z][triggerencount].v1.x)))

			{
				if (!gameend && menubool)
					std::cout << "\nbullet\n";
				gameend = true;


			}
			triggerencount++;
			
		}
		
		
		


	}
	if (!rightBullet)
	{
		
		createTriggerRight();
	}
	if (rightBullet)
	{
		
		createTriggerRight();
	}
	
	for (int j = 0; j < MAX_CAVE; j++) {
		if (((mainplayer.v1.y < cave[j].v3.y) && (mainplayer.v1.x <  cave[j].v3.x)) && ((mainplayer.v3.y  >  cave[j].v1.y) && (mainplayer.v3.x > cave[j].v1.x)))
		{
			if (!gameend)
				
			gameend = true;


		}
	}

	for (int j = 0; j < MAX_CACTUS; j++) {
		if (((mainplayer.v1.y < cactus[j].v3.y) && (mainplayer.v1.x <  cactus[j].v3.x)) && ((mainplayer.v3.y  >  cactus[j].v1.y) && (mainplayer.v3.x > cactus[j].v1.x)))
		{



		}
	}

	for (int i = 0; i < NUM_OF_PLATFORMS; i++) {
		position = platforms[i].getPosition();
		position.x -= 0.01;
		if (position.x < -1.4f)
			position.x += (NUM_OF_PLATFORMS - 1) * platforms[i].width;
		platforms[i].setPosition(position);
	}

	for (int i = 0; i < MAX_CAVE; i++) {
		position = cave[i].getPosition();
		position.x -= 0.01;
		cave[i].setPosition(position);
	}

	for (int i = 0; i < MAX_CACTUS; i++) {
		position = cactus[i].getPosition();
		position.x -= 0.01;
		cactus[i].setPosition(position);
	}

	

	if (trigger)
	{

		createTrigger();

	}

	if (!trigger)
	{
		createTrigger();

	}

	

	for (int i = 0; i < triggercount; i++)
	{

		score = i;

		triggershape[i].v1.y += trigyspeed*dt;


		triggershape[i].v2.y += trigyspeed*dt;


		triggershape[i].v3.y += trigyspeed*dt;

		triggershape[i].v4.y += trigyspeed*dt;


		for (int j = 0; j < NUM_OF_CONSTENEMY; j++) {

			if ((triggershape[i].v2.y < constenemy[j].v2.y) && (triggershape[i].v2.y > constenemy[j].v1.y) && (triggershape[i].v2.x > constenemy[j].v1.x) && (triggershape[i].v3.x < constenemy[j].v4.x))
			{

				collid[j] = true;
				gamescore++;

				if (triggershape[i].v2.y == constenemy[j].v1.y) {
					gamescore++;
					std::cout << gamescore<<std::endl;
				}

			}
		}
		if (triggershape[i].v2.y > 1.2f)
		{
			
		}
	}

	

	for (int i = 0; i < triggercount1; i++)
	{

		score = i;

		triggershape1[i].v1.x += trigyspeed * dt;


		triggershape1[i].v2.x += trigyspeed * dt;


		triggershape1[i].v3.x += trigyspeed * dt;

		triggershape1[i].v4.x += trigyspeed * dt;


		for (int j = 0; j < MAX_CACTUS; j++) {

						
			if ((triggershape1[i].v2.y < cactus[j].v2.y) && (triggershape1[i].v2.y > cactus[j].v1.y) && (triggershape1[i].v2.x > cactus[j].v1.x) && (triggershape1[i].v3.x < cactus[j].v4.x))
			{

				collid1[j] = true;
				//std::cout << "endl";
				gamescore++;

				vertex2f pos (-1.2f, -1.2f);
				triggershape1[i].setPosition(pos);
				if (triggershape1[i].v2.x == cactus[j].v1.x) {
					gamescore++;
				
				}

			}
		}
	}



	

	for (int i = 0; i < NUM_OF_CONSTENEMY; i++) {
		position = constenemy[i].getPosition();
		
		position.x -= 0.9*dt;
		if (position.x < -1.2f)
		{
			bulletmovementx = true;
			
			position.x += (rand() % 10) + (NUM_OF_CONSTENEMY - 1) * constenemy[i].width;
			collid[i] = false;

		}

		constenemy[i].setPosition(position);
	}
	for (int i = 0; i < MAX_CACTUS; i++) {
		position = cactus[i].getPosition();
		
		position.x -= 0.01*dt;
		if (position.x < -1.2f)
		{
			bulletmovementx = true;
		
			position.x += (rand() % 10) + (MAX_CACTUS - 1) * cactus[i].width;
			
		}
		

		cactus[i].setPosition(position);
	}

	if (gamescore == 1)
	{
		
		std::cout << ".....";
		GLuint txt = loadTexture("./asset/numbers/1.png");
		number.setTextureID(txt);
	}
	if (gamescore == 2)
	{
		GLuint txt = loadTexture("./asset/numbers/2.png");
		number.setTextureID(txt);
	}
	if (gamescore == 3)
	{
		GLuint txt = loadTexture("./asset/numbers/3.png");
		number.setTextureID(txt);
	}
	if (gamescore == 4)
	{
		GLuint txt = loadTexture("./asset/numbers/4.png");
		number.setTextureID(txt);
	}
	
	if (gamescore == 5)
	{
		GLuint txt = loadTexture("./asset/numbers/5.png");
		number.setTextureID(txt);
	}
	if (gamescore == 6)
	{
		GLuint txt = loadTexture("./asset/numbers/6.png");
		number.setTextureID(txt);
	}
	
	if (gamescore == 7)
	{
		GLuint txt = loadTexture("./asset/numbers/7.png");
		number.setTextureID(txt);
	}
	if (gamescore == 8)
	{
		GLuint txt = loadTexture("./asset/numbers/8.png");
		number.setTextureID(txt);
	}
	if (gamescore == 9)
	{
		GLuint txt = loadTexture("./asset/numbers/9.png");
		number.setTextureID(txt);
	}
	if (gamescore == 10)
	{
		GLuint txt = loadTexture("./asset/numbers/10.png");
		number.setTextureID(txt);
	}
	if (gamescore == 11)
	{
		GLuint txt = loadTexture("./asset/numbers/11.png");
		number.setTextureID(txt);
	}
	if (gamescore == 12)
	{
		GLuint txt = loadTexture("./asset/numbers/12.png");
		number.setTextureID(txt);
	}
	if (gamescore == 13)
	{
		GLuint txt = loadTexture("./asset/numbers/13.png");
		number.setTextureID(txt);
	}
	if (gamescore == 14)
	{
		GLuint txt = loadTexture("./asset/numbers/14.png");
		number.setTextureID(txt);
	}
	if (gamescore == 15)
	{
		GLuint txt = loadTexture("./asset/numbers/15.png");
		number.setTextureID(txt);
	}
	if (gamescore == 16)
	{
		GLuint txt = loadTexture("./asset/numbers/16.png");
		number.setTextureID(txt);
	}
	if (gamescore == 17)
	{
		GLuint txt = loadTexture("./asset/numbers/17.png");
		number.setTextureID(txt);
	}
	if (gamescore == 18)
	{
		GLuint txt = loadTexture("./asset/numbers/18.png");
		number.setTextureID(txt);
	}
	if (gamescore == 19)
	{
		GLuint txt = loadTexture("./asset/numbers/19.png");
		number.setTextureID(txt);
	}
	if (gamescore == 20)
	{
		GLuint txt = loadTexture("./asset/numbers/20.png");
		number.setTextureID(txt);
	}
	if (gamescore == 21)
	{
		GLuint txt = loadTexture("./asset/numbers/21.png");
		number.setTextureID(txt);
	}
	if (gamescore == 22)
	{
		GLuint txt = loadTexture("./asset/numbers/22.png");
		number.setTextureID(txt);
	}
	if (gamescore == 23)
	{
		GLuint txt = loadTexture("./asset/numbers/23.png");
		number.setTextureID(txt);
	}
	if (gamescore == 24)
	{
		GLuint txt = loadTexture("./asset/numbers/24.png");
		number.setTextureID(txt);
	}
	if (gamescore == 25)
	{
		GLuint txt = loadTexture("./asset/numbers/25.png");
		number.setTextureID(txt);
	}
	if (gamescore == 26)
	{
		GLuint txt = loadTexture("./asset/numbers/26.png");
		number.setTextureID(txt);
	}
	if (gamescore == 27)
	{
		GLuint txt = loadTexture("./asset/numbers/27.png");
		number.setTextureID(txt);
	}
	if (gamescore == 28)
	{
		GLuint txt = loadTexture("./asset/numbers/28.png");
		number.setTextureID(txt);
	}
	if (gamescore == 29)
	{
		GLuint txt = loadTexture("./asset/numbers/29.png");
		number.setTextureID(txt);
	}
	if (gamescore == 30)
	{
		GLuint txt = loadTexture("./asset/numbers/30.png");
		number.setTextureID(txt);
	}
	if (gamescore == 31)
	{
		GLuint txt = loadTexture("./asset/numbers/31.png");
		number.setTextureID(txt);
	}
	if (gamescore == 32)
	{
		GLuint txt = loadTexture("./asset/numbers/32.png");
		number.setTextureID(txt);
	}
	if (gamescore == 33)
	{
		GLuint txt = loadTexture("./asset/numbers/33.png");
		number.setTextureID(txt);
	}
	if (gamescore == 34)
	{
		GLuint txt = loadTexture("./asset/numbers/34.png");
		number.setTextureID(txt);
	}
	if (gamescore == 35)
	{
		GLuint txt = loadTexture("./asset/numbers/35.png");
		number.setTextureID(txt);
	}
	if (gamescore == 36)
	{
		GLuint txt = loadTexture("./asset/numbers/36.png");
		number.setTextureID(txt);
	}
	if (gamescore == 37)
	{
		GLuint txt = loadTexture("./asset/numbers/37.png");
		number.setTextureID(txt);
	}
	if (gamescore == 38)
	{
		GLuint txt = loadTexture("./asset/numbers/38.png");
		number.setTextureID(txt);
	}
	if (gamescore == 39)
	{
		GLuint txt = loadTexture("./asset/numbers/39.png");
		number.setTextureID(txt);
	}
	if (gamescore == 40)
	{
		GLuint txt = loadTexture("./asset/numbers/40.png");
		number.setTextureID(txt);
	}
	if (gamescore == 41)
	{
		GLuint txt = loadTexture("./asset/numbers/41.png");
		number.setTextureID(txt);
	}
	if (gamescore == 42)
	{
		GLuint txt = loadTexture("./asset/numbers/42.png");
		number.setTextureID(txt);
	}
	if (gamescore == 43)
	{
		GLuint txt = loadTexture("./asset/numbers/43.png");
		number.setTextureID(txt);
	}
	if (gamescore == 44)
	{
		GLuint txt = loadTexture("./asset/numbers/44.png");
		number.setTextureID(txt);
	}
	if (gamescore == 45)
	{
		GLuint txt = loadTexture("./asset/numbers/45.png");
		number.setTextureID(txt);
	}
	if (gamescore == 46)
	{
		GLuint txt = loadTexture("./asset/numbers/46.png");
		number.setTextureID(txt);
	}
	if (gamescore == 47)
	{
		GLuint txt = loadTexture("./asset/numbers/47.png");
		number.setTextureID(txt);
	}
	if (gamescore == 48)
	{
		GLuint txt = loadTexture("./asset/numbers/48.png");
		number.setTextureID(txt);
	}
	if (gamescore == 49)
	{
		GLuint txt = loadTexture("./asset/numbers/49.png");
		number.setTextureID(txt);
	}
	if (gamescore == 50)
	{
		GLuint txt = loadTexture("./asset/numbers/50.png");
		number.setTextureID(txt);
	}
	if (gamescore == 51)
	{
		GLuint txt = loadTexture("./asset/numbers/51.png");
		number.setTextureID(txt);
	}
	if (gamescore == 52)
	{
		GLuint txt = loadTexture("./asset/numbers/52.png");
		number.setTextureID(txt);
	}
	if (gamescore == 53)
	{
		GLuint txt = loadTexture("./asset/numbers/53.png");
		number.setTextureID(txt);
	}
	if (gamescore == 54)
	{
		GLuint txt = loadTexture("./asset/numbers/54.png");
		number.setTextureID(txt);
	}
	if (gamescore == 55)
	{
		GLuint txt = loadTexture("./asset/numbers/55.png");
		number.setTextureID(txt);
	}
	if (gamescore == 56)
	{
		GLuint txt = loadTexture("./asset/numbers/56.png");
		number.setTextureID(txt);
	}
	if (gamescore == 57)
	{
		GLuint txt = loadTexture("./asset/numbers/57.png");
		number.setTextureID(txt);
	}
	if (gamescore == 58)
	{
		GLuint txt = loadTexture("./asset/numbers/58.png");
		number.setTextureID(txt);
	}
	if (gamescore == 59)
	{
		GLuint txt = loadTexture("./asset/numbers/59.png");
		number.setTextureID(txt);
	}
	if (gamescore == 60)
	{
		GLuint txt = loadTexture("./asset/numbers/60.png");
		number.setTextureID(txt);
	}
	if (gamescore == 61)
	{
		GLuint txt = loadTexture("./asset/numbers/61.png");
		number.setTextureID(txt);
	}
	if (gamescore == 62)
	{
		GLuint txt = loadTexture("./asset/numbers/62.png");
		number.setTextureID(txt);
	}
	if (gamescore == 63)
	{
		GLuint txt = loadTexture("./asset/numbers/63.png");
		number.setTextureID(txt);
	}
	if (gamescore == 64)
	{
		GLuint txt = loadTexture("./asset/numbers/64.png");
		number.setTextureID(txt);
	}
	if (gamescore == 65)
	{
		GLuint txt = loadTexture("./asset/numbers/65.png");
		number.setTextureID(txt);
	}
	if (gamescore == 66)
	{
		GLuint txt = loadTexture("./asset/numbers/66.png");
		number.setTextureID(txt);
	}
	if (gamescore == 67)
	{
		GLuint txt = loadTexture("./asset/numbers/67.png");
		number.setTextureID(txt);
	}
	if (gamescore == 68)
	{
		GLuint txt = loadTexture("./asset/numbers/68.png");
		number.setTextureID(txt);
	}
	if (gamescore == 69)
	{
		GLuint txt = loadTexture("./asset/numbers/69.png");
		number.setTextureID(txt);
	}
	if (gamescore == 70)
	{
		GLuint txt = loadTexture("./asset/numbers/70.png");
		number.setTextureID(txt);
	}
	if (gamescore == 71)
	{
		GLuint txt = loadTexture("./asset/numbers/71.png");
		number.setTextureID(txt);
	}
	if (gamescore == 72)
	{
		GLuint txt = loadTexture("./asset/numbers/72.png");
		number.setTextureID(txt);
	}
	if (gamescore == 73)
	{
		GLuint txt = loadTexture("./asset/numbers/73.png");
		number.setTextureID(txt);
	}
	if (gamescore == 74)
	{
		GLuint txt = loadTexture("./asset/numbers/74.png");
		number.setTextureID(txt);
	}
	if (gamescore == 75)
	{
		GLuint txt = loadTexture("./asset/numbers/75.png");
		number.setTextureID(txt);
	}
	if (gamescore == 76)
	{
		GLuint txt = loadTexture("./asset/numbers/76.png");
		number.setTextureID(txt);
	}
	if (gamescore == 77)
	{
		GLuint txt = loadTexture("./asset/numbers/77.png");
		number.setTextureID(txt);
	}
	if (gamescore == 78)
	{
		GLuint txt = loadTexture("./asset/numbers/78.png");
		number.setTextureID(txt);
	}
	if (gamescore == 79)
	{
		GLuint txt = loadTexture("./asset/numbers/79.png");
		number.setTextureID(txt);
	}
	if (gamescore == 80)
	{
		GLuint txt = loadTexture("./asset/numbers/80.png");
		number.setTextureID(txt);
	}
	if (gamescore == 81)
	{
		GLuint txt = loadTexture("./asset/numbers/81.png");
		number.setTextureID(txt);
	}
	if (gamescore == 82)
	{
		GLuint txt = loadTexture("./asset/numbers/82.png");
		number.setTextureID(txt);
	}
	if (gamescore == 83)
	{
		GLuint txt = loadTexture("./asset/numbers/83.png");
		number.setTextureID(txt);
	}
	if (gamescore == 84)
	{
		GLuint txt = loadTexture("./asset/numbers/84.png");
		number.setTextureID(txt);
	}
	if (gamescore == 85)
	{
		GLuint txt = loadTexture("./asset/numbers/85.png");
		number.setTextureID(txt);
	}
	if (gamescore == 86)
	{
		GLuint txt = loadTexture("./asset/numbers/86.png");
		number.setTextureID(txt);
	}
	if (gamescore == 87)
	{
		GLuint txt = loadTexture("./asset/numbers/87.png");
		number.setTextureID(txt);
	}
	if (gamescore == 88)
	{
		GLuint txt = loadTexture("./asset/numbers/88.png");
		number.setTextureID(txt);
	}
	if (gamescore == 89)
	{
		GLuint txt = loadTexture("./asset/numbers/89.png");
		number.setTextureID(txt);
	}
	if (gamescore == 90)
	{
		GLuint txt = loadTexture("./asset/numbers/90.png");
		number.setTextureID(txt);
	}
	if (gamescore == 91)
	{
		GLuint txt = loadTexture("./asset/numbers/91.png");
		number.setTextureID(txt);
	}
	if (gamescore == 92)
	{
		GLuint txt = loadTexture("./asset/numbers/92.png");
		number.setTextureID(txt);
	}
	if (gamescore == 93)
	{
		GLuint txt = loadTexture("./asset/numbers/93.png");
		number.setTextureID(txt);
	}
	if (gamescore == 94)
	{
		GLuint txt = loadTexture("./asset/numbers/94.png");
		number.setTextureID(txt);
	}
	if (gamescore == 95)
	{
		GLuint txt = loadTexture("./asset/numbers/95.png");
		number.setTextureID(txt);
	}
	if (gamescore == 96)
	{
		GLuint txt = loadTexture("./asset/numbers/96.png");
		number.setTextureID(txt);
	}
	if (gamescore == 97)
	{
		GLuint txt = loadTexture("./asset/numbers/97.png");
		number.setTextureID(txt);
	}
	if (gamescore == 98)
	{
		GLuint txt = loadTexture("./asset/numbers/98.png");
		number.setTextureID(txt);
	}
	if (gamescore == 99)
	{
		GLuint txt = loadTexture("./asset/numbers/99.png");
		number.setTextureID(txt);
	}
	if (gamescore == 100)
	{
		GLuint txt = loadTexture("./asset/numbers/100.png");
		number.setTextureID(txt);
	}
}





void keyCallback(GLFWwindow* w1, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_RIGHT)
	{

		if (action == GLFW_PRESS)

			playmovexright = true;


		if (action == GLFW_RELEASE)
			playmovexright = false;

	}

	if (key == GLFW_KEY_ENTER)
	{
		if (action == GLFW_PRESS)
			menubool = true;
	}

	if (key == GLFW_KEY_LEFT)
	{

		if (action == GLFW_PRESS)

			playmovexleft = true;


		if (action == GLFW_RELEASE)
			playmovexleft = false;


	}
	if (key == GLFW_KEY_UP)
	{

		if (action == GLFW_PRESS)

			playerJump = true;


		if (action == GLFW_RELEASE)
		{
			playerJump = false;
			playerBack = true;
		}
	}

	if (key == GLFW_KEY_SPACE)
	{

		if (action == GLFW_PRESS)
		{
			triggercount++;
			trigger = true;
			
		}

		if (action == GLFW_RELEASE)
		{
			trigger = false;
			
		}

	}
	if (key == GLFW_KEY_D)
	{

		if (action == GLFW_PRESS)
		{
			triggercount1++;
			rightBullet = true;
		}

		if (action == GLFW_RELEASE)
			rightBullet = false;

	}
}


void render() {

	menu.draw();
	if (menubool)
	{
		background.draw();
		background2.draw();

		mainplayer.draw();



		
		for (int i = 0; i < triggercount; i++)
		{
			if (!stop)
			{

				triggershape[i].draw();
			}
		}
		for (int i = 0; i < triggercount1; i++)
		{
			if (!collid1[i]) {
				std::cout << "bullet";
				triggershape1[i].draw();
			}
		}

		
		for (int z = 0; z < constenemycount; z++)
		{
			triggerencount = 1;
			while (triggerencount % 2 != 0)
			{
				triggerenemyshape[z][triggerencount].draw();
				triggerencount++;
			}

		}


		number.draw();

		
		for (int i = 0; i < NUM_OF_CONSTENEMY; i++)
			if (!collid[i]) {
				constenemy[i].draw();

			}

		for (int i = 0; i < NUM_OF_PLATFORMS; i++)
			platforms[i].draw();

		for (int i = 0; i < MAX_CAVE; i++)
		{
			cave[i].draw();
		}
		for (int i = 0; i < MAX_CACTUS; i++)
		{
			if (!collid1[i]) {
				cactus[i].draw();
			}
		}
	}
}



int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "IMPACT!", NULL, NULL);
	
	glfwSetKeyCallback(window, keyCallback);
	glfwMakeContextCurrent(window);

	
	Menu();
	
		createmainbackground();

		createConstEnemy();
		createNumber();
	

		createmainPlayer();

		createPlatform();
		createBarrel();

		createBox();
		
	do
	{
		

		do {

			

			if(menubool)
				if (bulletmovementx)
					update1();
				update();

			
			glClearColor(0.1, 0.1, 0.1, 0);
			glClear(GL_COLOR_BUFFER_BIT);


			render();

			if (gameend)
			{

				gameoverscreen();
				break;

			}
			glfwSwapBuffers(window);
			glfwPollEvents();

		} while (!gameend);

		gameover.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (!glfwWindowShouldClose(window));
	return 0;
}



