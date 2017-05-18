
#include "../GL/glut.h"
#include <cmath>
#define PI 3.1415926535898
extern float x,y,z,angle_C;
extern int xtest,ytest,ztest;

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.9, 0., 0., 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
    GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat no_shininess[] = { 0.0 };
    GLfloat low_shininess[] = { 20.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

void material(void)
{
	//??
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);
	glShadeModel(GL_SMOOTH);
}

	void myinit_light(void)
{

	GLfloat light1_ambient[]={ 0.1f, 0.1f, 0.1f, 1.0f }; 
	GLfloat light1_diffuse[]={ 0.7f, .7f, .7f, 1.0f };
	GLfloat light1_specular[]={0.9,0.9,0.9,1.};
	GLfloat light1_position[]={ -500, 2000, 1000, 1.0f };	
	GLfloat light1_spot_direction[]={1,0,0};
	GLfloat light2_position[]={ 0.0f, 20.0f, -7.0f, 1.0f };	

	glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
	glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);
	glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
//	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,15);
//	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0);
//	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1_spot_direction);
/*
	glLightfv(GL_LIGHT2,GL_AMBIENT,light1_ambient);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,light1_diffuse);
	glLightfv(GL_LIGHT2,GL_SPECULAR,light1_specular);
	glLightfv(GL_LIGHT2,GL_POSITION,light2_position);

	GLfloat light0_ambient[]={0,0,0,1};
	GLfloat light0_diffuse[]={1.,1.,1.,1};
	GLfloat light0_specular[]={1.,1.,1.,1};
	GLfloat light0_position[]={0,0,1000,1};

	glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);*/

  	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
//	glDisable(GL_LIGHT2);


}
	void myinit_light_t(void)
{

	GLfloat light1_ambient[]={ 0.1f, 0.1f, 0.1f, 1.0f }; 
	GLfloat light1_diffuse[]={ 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light1_specular[]={0.3,0.3,0.3,1.};
	GLfloat light1_position[]={ 0, 5000,0, 1.0f };	
//	GLfloat light1_spot_direction[]={1,0,0};
	GLfloat light2_ambient[]={ 0.3f, 0.3f, 0.3f, 1.0f }; 
	GLfloat light2_diffuse[]={ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light2_position[]={ x, (y+100)*sin(z),(y+100)*cos(z), 1.0f };	
	GLfloat light2_spot_direction[]={0,-sin(z),cos(z)};

	glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
	glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);
	glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
//	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,15);
//	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0);
//	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1_spot_direction);

	glLightfv(GL_LIGHT2,GL_AMBIENT,light2_ambient);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
//	glLightfv(GL_LIGHT2,GL_SPECULAR,light1_specular);
	glLightfv(GL_LIGHT2,GL_POSITION,light2_position);

	GLfloat light0_ambient[]={0,0,0,1};
	GLfloat light0_diffuse[]={1.,1.,1.,0.5};
	GLfloat light0_specular[]={1.,1.,1.,0.5};
	GLfloat light0_position[]={1,-1,1,1};

	glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

  	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
//	glEnable(GL_LIGHT2);


}