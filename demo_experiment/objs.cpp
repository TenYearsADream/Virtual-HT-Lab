#include "../GL/glut.h"
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <list>
#include <vector>
using namespace std;
#define PI 3.1415926535898
extern const int SAMPLE_DIVISION=30;

int sample_radius[SAMPLE_DIVISION+1];//+5=实际半径
bool sample_ini_flag=false;
int cursor_position,cursor_angle;
extern int step;
extern bool space_flag;
extern int xtest,ytest,ztest;
extern list<float> buffer[];
extern float z;
extern float x;
extern float y;

void drawCNString(const char* str);
void selectFont(int size, int charset, const char* face);
GLuint load_texture(const char* file_name);

void Circle (float Radius,int CirAcc,float x,float y,float z,float v_x,float v_y,float v_z)//圆（半径，份数，坐标，法向量）
{
	float r;r=sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef((acos(v_z/r))*180/PI,-v_y,v_x,0);
	glBegin(GL_POLYGON);
		glNormal3f( 0.0f, 0.0f, v_z);	//////
	    int i;
		float Angle;
		for (i=1;i<=CirAcc;i++){
			Angle=i*2*PI/CirAcc;
			glVertex2f(Radius*cos(Angle),Radius*sin(Angle));
		};
	glEnd();
	glPopMatrix();
}
void Cylinder (float Radius,int CirAcc,float x,float y,float z,float v_x,float v_y,float v_z,float l)//圆柱（半径，份数，坐标，法向量）
{
	int i;
	float Angle;
	float r;r=sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	glPushMatrix();
    glTranslatef(x,y,z);
	glRotatef((acos(v_z/r))*180/PI,-v_y,v_x,0);
	glBegin(GL_QUAD_STRIP);
		for (i=1;i<=CirAcc+1;i++){
			
			    Angle=i*2*PI/CirAcc;                   //Cylinder(1,51,1,1,-1,1,1,-1,4)
		if (i%2==1) glNormal3f( cos(Angle), sin(Angle), 0);	//////
			glTexCoord2f(float(i-1)/CirAcc, 1);   glVertex3f(Radius*cos(Angle),Radius*sin(Angle),l);
			glTexCoord2f(float(i-1)/CirAcc, 0);   glVertex3f(Radius*cos(Angle),Radius*sin(Angle),0);

		};
	glEnd();
	Circle(Radius,CirAcc,0,0,0,0,0,1);
	Circle(Radius,CirAcc,0,0,l,0,0,-1);
	glPopMatrix();
}


void Cylinder2 (float Radius,float Radius2,float x,float y,float z,float v_x,float v_y,float v_z,
	float x2,float y2,float z2,float v_x2,float v_y2,float v_z2,float tex_h,float tex_dh,
	int CirAcc=100)
{                                        //类圆柱：空间两个圆相连 不封闭（半径，圆1坐标，法向量，圆2坐标，法向量）份数  CirAcc   
	int i;
	float Angle;
	float r;r=sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	float r2;r2=sqrt(v_x2*v_x2+v_y2*v_y2+v_z2*v_z2);
	float xx,yy,zz,xx2,yy2,zz2;
	glBegin(GL_QUAD_STRIP);
		for (i=1;i<=CirAcc+1;i++){
			Angle=i*2*PI/CirAcc;       
			xx=x+(Radius*v_y*cos(Angle)+Radius*v_x*v_z*sin(Angle))/sqrt(r*r-v_z*v_z);
    		xx2=x2+(Radius2*v_y2*cos(Angle)+Radius2*v_x2*v_z2*sin(Angle))/sqrt(r2*r2-v_z2*v_z2);
		    yy=y-(Radius*v_x*cos(Angle)+Radius*v_y*v_z*sin(Angle))/sqrt(r*r-v_z*v_z);
	    	yy2=y2-(Radius2*v_x2*cos(Angle)+Radius2*v_y2*v_z2*sin(Angle))/sqrt(r2*r2-v_z2*v_z2);
		    zz=z-Radius/r*sqrt(r*r-v_z*v_z)*sin(Angle);
	    	zz2=z2-Radius2/r2*sqrt(r2*r2-v_z2*v_z2)*sin(Angle);
 		  if (i%2==1) glNormal3f( xx+xx2-x-x2, yy+yy2-y-y2, zz+zz2-z-z2);	//////
			glTexCoord2f(float(i-1)/CirAcc, tex_h+tex_dh);   glVertex3f(xx2,yy2,zz2);
			glTexCoord2f(float(i-1)/CirAcc, tex_h);   glVertex3f(xx,yy,zz);

		};

	glEnd();
//	Circle(Radius,CirAcc,x,y,z,v_x,v_y,v_z);
//	Circle(Radius,CirAcc,x2,y2,z2,v_x2,v_y2,v_z2);

}
void Ball (float Radius,int CirAcc)
{
	float h=Radius/CirAcc;
	for (int i=0;i<CirAcc;i++)
	{
		Cylinder2(sqrt(Radius*Radius-i*i*h*h),sqrt(Radius*Radius-(i+1)*(i+1)*h*h),0,i*h,0,0,h,0,0,(i+1)*h,0,0,h,0,float(i)/CirAcc,1/float(CirAcc));
	}
	h*=-1;
	for (int i=0;i<CirAcc;i++)
	{
		Cylinder2(sqrt(Radius*Radius-i*i*h*h),sqrt(Radius*Radius-(i+1)*(i+1)*h*h),0,i*h,0,0,h,0,0,(i+1)*h,0,0,h,0,float(i)/CirAcc,1/float(CirAcc));
	}
}

void ring (float c_r,float t_r,float x,float y,float z,float v_x,float v_y,float v_z)//圆环（应用类圆柱）（半径，截面半径，坐标，法向量）
{
		int i,CirAcc=50;
		float Angle;
		float r;r=sqrt(v_x*v_x+v_y*v_y+v_z*v_z);//ring(2,0.5,2,2,2,0,1,1);
		glPushMatrix();
        glTranslatef(x,y,z);
	    glRotatef((acos(v_z/r))*180/PI,-v_y,v_x,0);
		for (i=1;i<=CirAcc+1;i++){
			Angle=i*2*PI/CirAcc;   
			Cylinder2(t_r,t_r,cos(Angle),sin(Angle),0,-sin(Angle),cos(Angle),0,cos(Angle+2*PI/CirAcc),sin(Angle+2*PI/CirAcc),0,-sin(Angle+2*PI/CirAcc),cos(Angle+2*PI/CirAcc),0,float(i)/CirAcc,1/float(CirAcc));

		};
		glPopMatrix();
}
void screw (float c_r,float pitch,float n,float t_r,float x,float y,float z,float v_x,float v_y,float v_z)//螺线（半径，螺距，圈数，截面半径，坐标，法向量）
{
		int i,CirAcc=50;
		float Angle;
		float r;r=sqrt(v_x*v_x+v_y*v_y+v_z*v_z);//screw(5,1,7.5,0.1,0,0,-2,0,0,1);
		glPushMatrix();
        glTranslatef(x,y,z);
	    glRotatef((acos(v_z/r))*180/PI,-v_y,v_x,0);
		for (i=1;i<=(CirAcc+1)*n;i++){
		  Angle=i*2*PI/CirAcc;   
		  Cylinder2(t_r,t_r,cos(Angle),sin(Angle),(i-1)*pitch/CirAcc,-sin(Angle),cos(Angle),0,cos(Angle+2*PI/CirAcc),sin(Angle+2*PI/CirAcc),i*pitch/CirAcc,-sin(Angle+2*PI/CirAcc),cos(Angle+2*PI/CirAcc),0,float(i)/CirAcc,1/float(CirAcc));
		};

		Angle=2*PI/CirAcc;   
		Circle(t_r,CirAcc,cos(Angle),sin(Angle),0,sin(Angle),-cos(Angle),0);//两边封口
		Angle=2*PI/CirAcc*(CirAcc+1)*n;   
		Circle(t_r,CirAcc,cos(Angle+2*PI/CirAcc),sin(Angle+2*PI/CirAcc),(CirAcc+1)*n*pitch/CirAcc,-sin(Angle+2*PI/CirAcc),cos(Angle+2*PI/CirAcc),0);
		glPopMatrix();
}

void sample_cursor(float end_x,float end_y,float end_z,float v_x,float v_y,float v_z,float length=100){
	v_x=v_x/sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	v_y=v_y/sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	v_z=v_z/sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	glColor3f(1,0,0);
	Cylinder(2,4,end_x,end_y,end_z,-v_x,-v_y,-v_z,length);

}

void Cylinder3 (float Radius,float Radius2,float x,float y,float z,float v_x,float v_y,float v_z,
	float x2,float y2,float z2,float v_x2,float v_y2,float v_z2,float tex_h,float tex_dh,
	int CirAcc,vector<int> temperature_vec,int cursor,int division)
{                                        //类圆柱：空间两个圆相连 不封闭（半径，圆1坐标，法向量，圆2坐标，法向量）份数  CirAcc   
	int i;
	float Angle;
	float r;r=sqrt(v_x*v_x+v_y*v_y+v_z*v_z);
	float r2;r2=sqrt(v_x2*v_x2+v_y2*v_y2+v_z2*v_z2);
	float xx,yy,zz,xx2,yy2,zz2;       
			glColor3f(0,1,0);
	glBegin(GL_QUAD_STRIP);
		for (i=1;i<=CirAcc+1;i++){
			Angle=i*2*PI/CirAcc;                   
			glColor3f(i/float(CirAcc),0,0);

			xx=x+(Radius*v_y*cos(Angle)+Radius*v_x*v_z*sin(Angle))/sqrt(r*r-v_z*v_z);
    		xx2=x2+(Radius2*v_y2*cos(Angle)+Radius2*v_x2*v_z2*sin(Angle))/sqrt(r2*r2-v_z2*v_z2);
		    yy=y-(Radius*v_x*cos(Angle)+Radius*v_y*v_z*sin(Angle))/sqrt(r*r-v_z*v_z);
	    	yy2=y2-(Radius2*v_x2*cos(Angle)+Radius2*v_y2*v_z2*sin(Angle))/sqrt(r2*r2-v_z2*v_z2);
		    zz=z-Radius/r*sqrt(r*r-v_z*v_z)*sin(Angle);
	    	zz2=z2-Radius2/r2*sqrt(r2*r2-v_z2*v_z2)*sin(Angle);
 		  if (i%2==1) glNormal3f( xx+xx2-x-x2, yy+yy2-y-y2, zz+zz2-z-z2);	//////
			glTexCoord2f(float(i-1)/CirAcc, tex_h+tex_dh);   glVertex3f(xx2,yy2,zz2);
			glTexCoord2f(float(i-1)/CirAcc, tex_h);   glVertex3f(xx,yy,zz);

		};

	glEnd();
//	Circle(Radius,CirAcc,x,y,z,v_x,v_y,v_z);
//	Circle(Radius,CirAcc,x2,y2,z2,v_x2,v_y2,v_z2);

}

void drawSample()
{	
	glPushMatrix();
	for(int j=0;j<SAMPLE_DIVISION;j++){
		glBegin(GL_QUAD_STRIP);
		float Angle;
		for (int i=0;i<=60;i++){
			Angle=i*2*PI/60.0f;
			glNormal3f( 0.0f, sin(Angle),cos(Angle));
			glVertex3f(j*300.0f/SAMPLE_DIVISION-150,sample_radius[j]*sin(Angle),sample_radius[j]*cos(Angle));
			glVertex3f((j+1)*300.0f/SAMPLE_DIVISION-150,sample_radius[j+1]*sin(Angle),sample_radius[j+1]*cos(Angle));
		}
		glEnd();
	}
	
	glBegin(GL_POLYGON);
		float Angle;
		for (int i=0;i<=60;i++){
			Angle=i*2*PI/60.0f+PI/2;
			glNormal3f( 1.0f, 0,0);
			if(i==0)
				glVertex3f(-150,100*sin(Angle),100*cos(Angle));
			else
			glVertex3f(-150,sample_radius[0]*sin(Angle),sample_radius[0]*cos(Angle));
		}
	glEnd();
	glBegin(GL_POLYGON);
		for (int i=0;i>=-60;i--){
			Angle=i*2*PI/60.0f;
			glNormal3f( -1.0f, 0,0);
			glVertex3f(150,sample_radius[SAMPLE_DIVISION]*sin(Angle),sample_radius[SAMPLE_DIVISION]*cos(Angle));
		}
	glEnd();
	glPopMatrix();

}

extern vector<vector<float>> temperature_measure_vec;

void drawSample(vector<float> temperature_vec)
{	
	float tmax=temperature_measure_vec[0][0];
	glPushMatrix();
	
	for(int j=0;j<SAMPLE_DIVISION;j++){
		glBegin(GL_QUAD_STRIP);
		float Angle;
		for (int i=0;i<=60;i++){
			float t;
			if(i==60)
				t=temperature_vec[j*6];
			else
				t=temperature_vec[i/10+j*6];
			float a=(t-25)/(tmax-25);
			float b=1-2/(tmax-25)*abs(t-(tmax+25)/2);
			glColor3f(a,b,1-a);

			Angle=i*2*PI/60.0f+PI;
			glNormal3f( 0.0f, sin(Angle),cos(Angle));
			glVertex3f(j*300.0f/SAMPLE_DIVISION-150,sample_radius[j]*sin(Angle),sample_radius[j]*cos(Angle));
			glVertex3f((j+1)*300.0f/SAMPLE_DIVISION-150,sample_radius[j+1]*sin(Angle),sample_radius[j+1]*cos(Angle));
		}
		glEnd();
	}
	
	
	glBegin(GL_POLYGON);
		float Angle;
		for (int i=0;i<=60;i++){	
			float t;
			if(i==60)
				t=temperature_vec[0];
			else
				t=temperature_vec[i/10];
			float a=(t-25)/(tmax-25);
			float b=1-2/(tmax-25)*abs(t-(tmax+25)/2);
			glColor3f(a,b,1-a);
			Angle=i*2*PI/60.0f+PI;
			glNormal3f( 1.0f, 0,0);
			glVertex3f(-150,sample_radius[0]*sin(Angle),sample_radius[0]*cos(Angle));
		}
	glEnd();

	glBegin(GL_POLYGON);
		for (int i=0;i<=60;i++){
			float t;
			if(i==60)
				t=temperature_vec[SAMPLE_DIVISION*6];
			else
				t=temperature_vec[i/10+SAMPLE_DIVISION*6];
			float a=(t-25)/(tmax-25);
			float b=1-2/(tmax-25)*abs(t-(tmax+25)/2);
			glColor3f(a,b,1-a);
			Angle=-i*2*PI/60.0f+PI;
			glNormal3f( -1.0f, 0,0);
			glVertex3f(150,sample_radius[SAMPLE_DIVISION]*sin(Angle),sample_radius[SAMPLE_DIVISION]*cos(Angle));
		}
	glEnd();

	glPopMatrix();

}

extern float measure_point_xyz[];
extern int measure_point,material_no;
extern vector<vector<float>> temperature_all_vec;//[duration][186]

void sample (int t){		//试样坐标未变换
/*	if(!sample_ini_flag||(space_flag&&step==0)){//initial radius=5    空格重置
		for(int i=0;i<SAMPLE_DIVISION+1;i++){
			sample_radius[i]=5;
		}
		sample_ini_flag=true;
		space_flag=false;
	}*/
	if(!sample_ini_flag){//initial radius=5    空格重置
		for(int i=0;i<SAMPLE_DIVISION+1;i++){
			sample_radius[i]=5;
		}
		sample_ini_flag=true;
	}


	if(t<0){
		if(material_no)	glColor3f(0.5,0.5,0.5);//换材料变色
		else glColor3f(0.9,0.9,0.9);
		/*
		for(int i=0;i<SAMPLE_DIVISION;i++){
			Cylinder2(sample_radius[i],sample_radius[i+1],
				i*10-150,0,0,
				1,0,0,
				(i+1)*10-150,0,0,
				1,0,0,
				SAMPLE_DIVISION/300.0*i,SAMPLE_DIVISION/300.0,
				60);//,vector<int> temperature_vec,int cursor,int division
		}
		Circle(sample_radius[0],100,-150,0,0,-1,0,0);//两边封口
		Circle(sample_radius[SAMPLE_DIVISION],100, 150,0,0, 1,0,0);*/
		drawSample();
	}
	else{
		drawSample(temperature_all_vec[t]);
	}

	if(step==0||step==1){
		//cursor
	glColor3f(1,0,0);
	if(cursor_position>SAMPLE_DIVISION-5)cursor_position=5;
	else if(cursor_position<5)cursor_position=SAMPLE_DIVISION-5;
	glPushMatrix();
	glRotatef(30*cursor_angle,1,0,0);
	sample_cursor(cursor_position*10-150,0,-sample_radius[cursor_position],0,0,1);
	glPopMatrix();
	}
	//words
	selectFont(28, 134, "楷体_GB2312");
	glColor3f(0,0,0);
	for(int i=0;i<measure_point;++i){
		glRasterPos3f(measure_point_xyz[i*3]-140,measure_point_xyz[i*3+1]*2,measure_point_xyz[i*3+2]*2);
		char num[2];
		sprintf(num,"%d",i+1);
		drawCNString(num);

	}

}//if((!samplemove_flag2)&&(step==5))

void parabola(float upx,float upy,float upz,float downx,float downy,float downz,int acc=12){
	glLineWidth(5);
	glPushMatrix();
	glTranslatef(upx,upy,upz);
	glRotatef(atan((upz-downz)/(upx-downx))*180/PI,0,-1,0);
	float xz=(upx-downx)*(upx-downx)+(upz-downz)*(upz-downz);
	float deltax=sqrt(xz)/acc;

	glBegin(GL_LINE_STRIP);
	for(int i=0;i<acc;++i) glVertex3f(deltax*i,(downy-upy)/(xz)*deltax*i*deltax*i,0);
	glEnd();
	glPopMatrix();
	
	glLineWidth(1);
	}


void thermocouple(float cenx,float ceny,float cenz,float yelx,float yely,float yelz,float purx,float pury,float purz,bool picked,bool inversecolor=false,int acc=12){	
	glColor3f(0,0,0);
	glPushMatrix();
	glTranslatef(cenx,ceny,cenz);
	if(picked)glutSolidSphere(10,5,5);
	else glutSolidSphere(5,5,5);
	glPopMatrix();

	if(!inversecolor){
		glColor3f(0.8,0.8,0.2);
		parabola(cenx,ceny,cenz,yelx,yely,yelz,acc);
		glColor3f(0.8,0.0,0.8);
		parabola(cenx,ceny,cenz,purx,pury,purz,acc);
	}
	else{
		glColor3f(0.8,0.0,0.8);
		parabola(cenx,ceny,cenz,yelx,yely,yelz,acc);
		glColor3f(0.8,0.8,0.2);
		parabola(cenx,ceny,cenz,purx,pury,purz,acc);
	}
}
double random(double start, double end)
{
	static bool init=true;
	if(init){
		srand(unsigned(time(0)));
		init=false;
	}
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

void screen(){
	static int tex_num;
	static bool init=true;
	if(init){
        tex_num=load_texture("../../../Texture/screen.bmp");
		init=false;
		/////////////


		///////////
	}
	glBindTexture(GL_TEXTURE_2D, tex_num);
	glColor3f(0,0,0);	

    glBegin(GL_QUADS);
	
	glTexCoord2f(1.74f, 1.0f); glVertex3f(-20,1120,670);//纹理坐标很迷，不知道为什么
	glTexCoord2f(0.74f, 1.0f); glVertex3f(-20,1120,-670);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20,80,-670);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20,80,670);
	
	glEnd();
	


	
	glLineWidth(2);

	for(int i=0;i<measure_point;++i){
		glColor3f(1-2.0*i/measure_point,1-abs(1-2.0*i/measure_point),2.0*i/measure_point-1);
		glBegin(GL_LINE_STRIP);
		int tao=0;
		for(list<float>::iterator iter=buffer[i].begin();
			iter!=buffer[i].end();++iter){
    		glVertex3f(-20,190+640/100.0*(*iter),tao-550);
			++tao;
		}
		
    	glVertex3f(-20,190+640/100.0*(buffer[i].back()),-550);
		glEnd();
		char data_back[5];
		
		selectFont(20, 134, "楷体_GB2312");
		glRasterPos3f(xtest-30,int(ytest+180+640/100.0*(buffer[i].back()))/30*30,ztest-650);
		sprintf(data_back,"%.1f",buffer[i].back());
	    drawCNString(data_back);	  
	}
	glLineWidth(1);
}//buffer数据用于显示，还有后台数据用于记录
