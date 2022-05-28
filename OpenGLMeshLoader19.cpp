#include <math.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include<stdlib.h>

#include <cmath>
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"


#include <string.h>
#include <iostream>
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)



class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	Vector3f add(float m,float n, float o) {
		return Vector3f(x + m, y + n, z + o);
	}

	Vector3f multi(float m, float n, float o) {
		return Vector3f(x * m, y * n, z * o);
	}


	float returnX() {
		return x;
	}
	float returnZ() {
		return z;
	}

};

bool underattack = false;
bool alreadyUnderAttack = false;
bool firstP = false;
bool neverRotated = true;
double playerx = 22;
double playerz = 0;
int playerdirection = 1;
int cameraOrientation = 1;

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 35.0f, float eyeY = 4, float eyeZ = 0.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

	void setView(float x, float y, float z) {

		eye = Vector3f(x, y, z);
	}
	void flipView(int state) {
		if (!firstP) {
			switch (state) {
			case 1:
				eye = eye.add(-14, 0, 0);
				break;
			case 2:
				eye = eye.add(0, 0, -14);
				break;
			case 3:
				eye = eye.add(14, 0, 0);
				break;
			case 4:
				eye = eye.add(0, 0, 14);
			default:
				break;
			}
			firstP = true;
			if (!neverRotated) {
				rotateY(180);

				neverRotated = !neverRotated;
			}
		}
		else {
			switch (state) {
			case 1:
				eye = eye.add(14, 0, 0);
				firstP = true;
				break;
			case 2:
				eye = eye.add(0, 0, 14);
				break;
			case 3:
				eye = eye.add(-14, 0, 0);
				break;
			case 4:
				eye = eye.add(0, 0, -14);
			default:
				break;
			}
			firstP = false;
			neverRotated = true;

		}
		
	}

	void cameraRotate(int p, int n) {
		float changex = eye.returnX() - playerx;
		float changez = eye.returnZ() - playerz;
		switch (p) {
		//prev camera orientation 1
		case 1:
			switch (n) {
				//wanted camera orientation 1
				case 1:
					break;
				case 2:
					
					moveZ(changex);
					//moveY(0.1 * changex);
					moveX(changex);
					rotateY(-90);
					/*std::cout << eye.returnZ() << std::endl;
					std::cout << playerz << std::endl;*/
					break;
				case 3:
					moveZ(2*changex);
					//moveY(0.1 * changex);
					rotateY(180);
					break;
				case 4:
					moveZ(changex);
					//moveY(0.1 * changex);
					moveX(-changex);
					rotateY(90);
					break;
				default:
					break;
			}
			break;

		case 2:
			switch (n) {
				//wanted camera orientation 1
			case 1:
				moveZ(changez);
				//moveY(-0.1 * changez);
				moveX(-changez);
				rotateY(90);
				break;
			case 2:
				break;
			case 3:
				moveZ(changez);
				//moveY(0.1 * changez);
				moveX(changez);
				rotateY(-90);
				break;
			case 4:
				moveZ(2 * changez);
				//moveY(0.1 * changez);
				rotateY(180);
				break;
			default:
				break;
			}
			break;

		case 3:
			switch (n) {
				//wanted camera orientation 1
			case 1:
				moveZ(2 * -changex);
				//moveY(0.1 * -changex);
				rotateY(180);
				break;
			case 2:
				moveZ(-changex);
				//moveY(0.1 * -changex);
				moveX(changex);
				rotateY(90);
				break;
			case 3:
				break;
			case 4:
				moveZ(-changex);
				//moveY(0.1 * -changex);
				moveX(-changex);
				rotateY(-90);
				break;
			default:
				break;
			}
			break;

		case 4:
			switch (n) {
				//wanted camera orientation 1
			case 1:
				moveZ(-changez);
				//moveY(0.1 * changez);
				moveX(-changez);
				rotateY(-90);
				break;
			case 2:
				moveZ(2 * -changez);
				//moveY(0.1 * changez);
				rotateY(180);
				break;
			case 3:
				moveZ(-changez);
				//moveY(-0.1 * changez);
				moveX(changez);
				rotateY(90);
				break;
			case 4:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
};


Camera camera;

float lightypos = 0;
float lightzpos = 50;
float lightr = 0.1;
float lightg = 0.1;
float lightb = 0.1;
bool sunrise = true;

void setupLights() {
	glEnable(GL_LIGHTING);

	GLfloat ambient[] = { lightr, lightg,lightb, 1.0f };
	GLfloat diffuse[] = { lightr, lightg, lightb, 1.0f };
	GLfloat specular[] = { lightr, lightg, lightb, 1.0f };
	GLfloat shininess[] = { 100 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { lightr, lightg, lightb, 1 };
	GLfloat lightdirec[] = { 0,-1,0 };
	GLfloat lightPosition[] = { lightzpos, lightypos,lightzpos, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdirec);
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular2[] = { lightr, lightg, lightb, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular2);

	// Set Material's Shine value (0->128)
	GLfloat shininess2[] = { lightr*100+lightg*100+lightb*100 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess2);
}


void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


//general
int timer = 300;
bool won = false;
bool lost = false;
bool gameover = false;
bool part1 = true;
bool carryingWood = false;

int woodCollected = 0;
double hunger = 0;
double health = 100;
int score = 0;

bool hit = false;
bool playOnce = true;
bool hitFruit = false;
bool hitTree = false;
bool nearWood = false;

int fruitx = 12;
int fruitz = 12;

GLuint tex;
GLuint sky_night;
GLuint sun_tex;
GLuint moon_tex;

float playerRotation = 0;
Model_3DS model_player;
Model_3DS model_apple;
Model_3DS model_trex;
Model_3DS model_trunk;
Model_3DS model_tree;
Model_3DS model_rock;
Model_3DS model_wood;
Model_3DS model_space;

GLTexture tex_ground;
GLTexture tex_sea;
GLTexture tex_sea_night;
GLTexture tex_ground_night;




void healthBar() {
	glDisable(GL_LIGHTING);
	glColor3d(0, 3, 0);
	glBegin(GL_QUADS);
	glVertex2d(0.1, 0.1);
	glVertex2d(health/500+0.1, 0.1);
	glVertex2d(health/500+0.1, 0.15);
	glVertex2d(0.1, 0.15);
	glEnd();
	
	
	glColor3d(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(0.09, 0.09);
	glVertex2d(0.31, 0.09);
	glVertex2d(0.31, 0.16);
	glVertex2d(0.09, 0.16);
	glEnd();
	glEnable(GL_LIGHTING);
}

void hungerBar() {
	glDisable(GL_LIGHTING);
	glColor3d(3, 0, 0.1);
	glBegin(GL_QUADS);
	glVertex2d(0.1, 0.2);
	glVertex2d(0.1 + hunger/500, 0.2);
	glVertex2d(0.1 + hunger/500, 0.25);
	glVertex2d(0.1, 0.25);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(0.09, 0.19);
	glVertex2d(0.31, 0.19);
	glVertex2d(0.31, 0.26);
	glVertex2d(0.09, 0.26);
	glEnd();
	glEnable(GL_LIGHTING);
}

void TimerOfHunger(int val) {
	if (hunger < 100) {
		hunger += 1.5;
		if (hunger > 100) {
			hunger = 100;
		}
		if (carryingWood) {
			hunger += 1.5;
		}
	}
	else {
		if (health > 0)
			health -= 1.5;
			score -= 5;
		if (carryingWood) {
			health -= 1.5;
			score -= 5;
		}
		if (health < 0) {
			health = 0;
		}
	}

	if (part1) {
		glutTimerFunc(1000, TimerOfHunger, 0);
	}

}

double loading = 0.66;
void creatingShipLoadingBar() {
	glDisable(GL_LIGHTING);
	glColor3d(0, 2, 0);
	glBegin(GL_QUADS);
	glVertex2d(0.65, 0.5);
	glVertex2d(loading, 0.5);
	glVertex2d(loading, 0.6);
	glVertex2d(0.65, 0.6);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(0.64, 0.49);
	glVertex2d(1.26, 0.49);
	glVertex2d(1.26, 0.61);
	glVertex2d(0.64, 0.61);
	glEnd();
	glEnable(GL_LIGHTING);
}


int NoOfBoards = 0;
void boardEnd() {
	glPushMatrix();
	glScaled(1, 1, 0.1);
	glutSolidSphere(0.4, 10, 10);
	glPopMatrix();
}

void boardEnd2() {
	glPushMatrix();
	glScaled(0.1, 1, 1);
	glutSolidSphere(0.4, 10, 10);
	glPopMatrix();
}
void shipBoard() {

	glColor3d(1, 1, 0.2);
	for (int i = 0; i < NoOfBoards; i++) {
		if (i == 9) {
			//glPushMatrix();
			//glTranslated(-20.5, 0.4, -1.9);
			//glRotated(90, 0, 1, 0);
			//GLUquadricObj* qobj;
			//qobj = gluNewQuadric();
			//gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
			//gluCylinder(qobj, 0.4, 0.4, 6.1, 100, 100);
			//glPopMatrix();

			//glPushMatrix();
			//glTranslated(-20.5, 0.4, -1.9);
			//boardEnd2();
			//glPopMatrix();

			//glPushMatrix();
			//glTranslated(-14.4, 0.4, -1.9);
			//boardEnd2();
			//glPopMatrix();

			glPushMatrix();

			if (won) {
				/*glTranslated(playerEscapex + 18, 0, playerEscapez + 5);*/
			}
			glColor3f(5, 5, 5);
			glTranslated(-18, 0.4, -5);


			glRotated(90, 0, 1, 0);
			glScaled(0.3, 0.2, 0.3);

			model_wood.Draw();
			glPopMatrix();

		}
		else {
			if (i == 8) {
				//glPushMatrix();
				//glTranslated(-20.5, 0.4, -5.2);
				//glRotated(90, 0, 1, 0);
				//GLUquadricObj* qobj;
				//qobj = gluNewQuadric();
				//gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
				//gluCylinder(qobj, 0.4, 0.4, 6.1, 100, 100);
				//glPopMatrix();

				//glPushMatrix();
				//glTranslated(-20.5, 0.4, -5.2);
				//boardEnd2();
				//glPopMatrix();

				//glPushMatrix();
				//glTranslated(-14.4, 0.4, -5.2);
				//boardEnd2();
				//glPopMatrix();

				glPushMatrix();


				if (won) {
					/*glTranslated(playerEscapex + 18, 0, playerEscapez + 5);*/
				}
				glColor3f(5, 5, 5);
				glTranslated(-18, 0.4, -7.4);


				glRotated(90, 0, 1, 0);
				glScaled(0.3, 0.2, 0.3);

				model_wood.Draw();
				glPopMatrix();

			}
			else {
				//glPushMatrix();
				//glTranslated(-20 + (i * 0.7), 0.4, -6);
				//GLUquadricObj* qobj;
				//qobj = gluNewQuadric();
				//gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
				//gluCylinder(qobj, 0.4, 0.4, 5, 100, 100);
				//glPopMatrix();

				//glPushMatrix();
				//glTranslated(-20 + (i * 0.7), 0.4, -1);
				//boardEnd();
				//glPopMatrix();

				//glPushMatrix();
				//glTranslated(-20 + (i * 0.7), 0.4, -6);
				//boardEnd();
				//glPopMatrix();

				glPushMatrix();

				if (won) {
					/*glTranslated(playerEscapex + 17, 0, playerEscapez + 5);*/
				}

				glColor3f(5, 5, 5);
				glTranslated(-20 + (i * 0.7), 0.4, -6);
				glScaled(0.3, 0.2, 0.2);

				model_wood.Draw();
				glPopMatrix();


			}
		}
	}
}

void print(double x, double y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void printscore() {
	glDisable(GL_LIGHTING);
	glColor3d(0.5, 3, 0.5);
	char* p0s[20];
	sprintf((char*)p0s, "Score: %d", score);
	print(1.7, 0.15, (char*)p0s);
	glEnable(GL_LIGHTING);
}

void printGameOver() {
	glDisable(GL_LIGHTING);
	glColor3d(3, 0, 0);
	char* p0s[20];
	sprintf((char*)p0s, "GAME OVER");
	print(0.9, 0.6, (char*)p0s);
	glEnable(GL_LIGHTING);

}

void printGameWon() {
	glDisable(GL_LIGHTING);
	glColor3d(2.22, 0.49, 0.99);
	char* p0s[20];
	sprintf((char*)p0s, "YOU WON");
	print(0.9, 0.9, (char*)p0s);
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glColor3d(0.5, 3, 0.5);
	char* p0s2[20];
	sprintf((char*)p0s2, "Score: %d", score);
	print(0.9, 1, (char*)p0s2);
	glEnable(GL_LIGHTING);

}

void printWoodCollected() {
	glDisable(GL_LIGHTING);
	glColor3d(3, 3, 0.3);
	char* p0s[20];
	if (part1) {
		sprintf((char*)p0s, "Wood: %d", woodCollected);
	}
	else {
		sprintf((char*)p0s, "Progress: %d", 10 * (10 - woodCollected));
	}
	print(1.7, 0.25, (char*)p0s);
	glEnable(GL_LIGHTING);
}

void TimerOfTime(int val) {
	timer -= 1;

	if (timer == 0)
		gameover = true;

	glutTimerFunc(1000, TimerOfTime, 0);


}

void printTimer() {
	glDisable(GL_LIGHTING);
	glColor3d(3, 0.5, 0.5);
	char* p0s[20];
	sprintf((char*)p0s, "Timer: %d", timer);
	print(1.7, 0.2, (char*)p0s);
	glEnable(GL_LIGHTING);
}

void screen() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 2.0, 2.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if (!won) {
		healthBar();
		printscore();
		printTimer();
		printWoodCollected();
		if (part1) {

			hungerBar();
		}

		else {
			if (nearWood)
				creatingShipLoadingBar();
		}
		if (gameover) {
			printGameOver();
		}
	}
	else {
		printGameWon();
	}
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


}

//ground
void drawGround() {
	glEnable(GL_LIGHTING);

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	if (part1) {
		glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex_ground_night.texture[0]);
	}
		// Bind the ground texture
	

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-25, 0, -25);
	glTexCoord2f(5, 0);
	glVertex3f(25, 0, -25);
	glTexCoord2f(5,5);
	glVertex3f(25, 0, 25);
	glTexCoord2f(0, 5);
	glVertex3f(-25, 0, 25);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//sea
	//glDisable(GL_LIGHTING);	// Disable lighting 
	if (part1) {
		glColor3f(5, 5, 5);
	}
		// Dim the ground texture a bit
	else {
		glColor3f(1, 1, 1);
	}

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	if (part1) {
		glBindTexture(GL_TEXTURE_2D, tex_sea.texture[0]);	// Bind the ground texture
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex_sea_night.texture[0]);	// Bind the ground texture
	}
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-50, -0.25, -50);
	glTexCoord2f(5, 0);
	glVertex3f(50, -0.25, -50);
	glTexCoord2f(5, 5);
	glVertex3f(50, -0.25, 50);
	glTexCoord2f(0, 5);
	glVertex3f(-50, -0.25, 50);
	glEnd();
	glPopMatrix();

	//glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//sky box
	glPushMatrix();

	glColor3f(5, 5, 5);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(0, 0, 0);
	glRotated(90, 1, 0, 1);
	if (part1) {
		glBindTexture(GL_TEXTURE_2D, tex);
	}
	else {
		glColor3f(4, 4, 4);
		glBindTexture(GL_TEXTURE_2D, sky_night);
	}
	
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	

	glPopMatrix();

	//sun and moon
	glPushMatrix();

	glColor3f(5, 5, 5);
	GLUquadricObj* qobj2;
	qobj2 = gluNewQuadric();
	glTranslated(lightzpos, lightypos, lightzpos);
	glRotated(90, 1, 0, 1);
	glColor3f(4, 4, 4);
	if (part1) {
		glBindTexture(GL_TEXTURE_2D, sun_tex);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, moon_tex);
	}
	
	gluQuadricTexture(qobj2, true);
	gluQuadricNormals(qobj2, GL_SMOOTH);
	gluSphere(qobj2, 1, 30, 30);
	gluDeleteQuadric(qobj2);


	glPopMatrix();

}


//trees
double  treesize[15] = { 0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4};
double treex[15] = { 0,5,-5,10,-10,15,-15,20,-20,  1, -8,  8,-13,13,19};
double treez[15] = { 2,7,-9,1 ,-19,10, 21,20,-14,-10, 11,-14, 18,-7,12 };
int treeHits[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int treeHealth[15] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void drawTree(int x, int y, int z, double size,int health) {
	glEnable(GL_LIGHTING);
	if (part1) {
		if (health == 1) {

			//cone1
			glPushMatrix();
			glColor3d(0.9, 0.2, 0.2);

			glTranslated(x, y * size, z);


			glScaled(size * 0.0035, size * 0.0035, size * 0.0035);
			glColor3f(1, 1, 0);
			model_tree.Draw();
			glPopMatrix();



		}
		else {
			//stem

			glPushMatrix();



			glTranslated(x, 0, z); // cylinder at (0,0,1)  
			glScaled(0.01, 0.01, 0.01);
			glColor3f(1, 1, 0);
			model_trunk.Draw();
			glColor3d(1, 1, 1);
			glPopMatrix();
		}
	}
	else {
		glPushMatrix();



		glTranslated(x, 0, z); // cylinder at (0,0,1)  
		glScaled(0.05, 0.05, 0.05);
		glColor3f(1, 1, 0);
		model_rock.Draw();
		glColor3d(1, 1, 0);
		glPopMatrix();
	}
}
void drawTrees() {

	for (int i = 0; i < 15; i++) {
		drawTree(treex[i], 20, treez[i], treesize[i], treeHealth[i]);
	}
}

//draw animal
double animalX = 0;
double animalZ = 0;
double animalRotation = 0;
void drawAnimal(){

	//dino
	glPushMatrix();
	glTranslated(animalX, 4, animalZ);
	glScaled(0.25, 0.25, 0.25);
	glRotated(animalRotation, 0, 1, 0);
	glColor3f(1, 1, 1);
	model_trex.Draw();
	
	glPopMatrix();

}

//fruit

//randomize place for fruit
void randomFruitLoc() {
	srand(time(0));

	fruitx = rand() % 20;
	fruitz = rand() % 20;

	int posx = rand() % 2;
	int posz = rand() % 2;

	if (posx > 0) {
		fruitx = -fruitx;
	}
	if (posz > 0) {
		fruitz = -fruitz;
	}
}

void drawFruit(double x, double y, double z) {

	//apple
	glPushMatrix();
	glTranslated(x, y, z);
	glRotatef(90.f, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	glColor3f(1, 1, 0);
	model_apple.Draw();
	glColor3d(1, 1, 1);
	glPopMatrix();

}

//player
float playery = 0;
double fallingRot = 0;
void drawPlayer() {
	glPushMatrix();
	

	//wood

	if (carryingWood) {
		glPushMatrix();
		
		

		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		if (cameraOrientation == 1 || cameraOrientation == 3) {
			glTranslated(playerx, 2, playerz + 0.8);
		}
		else {
			glTranslated(playerx + 0.8, 2, playerz);
		}
		glRotated(playerRotation + 90, 0, 1, 0);
		glScaled(0.2, 0.2,0.2);
		model_wood.Draw();
		glPopMatrix();
	}

	//player
	glPushMatrix();
	if (won) {
		/*glTranslated(playerEscapex-playerx, 0, playerEscapez-playerz);*/
	}
	glColor3f(1, 1, 1);
	glTranslated(playerx, playery, playerz);
	glRotated(playerRotation, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(fallingRot, 1, 0, 0);
	glScalef(0.035, 0.035, 0.035);
	model_player.Draw();
	glPopMatrix();

	
	glPopMatrix();

}



//wood Collector
void drawWoodCollected() {
	glPushMatrix();



	glColor3f(1, 1, 1);
	glTranslated(-24, 0, 0);

	glScaled(0.4, 0.4, 0.5);

	model_wood.Draw();
	glPopMatrix();

	for (double i = 0; i < woodCollected; i++) {
		double f = 1;
		int m = (int)i;
		if ( m % 2 == 0) {
			f = -f;
		}

		glPushMatrix();
		



		glColor3f(5, 5, 5);
		glEnable(GL_TEXTURE_2D);

		
		glTranslated(-24, (i*4/ 10) + 0.8, f);
		glScaled(0.2, 0.2, 0.2);
		model_wood.Draw();
		
		glPopMatrix();
	}
}
float motion = 0;
void drawSpace() {
	glPushMatrix();
	glTranslated(playerx, 17, playerz + motion);
	glScaled(5, 5, 5);
	model_space.Draw();
	glPopMatrix();
}

bool stopOnce = false;
void Display() {
	setupCamera();
	setupLights();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	screen();
	if (won) {
		drawSpace();
		
	}

	drawWoodCollected();
	drawGround();
	drawTrees();
	
	if (part1) {
		
		drawFruit(fruitx, 0, fruitz);
		if (hitFruit)
		{
			hitFruit = false;
			PlaySound(NULL, NULL, SND_ASYNC);

			PlaySound(TEXT("sounds/Twinkle.wav"), NULL, SND_ASYNC);
		}

		if (hitTree)
		{
			hitTree = false;
			PlaySound(NULL, NULL, SND_ASYNC);
			PlaySound(TEXT("sounds/ChoppingWood.wav"), NULL, SND_ASYNC);
		}
	}
	else {
		drawAnimal();

		//Alaa
		if (playerx <= -22 && playerx >= -24 && playerz >= -5 && playerz <= 5 && woodCollected > 0)
			nearWood = true;
		else
			nearWood = false;

		shipBoard();
		if (!won) {
			if (woodCollected == 0 && health > 0) {
				won = true;
				score += 10 * timer;
				camera.setView(30, 10, 40);
				lightr = 0.85;
				lightg = 0.35;
				lightb = 0.6;
				lightypos = 25;
				lightzpos = 0;
				PlaySound(NULL, NULL, SND_SYNC);
				PlaySound(TEXT("sounds/won.wav"), NULL, SND_ASYNC);
			}
		}
	}

	double angle1 = atan2(animalZ - animalZ +1, animalX - animalX);
	double angle2 = atan2(animalZ - playerz, animalX - playerx);
	animalRotation = (angle1 - angle2) * 180 / 3.14 + 180;
	

	
	if (playerx >= animalX - 2.2 && playerx <= animalX + 2.2 && playerz >= animalZ - 2.2 && playerz <= animalZ + 2.2 && hit == false) {
		hit = true;
	}
	else {
		hit = false;
	}

	for (int i = 0; i < 15; i++) {
		float range = 0.5;
		if (!part1) {
			range = 2;
		}
		if (playerx >= treex[i] - range && playerx <= treex[i] + range && playerz <= treez[i] + range && playerz >= treez[i] - range)
		{
			if (!won) {
				PlaySound(NULL, NULL, SND_ASYNC);
				PlaySound(TEXT("sounds/ouch.wav"), NULL, SND_ASYNC);
			}
			if (!won && !part1) {
				playery = 2;
			}

		}
	}
	if (playery < 15) {
		drawPlayer();
	}
	if (!won) {
		playery = 0;
	}
	if (nearWood && playOnce) {
		playOnce = false;
		if(!won)
		PlaySound(NULL, NULL, SND_SYNC);
		PlaySound(TEXT("sounds/ship making.wav"), NULL, SND_ASYNC);

	}
	if (!nearWood || loading == 0.66) {
		if (!playOnce&&!won)
			PlaySound(NULL, NULL, SND_ASYNC);
		playOnce = true;

	}
	if (health <= 0) {
		gameover = true;
	}
	if (gameover) {
		if (!stopOnce) {
			if (!won) 
			PlaySound(NULL, NULL, SND_SYNC);
			PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
			stopOnce = true;
		}



	}
	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 1;
	if (!gameover && !won) {
		switch (key) {
		case 'z':
			camera.moveY(d);
			break;
		case 'x':
			camera.moveY(-d);
			break;
		case 'c':
			camera.moveX(d);
			break;
		case 'v':
			camera.moveX(-d);
			break;
		case 'b':
			camera.moveZ(d);
			break;
		case 'n':
			camera.moveZ(-d);
			break;

		case 'j':
			camera.setView(5, 70, 0.1);
			break;
		case 'k':
			camera.setView(70, 0.1, 0.1);
			break;
		case 'l':
			camera.setView(0.1, 0.1, 70);
			break;

		case 'w':
			playerRotation = 0;
			if (cameraOrientation != 1) {
				neverRotated = false;
			}
			camera.cameraRotate(cameraOrientation, 1);
			cameraOrientation = 1;
			if (playerx > -25) {
				playerx -= 0.2;
				camera.moveZ(0.2);
			}
			else {
				if (health > 0) {
					health -= 10;
					score -= 50;
					playerx = -20;
					if (!won) {
						PlaySound(NULL, NULL, SND_SYNC);
						PlaySound(TEXT("sounds/drown.wav"), NULL, SND_ASYNC);
					}
					camera.moveZ(-5);
					if (health < 0) {
						health = 0;
					}
				}
			}


			break;

		case 'a':
			playerRotation = 90;
			if (cameraOrientation != 4) {
				neverRotated = false;
			}
			camera.cameraRotate(cameraOrientation, 4);
			cameraOrientation = 4;
			if (playerz < 25) {
				playerz += 0.2;
				camera.moveZ(0.2);
			}
			else {
				if (health > 0) {
					health -= 10;
					score -= 50;
					playerz = 20;
					camera.moveZ(-5);
					PlaySound(NULL, NULL, SND_ASYNC);
					PlaySound(TEXT("sounds/drown.wav"), NULL, SND_ASYNC);

					if (health < 0) {
						health = 0;
					}
				}

			}

			break;
		case 'd':
			playerRotation = -90;
			if (cameraOrientation != 2) {
				neverRotated = false;
			}
			camera.cameraRotate(cameraOrientation, 2);
			cameraOrientation = 2;

			if (playerz > -25) {
				playerz -= 0.2;
				camera.moveZ(0.2);
			}
			else {
				if (health > 0) {
					health -= 10;
					score -= 50;
					playerz = -20;
					PlaySound(NULL, NULL, SND_ASYNC);
					PlaySound(TEXT("sounds/drown.wav"), NULL, SND_ASYNC);

					camera.moveZ(-5);
					if (health < 0) {
						health = 0;
					}
				}
			}

			break;

		case 's':
			playerRotation = 180;
			if (cameraOrientation != 3) {
				neverRotated = false;
			}
			camera.cameraRotate(cameraOrientation, 3);
			cameraOrientation = 3;
			if (playerx < 25) {
				playerx += 0.2;
				camera.moveZ(0.2);
			}
			else {
				if (health > 0) {
					health -= 10;
					score -= 50;
					playerx = 20;
					PlaySound(NULL, NULL, SND_ASYNC);
					PlaySound(TEXT("sounds/drown.wav"), NULL, SND_ASYNC);

					camera.moveZ(-5);
					if (health < 0) {
						health = 0;
					}
				}
			}

			break;

		case '1':
			PlaySound(NULL, NULL, SND_ASYNC);
			PlaySound(TEXT("sounds/roar.wav"), NULL, SND_ASYNC);
			part1 = false;
			score += 10 * timer;
			woodCollected = 10;
			timer = 250;
			lightr = 0.01;
			lightg = 0.01;
			lightb = 0.01;
			lightypos = 0;
			lightzpos = 50;
			sunrise = true;
			for (int i = 0; i < 15; i++) {
				treeHealth[i] = 0;
			}
			break;
		case '2':
			camera.flipView(cameraOrientation);
			break;
		case '3':
			won = true;
			score += 10 * timer;

			camera.setView(25, 10, 40);
			lightr = 1.275;
			lightg = 0.525;
			lightb = 0.9;
			lightypos = 25;
			lightzpos = 0;
			PlaySound(NULL, NULL, SND_ASYNC);
			PlaySound(TEXT("sounds/won.wav"), NULL, SND_ASYNC);
			break;
		case GLUT_KEY_ESCAPE:
			exit(EXIT_SUCCESS);
		}
		glutPostRedisplay();
	}
	
}
void Special(int key, int x, int y) {
	float a = 2.0;

	if (!gameover) {
		switch (key) {
		case GLUT_KEY_UP:
			camera.rotateX(a);
			break;
		case GLUT_KEY_DOWN:
			camera.rotateX(-a);
			break;
		case GLUT_KEY_LEFT:
			camera.rotateY(a);
			break;
		case GLUT_KEY_RIGHT:
			camera.rotateY(-a);
			break;
		}

		glutPostRedisplay();
	}
}


void TimerOfAttack(int val) {
	alreadyUnderAttack = true;
	if (playerx >= animalX - 2.2 && playerx <= animalX + 2.2 && playerz >= animalZ - 2.2 && playerz <= animalZ + 2.2 &&!won &&!gameover) {
		if (health > 0) {
			health -= 10;
			PlaySound(NULL, NULL, SND_SYNC);
			PlaySound(TEXT("sounds/eat.wav"), NULL, SND_ASYNC);

		}
		else {
			health = 0;
		}
		glutTimerFunc(1500, TimerOfAttack, 0);
	}
	else {
		underattack = false;
		alreadyUnderAttack = false;
	}

}

void Anim() {
	
	if (gameover && fallingRot < 80) {
		fallingRot += 1;
	}
	if (underattack && !alreadyUnderAttack) {
		glutTimerFunc(0, TimerOfAttack, 0);
	}
	if (part1) {
		//move light
		if (sunrise) {
			lightypos += 0.022;
			lightzpos -= 0.022;
			lightr += 0.0012;
			lightg += 0.0012;
			lightb += 0.00012;
			if (lightypos >= 50) {
				sunrise = false;
			}
		}
		else {
			lightypos -= 0.022;
			lightzpos -= 0.022;

			lightr -= 0.0012;
			lightg -= 0.0012;
			lightb -= 0.00012;
		}
		//hit fruit?
		if (playerx > fruitx - 2 && playerx < fruitx + 2) {
			if (playerz > fruitz - 2 && playerz < fruitz + 2) {
				hunger = 0;
				hitFruit = true;
				score += 100;
				randomFruitLoc();
			}
		}

		//unload wood
		if (carryingWood) {
			if (playerx < -20) {
				if (playerz > -5 && playerz < 5) {
					woodCollected += 1;
					carryingWood = false;
					score += 50;
					if (woodCollected == 10) {
						part1 = false;
						score += 10 * timer;
						timer = 250;
						lightr = 0.01;
						lightg = 0.01;
						lightb = 0.01;
						lightypos = 0;
						lightzpos = 50;
						sunrise = true;
						for (int i = 0; i < 15; i++) {
							treeHealth[i] = 0;
						}
						PlaySound(NULL, NULL, SND_ASYNC);
						PlaySound(TEXT("sounds/roar.wav"), NULL, SND_ASYNC);
					}
				}
			}
		}
	}
	else {
		if (sunrise) {
			lightypos += 0.0008;
			lightzpos -= 0.0008;
			lightr += 0.000004;
			lightg += 0.000004;
			lightb += 0.000004;
			if (lightypos >= 50) {
				sunrise = false;
			}
		}
		else {
			lightypos -= 0.0008;
			lightzpos += 0.0008;
			lightr -= 0.00004;
			lightg -= 0.00004;
			lightb -= 0.00004;
		}
		if (playery < 5) {
			float diffx = playerx - animalX;
			float diffz = playerz - animalZ;
			if (diffx > 0.75) {
				animalX += 0.0025;
			}
			else {
				if (diffx < 0.75)
					animalX -= 0.0025;
			}
			if (diffz > 0.75) {
				animalZ += 0.0025;
			}
			else {
				if (diffz < 0.75)
					animalZ -= 0.0025;
			}
		}
		
		if (nearWood && woodCollected > 0) {
			if (loading < 1.25)
				loading += 0.0005; 
			else {
				score += 50;
				NoOfBoards += 1;
				loading = 0.66;
				woodCollected -= 1;
			}
		}
		else {
			loading = 0.66; 

		}
		if (playerx >= animalX - 2.2 && playerx <= animalX + 2.2 && playerz >= animalZ - 2.2 && playerz <= animalZ + 2.2) {
			if (underattack == false && alreadyUnderAttack == false) {
				underattack = true;
			}
			else {
				if(underattack == false ) {

				}
			}
		}
	}
	if (health <= 0) {
		gameover = true;
	}

	if (won && (playery <=15)) {
		playery += 0.0032;
	}
	if (playery >= 15) {
		motion += 1.2;
	}
	glutPostRedisplay();
}
int prevx = 0;
void Motion(int x, int y) {
	if (x > prevx) {
		camera.rotateY(-1);
	}
	else {
		camera.rotateY(1);
	}
	prevx = x;
}

void Mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:

		if (state == GLUT_UP)
		{
			if (part1) {
				if (!carryingWood) {
					for (int i = 0; i < 15; i++) {
						if (!carryingWood) {
							if (playerx > treex[i] - 3 && playerx < treex[i] + 3) {
								if (playerz > treez[i] - 3 && playerz < treez[i] + 3) {
									if (treeHealth[i] == 1) {
										treeHits[i] += 1;
										treesize[i] -= 0.1;
										score += 15;
										hitTree = true;
										if (hunger < 100) {
											hunger += 8;
											if (hunger > 100) {
												hunger = 100;
											}
										}
										else {
											if (health>0)
											health -= 8;
											score -= 5;
											if (health < 0) {
												health = 0;
											}
										}
										if (treeHits[i] == 3) {
											treeHits[i] = 0;
											treeHealth[i] = 0;
											carryingWood = true;
											score += 100;
										}
									}
								}
							}
						}

					}
				}
			}

		}
		break;
	}
}

void LoadAssets()
{
	model_apple.Load("Models/3ds Apple/Apple.3ds");
	model_trex.Load("Models/trex/body.3ds");
	model_trunk.Load("Models/trunk/Trunk.3ds");
	model_player.Load("Models/Male1_3ds/Male1.3ds");
	model_tree.Load("Models/palm/palmtree.3ds");
	model_rock.Load("models/rock/whitestone.3ds");
	model_wood.Load("models/wood/wood.3ds");
	model_space.Load("models/space/python.3ds");
	tex_ground.Load("Textures/ground.bmp");
	tex_ground_night.Load("Textures/groundNight.bmp");
	tex_sea.Load("Textures/water.bmp");
	tex_sea_night.Load("Textures/waterNight.bmp");
	loadBMP(&tex, "Textures/sky.bmp", true);
	loadBMP(&sky_night, "Textures/skynight.bmp", true);
	loadBMP(&sun_tex, "Textures/sun.bmp", true);
	loadBMP(&moon_tex, "Textures/moon.bmp", true);


}

void resetCam() {
	playerRotation = -90;
	if (cameraOrientation != 2) {
		neverRotated = false;
	}
	camera.cameraRotate(cameraOrientation, 2);
	cameraOrientation = 2;

	if (playerz > -25) {
		playerz -= 0.2;
		camera.moveZ(0.2);
	}
	else {
		if (health > 0) {
			health -= 10;
			score -= 10;
			playerz = -20;
			camera.moveZ(-5);
			if (health < 0) {
				health = 0;
			}
		}
	}

	playerRotation = 0;
	if (cameraOrientation != 1) {
		neverRotated = false;
	}
	camera.cameraRotate(cameraOrientation, 1);
	cameraOrientation = 1;
	if (playerx > -25) {
		playerx -= 0.2;
		camera.moveZ(0.2);
	}
	else {
		if (health > 0) {
			health -= 10;
			score -= 100;
			playerx = -20;
			camera.moveZ(-5);
			if (health < 0) {
				health = 0;
			}
		}
	}

}
void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(200, 50);

	glutCreateWindow("Escape the Island");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Anim);
	glutMotionFunc(Motion);
	glutMouseFunc(Mouse);
	LoadAssets();
	randomFruitLoc();
	glutTimerFunc(5000, TimerOfHunger, 0);
	glutTimerFunc(0, TimerOfTime, 0);

	PlaySound(TEXT("sounds/start.wav"), NULL, SND_ASYNC);

	resetCam();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}

