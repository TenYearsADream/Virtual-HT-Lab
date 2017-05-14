
#include "../GL/glut.h"
//#include <GL\glaux.h>
//#pragma comment (lib,"../GL/glut32.lib")   //C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib
#include <cmath>
//#include <stdio.h>
#include <stdlib.h>
//#include<windows.h>
//#include<iostream>

////

//#include <string>
//#include <sstream>
extern float x,y;
#define PI 3.14159
void DrawList(int i);


#define	MAX_PARTICLES	1000		// ��������������
extern float wind_v;
int dm=2000;

int	sp;				// �����״
	 
float	slowdown=70;			// �������� ���������ƶ��Ŀ���.��ֵ����,�ƶ�Խ��

float	xspeed=0;				// X������ٶ�

float	yspeed;				// Y������ٶ�

float	zoom=0.0f;			// ��Z������	
 
GLuint	loop;				// ѭ������   Ԥ�ȶ������Ӳ�����Ļ�л�����

GLuint	col=0;				// ��ǰ����ɫ  �������Ӳ�ͬ����ɫ
//extern bool particle_end;

float ri_=350;
extern int mover_H_ai;
typedef struct						// �����������ݽṹ
{
	bool	active;					// �Ƿ񼤻�
	float	life;					// ��������
	float	fade;					// ˥���ٶ�	
	float	R;					// ��ɫֵ
	float	G;					// ��ɫֵ
	float	B;					// ��ɫֵ
	float	x;					// X λ��
	float	y;					// R λ��
	float	z;					// Theta λ��
	float	xi;					// X �����ٶ�
	float	yi;					// R ����
	float	zi;					// theta ����
	float	xg;					// X ������ٶ�
	float	yg;					// R ������ٶ�
	float	zg;	
	bool    fir;                // theta ������ٶ�
}

particles;						// �������ݽṹ


particles particle[MAX_PARTICLES];				// ����1000�����ӵ�����

 
/*static GLfloat colors[12][3]=				// �ʺ���ɫ  ��ÿһ����ɫ��0��11���Ǵ洢����,����,������.�������ɫ�������12��������ɫ�Ӻ�ɫ��������ɫ

{

	{0.0f,1.5f,0.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,0.0f},{1.0f,1.0f,0.0f},

	{1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{0.5f,0.0f,0.0f},

	{0.5f,0.0f,0.0f},{0.f,0.f,0.f},{0.5f,0.5f,0.5f},{0.5f,0.5f,0.5f}

};*/
float poly(int lop,float a,float b,float c,float d){
	 return particle[lop].x*particle[lop].x*particle[lop].x*a+particle[lop].x*particle[lop].x*b+particle[lop].x*c+d;
 }

float pick(float dir_x,float dir_y,float dir_z,float eye_x,float eye_y,float eye_z,float ball_x,float ball_y,float ball_z){//colision_detection
	float t,d,xc,yc,zc;
	t=(dir_x*(ball_x-eye_x)+dir_y*(ball_y-eye_y)+dir_z*(ball_z-eye_z))/(dir_x*dir_x+dir_y*dir_y+dir_z*dir_z);
	xc=dir_x*t+eye_x;
	yc=dir_y*t+eye_y;
	zc=dir_z*t+eye_z;
	d=sqrt((ball_x-xc)*(ball_x-xc)+(ball_y-yc)*(ball_y-yc)+(ball_z-zc)*(ball_z-zc));
	return(d);
}

void color(float rrr,int lop){
	 if((rrr>150)&&(rrr<250)&&(particle[loop].x<500)) {
		 particle[loop].R=poly(lop,7.238e-9,-2.229e-6,-0.0023,0.8);
		 particle[loop].G=poly(lop,1.8e-8,-2.62e-5,0.0072,1);
		 particle[loop].B=poly(lop,-1.515e-9,2.273e-6,2.424e-4,6.384e-16);}
	 else {/////////////////
		 particle[loop].R=0;
		 particle[loop].G=0;
		 particle[loop].B=1;}
 }


void initial(int loop){
	    particle[loop].active=true;					// ʹ���е�����Ϊ����״̬

		particle[loop].life=1.0f;					// ���е���������ֵΪ���

		particle[loop].fade=float(rand()%100)/1000.0f+0.005f;		// �������˥������    ��������ֵ����0~99�е�����һ��,Ȼ��ƽ��1000�����õ�һ����С�ĸ�����.������ǰѽ������0.003f��ʹfade�ٶ�ֵ��Ϊ0
		
		color(particle[loop].y,loop);
 		
        particle[loop].x=-1000;
		particle[loop].y=rand()%560-280;
		particle[loop].z=rand()%600-300;

		particle[loop].xi=((rand()%10-5.0f)+300)*wind_v;		// �������X�᷽���ٶ�150 + -25

		particle[loop].yi=(rand()%10-20.0f);		// �������Y�᷽���ٶ�+ -25

		particle[loop].zi=(rand()%10-5.0f);		// �������Z�᷽���ٶ�+ -5��

 
		particle[loop].xg=0;						// ����X�᷽����ٶ�Ϊ0

		particle[loop].yg=0;//-abs(particle[loop].ri)/particle[loop].ri*3;						//  ����Y�᷽����ٶ�Ϊ

		particle[loop].zg=0;//-abs(particle[loop].ai)/particle[loop].ai*0.3f;						//  ����Z�᷽����ٶ�Ϊ
		particle[loop].fir=1;
}

 void initialparticles(void){   //��ʼ��
//	if (TextureImage[0]=LoadBMP("Data/Particle.bmp"))	// ������������//////////////////////////////
 
//	glDisable(GL_DEPTH_TEST);						//��ֹ��Ȳ���////////////////////////////////////	
 
	for (loop=0;loop<MAX_PARTICLES;loop++)				//��ʼ�����е�����

	{
		initial(loop);

	}
 }
 /*
void zone(float up,float down,float ri,float xg,float ag){
	if (particle[loop].r>up-5)particle[loop].ri=-ri;
	if (particle[loop].r<down+5)particle[loop].ri=ri;
	if ((particle[loop].r>down+5)&&(particle[loop].r<up-5)){particle[loop].ri=0;particle[loop].xg=xg;particle[loop].ag=ag;}
}
void zone_d(float up,float down,float ri,float xg,float ag,float range){
	if (range>up){
		if ((particle[loop].r>up-5)&&(particle[loop].r<range))particle[loop].ri=-ri;
		if (particle[loop].r<down+5)particle[loop].ri=ri;
		if ((particle[loop].r>down+5)&&(particle[loop].r<up-5)){particle[loop].ri=0;particle[loop].xg=xg;particle[loop].ag=ag;}
	}
	else{
		if (particle[loop].r>up-5)particle[loop].ri=-ri;
		if ((particle[loop].r<down+5)&&(particle[loop].r>range))particle[loop].ri=ri;
		if ((particle[loop].r>down+5)&&(particle[loop].r<up-5)){particle[loop].ri=0;particle[loop].xg=xg;particle[loop].ag=ag;}
	}
}

*/

 void check(int loop){
	 
	 float d;
	 if(particle[loop].fir){dm=-1000;}
	 else dm=000;

	 d=pick(0,0,1,-100,200,-2650, particle[loop].x  , particle[loop].y , particle[loop].z);
	 if ((particle[loop].x>-130)&&(particle[loop].x<-100)){
	 if (d<200) {
		 if((particle[loop].y)>200){  particle[loop].yi=700; particle[loop].yg=-20;}
		 else {  particle[loop].yi=-700; particle[loop].yg=20;}

	 }
	 }
	 if ((particle[loop].x>-100)&&(particle[loop].x<-70)){
	 if (d<200) {
		 if((particle[loop].y)>200){  particle[loop].yi=-20; particle[loop].yg=1;}
		 else {  particle[loop].yi=20; particle[loop].yg=-0;}

	 }
	 }
	 if (particle[loop].x>-70){
	 if (particle[loop].yg) {
		 if((particle[loop].y)>200){  particle[loop].yi=rand()%10-5; particle[loop].yg=0;}
		 else  {  particle[loop].yi=rand()%10-5; particle[loop].yg=0;}

	 }
	 }
/*	 glBegin(GL_LINES);
	 
	glVertex3f(1000,200.,0);
	glVertex3f(0,200.,0);
	glVertex3f(0,200.,0);
	glVertex3f(0,200.,1000);


	 glEnd();*/




/*
	 if((particle[loop].x>-1000)&&(particle[loop].x<-140)){
		 zone(765,250,ri_,0,0);
	 }
	 if((particle[loop].x>-140)&&(particle[loop].x<0)){
		 zone(765,250,ri_,5,-3);
	 }

	 if((particle[loop].x>0)&&(particle[loop].x<360)){
		 zone_d(450,322,ri_,1,-3,470);
		 zone_d(765,500,ri_,0,0,470);
	 }
	 if((particle[loop].x>360)&&(particle[loop].x<600)){
		 particle[loop].xg=0;particle[loop].ag=0;particle[loop].rg=0;
	 }
	 if((particle[loop].x>600)&&(particle[loop].x<1140)){/////////////////////////////////////////////core
		 zone_d(-0.098*particle[loop].x+511,325,ri_,1*mover_H_ai,-3*mover_H_ai,500);
	 }
	 if((particle[loop].x>1140)&&(particle[loop].x<1600)){
		 zone_d(0.1739*particle[loop].x+201.7391,0.2826*particle[loop].x+2.8261,ri_,50*mover_H_ai,0,500);////////////////////////
	 }
	 if((particle[loop].x>1600)&&(particle[loop].x<1620)){
		 zone(480,455,ri_,-1*mover_H_ai,1*mover_H_ai);
	 }
	 if((particle[loop].x>1620)&&(particle[loop].x<1670)){
		 zone(480,430,ri_,0,0);
	 }
	 if((particle[loop].x>1670)&&(particle[loop].x<1990)){
		 zone(0.203*particle[loop].x+140.78,430,ri_,-1,1);
	 }*/


 }

void DrawGLScene(GLvoid)								// ��������

{
							// ����ģ�ͱ任����

	for (loop=0;loop<MAX_PARTICLES;loop++)					// ѭ�����е�����

	{

//		if(particle_end)particle[loop].active=false;
 
		if (particle[loop].active)					// �������Ϊ�����

		{


			float px=particle[loop].x;				// ����X���λ��

			float py=particle[loop].y;				// ����Y���λ��

			float pz=particle[loop].z;			// ����Z���λ��


			glColor4f(particle[loop].R,particle[loop].G,particle[loop].B,particle[loop].life*0.1);
			
	//		sp=rand()%3;

		
		    glPushMatrix();
			glPushMatrix();
			glTranslatef(px,py,pz);
						glPushMatrix();
			glScalef(particle[loop].xi/500,1,1);
			glutSolidSphere(15,4,4);
						glPopMatrix();
			glPopMatrix();

			glPopMatrix();

//		glPopMatrix();

    	/*	glBegin(GL_TRIANGLE_STRIP);				// ���������δ�

				glTexCoord2d(1,1); glVertex3f(x+0.5f*particle[loop].life,y,z); 

				glTexCoord2d(0,1); glVertex3f(x,y,z); 

				glTexCoord2d(1,0); glVertex3f(x+0.1*particle[loop].life,y+0.25f*particle[loop].life,z+0.1*particle[loop].life); 

				glTexCoord2d(0,0); glVertex3f(x,y,z+0.25f*particle[loop].life);

				glTexCoord2d(0,1); glVertex3f(x+0.25f*particle[loop].life,y,z);

				glTexCoord2d(1,0); glVertex3f(x,y,z);
 
				glEnd();}*/

 
			particle[loop].x+=particle[loop].xi/(slowdown);	// ����X�����λ��

			particle[loop].y+=particle[loop].yi/(slowdown);	// ����Y�����λ��

			particle[loop].z+=particle[loop].zi/(slowdown);	// ����Z�����λ��


			particle[loop].xi+=particle[loop].xg;			// ����X�᷽���ٶȴ�С

			particle[loop].yi+=particle[loop].yg;			// ����Y�᷽���ٶȴ�С

			particle[loop].zi+=particle[loop].zg;			// ����Z�᷽���ٶȴ�С

			if((particle[loop].x>dm)) particle[loop].life-=particle[loop].fade;		// �������ӵ�����ֵ

 /*
				col=float(1-particle[loop].life)*11;

				particle[loop].R=colors[col][0];			// ����������ɫ

				particle[loop].G=colors[col][1];			

				particle[loop].B=colors[col][2];		*/

				check(loop);
				color(particle[loop].y,loop);
			if (particle[loop].life<0.0f)					// �����������ֵС��0
			{
				initial(loop);
				particle[loop].fir=0;
			}

 		
		}

    }


}




	