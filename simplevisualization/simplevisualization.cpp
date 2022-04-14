#include <iostream>
#include <string>
#include <vector>
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

using namespace std;

unsigned int texture;

float a;

void timer(int value);
void Reshape(int x, int y);
void GenTexture(string texname);
void TexPlane(float x, float y, float z, float sizex, float sizey, float r, float g, float b);
void Draw();

void timer(int value) {
	a += 1;
	if (a > 360) a = 0;
	Draw();
	glutTimerFunc(40, timer, 0);
}

void Reshape(int x, int y)
{
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)x / (float)y, 0.1f, 100.0f); // настраиваем трехмерную перспективу
	glMatrixMode(GL_MODELVIEW); // переходим в трехмерный режим
	glViewport(0, 0, x, y);
		
}

void GenTexture(string texname)
{
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	int width, height, nrChannels;
	unsigned char *data = stbi_load(texname.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		std::cout << "Successfully loaded texture " << texname.c_str() << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture " << texname.c_str() << std::endl;
	}
	stbi_image_free(data);

	
}


void TexPlane(float x, float y, float z, float sizex, float sizey, float r, float g, float b)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(x, y, z);
	glRotatef(a,0,1,0);
    
	float sz = sizex / 2;
	float szy = sizey / 2;
	
	glEnable(GL_TEXTURE_2D);
	
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);
	glVertex3f(-sz,  szy, 0);
	glTexCoord2f(0, 0);
	glVertex3f( sz,  szy, 0);
	glTexCoord2f(0, 1);
	glVertex3f( sz, -szy, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-sz, -szy, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Draw()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	TexPlane(0, 0, -2, 1, 1.3, 1, 1, 1);
	glutSwapBuffers();
	glFlush();
}

int main(int argc, char *argv[])
{
	printf("Preparing to render.\n");
	glutInit(&argc, argv);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("Lighting and Texturing");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Draw);
	glutTimerFunc(40,timer,0);
	
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST); // включаем тест глубины
	
	
	glViewport(0, 0, 400, 300);
	glOrtho(-100, 100, -100, 100, -100, 100);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat l1p[] = { 0.0f, 2.0f, -4.0f, 0.0f };
	GLfloat l2p[] = { 0.0, 0.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, l1p);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l2p);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GenTexture("test.jpg");

	glShadeModel(GL_SMOOTH);
	
	glutMainLoop();
	return 0;
}