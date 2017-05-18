
#pragma region includes

#include "../GL/glut.h"
#include "../GL/glaux.h"
#pragma comment (lib,"glut32.lib")   //C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib
#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <cmath>
#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <algorithm>
#include<windows.h>
#include<iostream>
#include <sstream>
#include <cstring>
#include <list>
#include <vector>
#include"pick.h"
#include <fstream>

#pragma endregion
using namespace std;


#pragma region DEFINEs

#define PI 3.1415926536
#define width0 1000
int width=800;//#define width 1366//1280//1366
int height=600;//#define height 768//1024//768
#define MAX_CHAR  128
#define yh  800
//#define rec_acc 10
#define xmax 2274
#define xmin -1240
#define zmax 959
#define zmin -935
#define MEASURE_MAX 3
#define ENVIRONMENT_T 25
#define SEND_EXE_ADDRESS           "TestEcho-Client-backage.exe"
#define OUTPUT_DATA_SAMPLE         "Client/parameter_sample.dat"
#define OUTPUT_DATA_LOCATION       "Client/TC_location_air_para.dat"

#define INPUT_DATA                 "Client/myDatreceive.dat"
#define NUM_OF_COR                 186

#define DELAY                      5//input 还有一个

#define RED 1
#define GREEN 2
#define BLUE 3
#define ORANGE 4


int speedChange=0;//[0,DELAY)
int timeInterval,duration;
vector<vector<float>> temperature_measure_vec;
vector<vector<float>> temperature_all_vec;
int sample_time=-1;
float Pr=0.702;
float Nu=0;
float lamda=0.02552;
double niu=16.155e-6;
float D=0.05;
int ballx=0,ballz=0;
extern float angle;
extern float angle_C ;
extern float angle_up;
float angle_c_up;
int pick_[20];
vector<float> t_record[MEASURE_MAX];
list<float> buffer[MEASURE_MAX];
extern int sample_radius[];
extern const int SAMPLE_DIVISION;
extern int cursor_position,cursor_angle;
int measure_point=0;
float measure_point_xyz[3*MEASURE_MAX]={0};
float all_point_xyz[3*(30+1)*6]={0};//SAMPLE_DIVISION
float temp_of_all_point[3*(30+1)*6];
float col_of_all_point[3*(30+1)*6];
bool collector_inverse[MEASURE_MAX];
bool samplemove_flag1=false,samplemove_flag2=false,ovenopen_flag=false,
	ovenclose_flag=false,screen_flag=false,heat_over=false;
int sample_t=0,ovendoor_t=0,windtunnel_flag=0;
int anemoscope_flag=0;
float anemoscope_speed,anemoscope_temp;
float anemoscope_x,anemoscope_z;
int oven_num=50,collector_flag=0,picked_sign_angle=0,material_no=0;
extern int xtest,ytest,ztest;

extern float z;
extern float x;
extern float y;
extern int pick_i,pick_j;
extern int step;
float wind_v=8;
float mover_H_angle=0 ,mover_L_angle=0;
float HPC_stator_blade_1=0,HPC_stator_blade_2=0,HPC_stator_blade_3=0,HPC_stator_blade_4=0;
bool cylinder_1_f=0,cylinder_2_f=0,cylinder_3_f=0,check=0,adjust=0,moni=0,over=0,nature=0;
int f_Hz=75;
extern bool enter_flag,space_flag,tab_flag,p_flag,c_flag,f_flag;
extern bool mouseLeftDown,mouseLeftUp;
extern int flag2,flag3,flag4,flag5,flag6,flag7,flag8,flag9;
int scene=0,step=0;
char stat1[200],stat2[200],stat3[200],stat4[200],stat5[200],stat6[200],stat7[200],stat8[200],stat9[200],stat10[200];
char stat11[200],stat12[200],stat13[200],stat14[200],stat15[200],stat16[200],stat17[200],stat18[200],stat19[200],stat20[200];

int width1;
GLuint texGround=0,texPart=0,texTool=0,texInfo=0;
GLuint texSave=0  ,texLoad=0,texExit=0,texCheck=0;
bool cursor=false,particle_end=true,particle_i=false;
extern bool test_mode;
int mover_H_ai=1,mover_L_ai=1;
int pic=0;//作图
int pick_flag=0;
#pragma endregion


pickIt my_pick;
int theOnePicked=0;

float red, green, blue;
int mainWindow, subWindow;


void processMenuEvents(int option) 
{
	switch (option) {
	case RED:
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f; break;
	case GREEN:
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f; break;
	case BLUE:
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f; break;
	case ORANGE:
		red = 1.0f;
		green = 0.5f;
		blue = 0.5f; break;
	}
}

void createGLUTMenus() 
{
	int menu;

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events
	menu = glutCreateMenu(processMenuEvents);

	//add entries to our menu
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Orange", ORANGE);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#pragma region possible funcitons

//objs
void Circle (float Radius,int CirAcc,float x,float y,float z,float v_x,float v_y,float v_z);
void Cylinder (float Radius,int CirAcc,float x,float y,float z,float v_x,float v_y,float v_z,float l);

void Cylinder2 (float Radius,float Radius2,float x,float y,float z,float v_x,float v_y,float v_z,
	float x2,float y2,float z2,float v_x2,float v_y2,float v_z2,float tex_h,float tex_dh,
	int CirAcc,vector<int> temperature_vec,int cursor,int division);
	
void ring (float c_r,float t_r,float x,float y,float z,float v_x,float v_y,float v_z);
void screw (float c_r,float pitch,float n,float t_r,float x,float y,float z,float v_x,float v_y,float v_z);
void Ball (float Radius,int CirAcc);
void sample(int t);
void parabola(float upx,float upy,float upz,float downx,float downy,float downz,int acc=12);
void thermocouple(float cenx,float ceny,float cenz,float yelx,float yely,float yelz,float purx,float pury,float purz,bool picked,bool inversecolor=false,int acc=12);
void screen();
double random(double start, double end);
//input
void timer(int id); 
void KeyUpProcess(unsigned char key, int xx, int yy);
void KeyProcess(unsigned char key, int xx, int yy);
void SpecialKeyUp(int key,int xx,int yy);
void SpecialKey(int key,int xx,int yy);
void Mouse(int button,int state,int x,int y);
void mouseMotion(int x, int y);
void mousePassiveMotion(int xx, int yy);
//void timer(int id);
//texture
GLuint load_texture(const char* file_name);
void part_tex(int step);
void Info_tex(int step);
void Tool_tex(int step);
//light
void myinit_light(void);
void myinit_light_t(void);
void material(void);
//list
GLvoid BuildLists(void);
void DrawList(int i);
//3dsxx
void DrawModel(int n);
void myinit_model(int n,char *FILE_NAME);
//particle
void initialparticles(void);
void DrawGLScene(GLvoid);

void Reshape(int w, int h)
{
	width=w;height=h;
/*	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) width1/(GLfloat) h, 1.0, 2000.0);
	BuildLists();/*/

}
/*
#include <vector>
#define INPUT_DATA                "C:/Users/Administrator/Desktop/Client/myDatreceive.dat"
#define OUT_PROFIX                "C:/Users/Administrator/Desktop/Client/"
#define OUT_POSTFIX                ".txt"
#define NUM_OF_COR                186
int timeInterval,duration;
vector<vector<int>> temperature_measure_vec;
vector<vector<int>> temperature_all_vec;*/
void myReadReceiveDat(const char *Filename,int& timeInterval,int& duration){
	ifstream fin(Filename);
	//timeInterval
	fin.ignore(30,':');
	fin>>timeInterval;
	//duration
	fin.ignore(30,':');
	fin>>duration;
	//temperature_measure_vec
	vector<float> temperature_temp;
	char namec[20];
	float namei;
	fin.ignore(30,':');//temperature_measure
	fin.getline(namec,20,':');
	while(namec[2]=='\0'){
		fin.ignore(0,':');
		while(temperature_temp.size()<duration){
			fin>>namei;
			temperature_temp.push_back(namei);
		}
		temperature_measure_vec.push_back(temperature_temp);
		temperature_temp.clear();
		fin.ignore(300,'\n');
		fin.getline(namec,20,':');
	}
	//temperature_all
	fin.ignore(30,':');
	while(temperature_all_vec.size()<duration){
		while(temperature_temp.size()<NUM_OF_COR){
			fin>>namei;
			temperature_temp.push_back(namei);
		}
		temperature_all_vec.push_back(temperature_temp);
		temperature_temp.clear();
		fin.ignore(300,':');
	}
}

void myGetSampleDat(const char *Filename,int material_no,int* arrayShape,int arraySize,int temperature)
 {
	ofstream fout(Filename);
	fout<<"Material:\t";//第一行
	switch(material_no){
	case 0:fout<<"AL\n";break;
	case 1:fout<<"STEEL\n";break;
	}
	
	char pChar[5];
	fout<<"Coordination&Radius[mm]:\n";//第二'行'
	for(int i=0;i<arraySize;i++){
		fout<<"\t("<<i*300/(arraySize-1)<<","<<arrayShape[i]<<")\n";//宏定义？？
	}

	fout<<"TEMPERATURE_INI[C]:\t"<<temperature<<'\n';//第三行

	fout.close();

 }

void myGetLocationDat(const char *Filename,int coordinate_no,float* arrayLocation,int velo_air,int temp_air)
 {
	ofstream fout(Filename);
	fout<<"NUM_coordination:\t"<<coordinate_no<<'\n';//第一行
	for(int i=0;i<coordinate_no;i++){
		fout<<"\t("<<arrayLocation[3*i]<<","<<arrayLocation[3*i+1]<<","<<arrayLocation[3*i+2]<<")\n";
	}

	fout<<"velocity_air[ms-1]:\t"<<velo_air<<'\n';
	fout<<"temperature_air[C]:\t"<<temp_air<<'\n';
	fout<<"all_coordinations:\t"<<(SAMPLE_DIVISION+1)*6<<'\n';
	
	for(int i=0;i<SAMPLE_DIVISION+1;i++){
		for(int j=0;j<6;j++){
			fout<<"\t("<<all_point_xyz[i*3*6+j*3]<<","<<all_point_xyz[i*3*6+j*3+1]<<","<<all_point_xyz[i*3*6+j*3+2]<<")\n";
		}
	}

	fout.close();

 }

#pragma endregion


GLuint ascii_lists;
void drawString(char* str) {  /////////文字
    static int isFirstCall = 1;

    if( isFirstCall ) { // 如果是第一次调用，执行初始化
                         // 为每一个ASCII字符产生一个显示列表
         isFirstCall = 0;

         // 申请MAX_CHAR个连续的显示列表编号
         ascii_lists = glGenLists(MAX_CHAR);

         // 把每个字符的绘制命令都装到对应的显示列表中
         wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, ascii_lists);
     }
     // 调用每个字符对应的显示列表，绘制每个字符
    for(; *str!='\0'; ++str)
         glCallList(ascii_lists + *str);
}

void selectFont(int size, int charset, const char* face) {
     HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
         charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
         DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
     HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
     DeleteObject(hOldFont);
}

void drawCNString(const char* str) {
    int len, i;
    wchar_t* wstring;
     HDC hDC = wglGetCurrentDC();
     GLuint list = glGenLists(1);

     // 计算字符的个数
     // 如果是双字节字符的（比如中文字符），两个字节才算一个字符
     // 否则一个字节算一个字符
     len = 0;
    for(i=0; str[i]!='\0'; ++i)
     {
        if( IsDBCSLeadByte(str[i]) )
             ++i;
         ++len;
     }

     // 将混合字符转化为宽字符
    wstring = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
     MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
    wstring[len] = L'\0';

     // 逐个输出字符
    for(i=0; i<len; ++i)
     {
         wglUseFontBitmapsW(hDC, wstring[i], 1, list);
         glCallList(list);
     }

     // 回收所有临时资源
    free(wstring);
     glDeleteLists(list, 1);
}


float pick(float dir_x,float dir_y,float dir_z,float eye_x,float eye_y,float eye_z,float ball_x,float ball_y,float ball_z);//colision_detection
//pick(cos(angle_C),tan(angle_c_up),sin(angle_C),x,y,z,,,,);


void picked_sign(float h,float x=0,float z=0){
	glPushMatrix();
	glTranslatef(x,h,z);
	glColor4f(0,1,0,1);
	glScalef(1,2,1);
	glRotatef(picked_sign_angle,0,1,0);
	glutSolidSphere(20,4,2);
	glPopMatrix();
}
void collector(int f){
	glPushMatrix();
	glTranslatef(0,0,200);
	if(theOnePicked==COLLECTOR_PICK){picked_sign(250);}
//	cout<<theOnePicked;
	if (f){
		DrawList(3);
		DrawList(4);
		DrawList(5);
	}
	glPopMatrix();

}

void monitor(int f){
	pick_[1]=pick(cos(angle_C),tan(angle_c_up),sin(angle_C),x,yh,z,640,500,1320);//-300,200,0);
	glPushMatrix();
	glTranslatef(3000,0,0);
	if(theOnePicked==MONITOR_PICK){picked_sign(1300,-100);}
	if (f){
		DrawList(2);
	}			 
	if(screen_flag) screen();
	glPopMatrix();	
}

void desk(int f){
	glPushMatrix();
	glTranslatef(0,-1000,0);
	if (f){
		DrawList(1);
	}
	glPopMatrix();	
}

void anemoscope(int f){
	pick_[2]=pick(cos(angle_C),tan(angle_c_up),sin(angle_C),x,yh,z,960,0,870);//-300,0,300);
	glPushMatrix();
	glTranslatef(800,0,-350);
	if(anemoscope_flag==1){
		glTranslatef(anemoscope_x-1450,75,anemoscope_z-50);
		glRotatef(-90,0,1,0);
	}
	if(theOnePicked==ANEMOSCOPE_PICK){picked_sign(150);}
	if (f){
		DrawList(9);
		DrawList(10);
		DrawList(11);
		glTranslatef(-20,10,300);
		glRotatef(90,0,-1,0);
		DrawList(12);
		DrawList(13);
	}
	glPopMatrix();	
}

void scissors(int f){
	glPushMatrix();
	glTranslatef(700,0,-400);	
	glRotatef(90,0,1,0);
//	if(theOnePicked==SCISSORS_PICK){}
	if (f){
		DrawList(23);
		DrawList(24);
		glRotatef(180,1,0,0);
		DrawList(23);
		DrawList(24);
	}				 
	glPopMatrix();	
}
void screw(int f){
	pick_[4]=pick(cos(angle_C),tan(angle_c_up),sin(angle_C),x,yh,z,1080,0,530);//0,0,400);
	glPushMatrix();
	glTranslatef(900,0,-300);
	glRotatef(90,0,1,0);
//	if(theOnePicked==SCREW_PICK){glTranslatef(0,100,0);}
	if (f){
		DrawList(21);
		DrawList(22);
	}				
	glPopMatrix();	

}
void shelf(int f){
	glPushMatrix();
	glTranslatef(-350,0,200);
	glScalef(1,2,1);
//	if(theOnePicked==SHELF_PICK){glTranslatef(0,100,0);}
	if (f){
		DrawList(14);
		DrawList(15);
	}
	glPopMatrix();
}

void LC_display(float x,float y,float z,int num_dis,char* str){
	
	selectFont(20, GB2312_CHARSET, "楷体_GB2312");//字符显示
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos3f(x,y,z); 
	char num[10];
	sprintf(num,"%d",num_dis);
	strcat(num,str);
	drawCNString(num);
	}

void oven(int f){	
	if (f){
	glPushMatrix();
	glTranslatef(520,0,120);
	if(theOnePicked==OVEN_PICK){picked_sign(500,200,200);}
	glRotatef(ovendoor_t,0,1,0);
		DrawList(17);
		DrawList(41);
	glPopMatrix();	
	glPushMatrix();
	glTranslatef(750,0,300);
//	if(theOnePicked==OVEN_PICK){glTranslatef(0,100,0);}
		DrawList(16);
		DrawList(18);
		DrawList(19);
	}
	glPopMatrix();	

	LC_display(990,300,50,oven_num,"°C");

}






float cylinder_col[30*30*5*3];
float h=0;
float Ch=0;
float L=0.4;
float rou=2.7e3,cp=880;
float t_inf=25,t0=ENVIRONMENT_T,t;
float tao=0,tao0=0;
float cylinder_t[30*30*5];
bool  cool=0;


void wind(float y,float zmin_,float zmax_,float r,float deltaz=1){
	if(abs(y-0)<1)return;
	bool negative=false;
	double vir_s=-5,vir_e=5;
	float vir_xk=random(vir_s,vir_e);

	if(y<0){
		glPushMatrix();
		glRotatef(180,0,0,1);
		negative=true;
		y=-y;
	}

	if(y>=2.1*r){
		glBegin(GL_LINES);
		glVertex3f(vir_xk*sin(z/50),y,zmin_);
		glVertex3f(vir_xk*sin(z/50),y,zmax_);
		glEnd();
	}
	else if(y>0&&y<2.1*r){
		glBegin(GL_LINE_STRIP);

		for(float z=zmin_;z<zmax_;z+=deltaz){
			if(z<-2*r||z>=2*r)      glVertex3f(vir_xk*sin(z/50),y,z);
			else if(z>=-r&&z<r)     glVertex3f(vir_xk*sin(z/50),y+(0.525-y/4/r)*(2*r*r-z*z)/r,z);
			else if(z>=-2*r&&z<-r)  glVertex3f(vir_xk*sin(z/50),y+(0.525-y/4/r)*(z+2*r)*(z+2*r)/r,z);
			else                    glVertex3f(vir_xk*sin(z/50),y+(0.525-y/4/r)*(z-2*r)*(z-2*r)/r,z);
		}
		
		glEnd();
	}
	
	if(negative){
		glPopMatrix();
	}
//	Circle(r,10,0,0,0,1,0,0);

}

void wind_array(int density_x,int density_y,bool sample_exist=true){
	float gap=15;
	glPushMatrix();
	for(int i=0;i<density_x;++i){
		glTranslatef(300.0f/density_x,0,0);
		glTranslatef(0,-0.5*gap,0);
		for(int j=0;j<density_y;++j){
			if(sample_exist)wind((float(j)/density_y-0.5)*gap,-500,200,sample_radius[(i+1)*SAMPLE_DIVISION/density_x]+5);
			else wind((float(j)/density_y-0.5)*gap,-500,200,1);
			glTranslatef(0,gap/density_y,0);
		}
		glTranslatef(0,-0.5*gap,0);
	}	
	glPopMatrix();
}

void wind_tunnel(){	
	glPushMatrix();
    glTranslatef(-500,-900,600);
	if(theOnePicked==WIND_TUNNEL_PICK){picked_sign(1300);}
	DrawList(6);
	DrawList(7);
	DrawList(8);
	DrawList(40);
    glTranslatef(0,1100,-400);
	glColor4f(0.3,0.7,1,0.5);

	if(windtunnel_flag>0){
		if(step==4||(step==5&&samplemove_flag2))  wind_array(5+windtunnel_flag/2,3+windtunnel_flag/3,false);
		else wind_array(5+windtunnel_flag/2,3+windtunnel_flag/3);//最密的情况 10,6
		LC_display(-40,20,280,windtunnel_flag,"");

	}
	glPopMatrix();
}


void aim(int f){
	glColor3f(0,1,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_LINES);//瞄准
	glVertex2f(0,0.05);glVertex2f(0, -0.05);
	glVertex2f(0.3,0);glVertex2f(-0.3,0);
	glEnd();
}

/*
*/
void renderScene(void)
{
	#pragma region init_renderscene

	static bool widFlag=true;
	if(widFlag){
		width1=width-320;
		widFlag=false;
	}

	angle_C = (angle) * PI /180;
	angle_c_up=(angle_up) * PI /180;


	// Clear Color and Depth Buffers
	glClearColor(0.7,0.7,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
/*
	glMatrixMode(GL_TEXTURE);
    glLoadIdentity();*/
//	glDisable(GL_TEXTURE_2D);

	glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);


	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	myinit_light();


    glViewport(0,0,width1,height);////////////////视口1//////////
    gluPerspective(60.0, GLfloat(width1)/height, 1.0, 20000.0);
//	glOrtho(700,700,700,700, 1,20000);


	glMatrixMode(GL_MODELVIEW);
	
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, yh, z,   
				x+cos(angle_C), yh+tan(angle_c_up) , z+sin(angle_C), 
				0,1,0	);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	material();
	//glColorMaterial(GL_FRONT,GL_DIFFUSE);
	glColorMaterial(GL_FRONT,GL_AMBIENT); 
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND); // 打开混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );




/*
		glPushMatrix();//粒子效果，接应于约850行
        glRotatef(90,0,1,0);
        glTranslatef(-600,-100,600);*/

#pragma endregion

	
	//find out which item is picked
	theOnePicked=my_pick.printPickedNum(cos(angle_C),tan(angle_c_up),sin(angle_C),x,yh,z);
	/*
	if(x>xmax)x=xmax;//移动区域限制
	else if(x<xmin)x=xmin;
	if(z>zmax)z=zmax;
	else if(z<zmin)z=zmin;
	*/

    if (!(step==-1)){
			
		DrawList(25);//room
		DrawList(26);
		DrawList(27);
		DrawList(28);
		glPushMatrix();
        glTranslatef(400,0,200);
		DrawList(29);
		glPopMatrix();
		

	    collector(1);
	    monitor(1);
	    desk(1);	
	    anemoscope(1);
	    screw(1);
	    shelf(1);
	    oven(1);
	    scissors(1);
	    
	    wind_tunnel();
	    aim(1);
	    
	    glPushMatrix();
	    glTranslatef(-350,200,200);
	    glRotatef(sample_t,0,-1,0);
	    glTranslatef(0,0,-sample_t*12.0);
	    glRotatef(sample_t,0,-1,0);
	    sample(sample_time);//objs.cpp
	    glPopMatrix();

	}
	//固定部件显示完成



	if(step==0){//捏试样,********注意添加选择材料()变颜色
		if(enter_flag){
			//获取sample_radius[];
			step=1;
			enter_flag=false;

			//all_point_xyz赋值
			for(int i=0;i<SAMPLE_DIVISION+1;i++){
				for(int j=0;j<6;j++){
					all_point_xyz[i*3*6+j*3]=i*300/SAMPLE_DIVISION;//x=
					all_point_xyz[i*3*6+j*3+1]= sample_radius[i]*sin(j/3.0f*PI);//y=
					all_point_xyz[i*3*6+j*3+2]= sample_radius[i]*cos(j/3.0f*PI);//z=
				}
			}			
		}
		if(space_flag){
			//多次获取测量点
			space_flag=false;
			material_no++;
			if(material_no>1)material_no=0;//0-AL 1-STEEL
		}
	}


    if (step==1){//选择测温部位，最后step=2
		if(space_flag){
			//多次获取测量点
			space_flag=false;
			if(measure_point<MEASURE_MAX){
				measure_point_xyz[measure_point*3]=cursor_position*300/SAMPLE_DIVISION;//x=
				measure_point_xyz[measure_point*3+1]= sample_radius[cursor_position]*sin(cursor_angle/6.0*PI);//y=
				measure_point_xyz[measure_point*3+2]=-sample_radius[cursor_position]*cos(cursor_angle/6.0*PI);//z=
				//step=2;
				++measure_point;
			}
			else ;//提示过多
		}
		
		if(theOnePicked==OVEN_PICK&&mouseLeftDown&&measure_point>0
			){
			for(int i=0;i<9;i++)cout<<measure_point_xyz[i]<<'\n';//获取measure_point_xyz及measure_point
			samplemove_flag1=true;
			ovenopen_flag=true;
			step=2;
		}
/*
		if(measure_point>MEASURE_MAX-1||enter_flag){
			step=2;
			enter_flag=false;
		}*/
	}

	if(step==2){
		//试样移位，待加热命令，step=3
		//提示上下左右该温度，回车确认
		if(enter_flag){
			//获取sample_radius[];
			ovenclose_flag=true;
			enter_flag=false;
			myGetSampleDat(OUTPUT_DATA_SAMPLE,material_no,sample_radius,SAMPLE_DIVISION+1,oven_num);
		}		
		if(theOnePicked==COLLECTOR_PICK&&mouseLeftDown&&ovendoor_t<10){
			step=3;
			collector_flag=1;
		}

	}
		
	static bool init=true;//随机错误热电偶
	if(init){
		for(int i=1;i<MEASURE_MAX;++i){
			collector_inverse[i]=int(random(1,3))-1;
		}
		collector_inverse[0]=false;//认为false是错的
		init=false;
	}
	
	if(step==3){
		//开始加热，确认结束后，step=4；期间连接热电偶
		if(theOnePicked==MONITOR_PICK&&mouseLeftDown){
//			if(screen_flag);//清空buffer数据,暂未实现
			screen_flag=true;
		}
		if(collector_flag){
			for(int i=0;i<measure_point;++i){
				thermocouple(-10,350,170+i*60/measure_point,15,100,170+i*60/measure_point,-15,100,170+i*60/measure_point,!(collector_flag-i-1),collector_inverse[i]);
			}//collector_inverse
		}

		if(enter_flag){
			//监测热电偶是否都正确
			for(int i=0;i<measure_point;++i){
				if(collector_inverse[i]==true) step=4;
				else {
					step=3;break;//提示存在错误
				}
			}
			enter_flag=false;
		}
		if(space_flag){
			//获取sample_radius[];
			if(collector_flag<=MEASURE_MAX) collector_inverse[collector_flag-1]=!collector_inverse[collector_flag-1];
			space_flag=false;
		}

	}

//	cout<<"t0"<<t0<<'\n';
	if(step==4){
		//结束加热		
		for(int i=0;i<measure_point;++i){
			thermocouple(-10,350,170+i*60/measure_point,15,100,170+i*60/measure_point,-15,100,170+i*60/measure_point,!(collector_flag-i-1),collector_inverse[i]);
		}

		if(theOnePicked==WIND_TUNNEL_PICK&&mouseLeftDown){//选择冷却模式,完成后回车
			windtunnel_flag=1;
		}		
		
		if(theOnePicked==ANEMOSCOPE_PICK&&mouseLeftDown){//测温测风速
			anemoscope_flag=1;
		}
		
		if((anemoscope_flag!=1)&&enter_flag){	
			enter_flag=false;
		}

		if(anemoscope_flag==1){	
			float kx=1.125-0.001*abs(anemoscope_x-325),kz=anemoscope_z/1750+29.0/35.0;
			if(kx>1||windtunnel_flag==0)kx=1;
			if(kz>1||windtunnel_flag==0)kz=1;
	        anemoscope_speed=sqrt(kx*kz)*windtunnel_flag;
			anemoscope_temp=sqrt(kx*kz)*(ENVIRONMENT_T-anemoscope_speed);
			if(enter_flag){		
			myGetLocationDat(OUTPUT_DATA_LOCATION,measure_point,measure_point_xyz,anemoscope_speed,anemoscope_temp);
			system(SEND_EXE_ADDRESS);///设置容错措施；进入等待界面
			myReadReceiveDat(INPUT_DATA,timeInterval,duration);
			//vector<vector<int>> temperature_measure_vec;
			//vector<vector<int>> temperature_all_vec;
			
			anemoscope_flag=2;
			enter_flag=false;
		}
		}


		if(oven_num-int(t0)>2){
			//提示正在加热
			//升温
		}
		else {//冷却
			if(anemoscope_flag==2){
				samplemove_flag2=true;
				ovenopen_flag=true;
				step=5;
			}
		}
		
	}

	if(step==5){
		//开始冷却
		if(!samplemove_flag2){
			ovenclose_flag=true;
			for(int i=0;i<measure_point;++i){
				thermocouple(-500+measure_point_xyz[i*3],200+measure_point_xyz[i*3+1],200+measure_point_xyz[i*3+2],
					45,135,170+i*60/measure_point,
				    15,135,170+i*60/measure_point,
					false,collector_inverse[i]);
		    }
		}

	}   
//	glPopMatrix();	接应上面粒子
	

if(particle_i){	initialparticles();particle_i=false;}//fire
if(!particle_end) DrawGLScene();

/*	glBindTexture(GL_TEXTURE_2D, texGround);
//	minit_list();
	    glEnable(GL_COLOR_MATERIAL);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	    glColor3f(0.72,0.49,0.02);



	*/
	//////////////////////////////////////
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) 100, 2.0, 5.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	0, 0, 3,   
				0, 0, 0, 
				0, 1, 0	);	
	glDisable(GL_LIGHTING);
	glViewport(0,0,width1,height);////////////////视口2//////////瞄准
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	
	glColor3f(0,1,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_LINES);//瞄准
	glVertex2f(0,0.05);glVertex2f(0, -0.05);
	glVertex2f(0.3,0);glVertex2f(-0.3,0);
	glEnd();
    glColor3f(1,1,1);
		

///////////////////////////////	


glMatrixMode(GL_PROJECTION);
glViewport(width1,0,width-width1,height);////////////////视口4//////////提示

    glLoadIdentity();
			gluLookAt(-1,0,0,   
				0,0,0, 
				0,1,0);	
				glOrtho(-100,100,-100,100,0,1000);
	glClearColor(0,0,0,1);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



	float stat_z=-10;

	selectFont(18, GB2312_CHARSET, "楷体_GB2312");
	glColor3f(1.0f, 0.0f, 0.0f);////////yoooooooooooooooooooooooooooooooooooooooooooooooooooooooo
	sprintf(stat1,"提示：室温 %d°C ",ENVIRONMENT_T);
	glRasterPos3f(0.0f,70.0f,stat_z); 
	drawCNString(stat1);
    sprintf(stat1,"      空气运动粘度 16.155e-6 m^2/s ");//其他室温也不会变，待改
	glRasterPos3f(0.0f,65.0f,stat_z);
	drawCNString(stat1);
	sprintf(stat1,"      Pr=0.702");
	glRasterPos3f(0.0f,60.0f,stat_z);
	drawCNString(stat1);
	sprintf(stat1,"操作说明：WSAD及鼠标控制移动");
	glRasterPos3f(0.0f,-40.0f,stat_z);
	drawCNString(stat1);
	sprintf(stat1,"          tab键隐藏提示栏");
	glRasterPos3f(0.0f,-45.0f,stat_z);
	drawCNString(stat1);
	sprintf(stat1,"          注意关闭输入法");
	glRasterPos3f(0.0f,-50.0f,stat_z);
	drawCNString(stat1);
	sprintf(stat1,"          Esc强制中断实验");
	glRasterPos3f(0.0f,-55.0f,stat_z);
	drawCNString(stat1);

	#pragma region theonepicked

	if(theOnePicked==0){
		sprintf(stat1,"已选中： ");  
	}
	if(theOnePicked==COLLECTOR_PICK){
		sprintf(stat1,"已选中：采集仪 ");  
	}
	if(theOnePicked==MONITOR_PICK){
		sprintf(stat1,"已选中：PC显示器 ");  
	}
	if(theOnePicked==ANEMOSCOPE_PICK){
		sprintf(stat1,"已选中：风速仪 ");  
	}
	if(theOnePicked==SCISSORS_PICK){
		sprintf(stat1,"已选中：剪刀 ");
	}
	if(theOnePicked==SCREW_PICK){
		sprintf(stat1,"已选中：螺丝刀 ");
	}
	if(theOnePicked==SHELF_PICK){
		sprintf(stat1,"已选中：支架 ");
	}
	if(theOnePicked==OVEN_PICK){
		sprintf(stat1,"已选中：加热炉 ");
	}
	if(theOnePicked==WIND_TUNNEL_PICK){
		sprintf(stat1,"已选中：风机 ");
	}
#pragma endregion

	glRasterPos3f(0.0f,50.0f,stat_z);
	drawCNString(stat1);

	sprintf(stat1,"当前步骤：");	
	glRasterPos3f(0.0f,5.0f,stat_z);
	drawCNString(stat1);

	if(step>=2&&step<5){
		sprintf(stat1,"加热中，试样当前温度：%.2f °C",t0);
		glRasterPos3f(0.0f,15.0f,stat_z);
		drawCNString(stat1);
	}

	if(step==0){
		sprintf(stat1,"方向键改变试样形状");	
		glRasterPos3f(0.0f,0.0f,stat_z);
		drawCNString(stat1);
		switch(material_no){
		case 0:	sprintf(stat1,"空格变换材料，当前：铝    ");break;
		case 1: sprintf(stat1,"空格变换材料，当前：不锈钢"); 
		}
		glRasterPos3f(0.0f,-5.0f,stat_z);
		drawCNString(stat1);
		sprintf(stat1,"当前直径:%dmm 当前位置:%dmm",sample_radius[cursor_position],(cursor_position-5)*300/SAMPLE_DIVISION);	
		glRasterPos3f(0.0f,-10.0f,stat_z);
		drawCNString(stat1);
		sprintf(stat1,"回车确认");	
		glRasterPos3f(0.0f,-15.0f,stat_z);
		drawCNString(stat1);
	}

	if(step==1){
		sprintf(stat1,"方向键移动光标");
	    glRasterPos3f(0.0f,0.0f,stat_z);
	    drawCNString(stat1);	
		sprintf(stat1,"空格确定测温位置");
	    glRasterPos3f(0.0f,-5.0f,stat_z);
	    drawCNString(stat1);	
        sprintf(stat1,"最多%d个(%d/%d)",MEASURE_MAX,measure_point,MEASURE_MAX);  
	    glRasterPos3f(0.0f,-10.0f,stat_z);
	    drawCNString(stat1);
        sprintf(stat1,"全部完成后左键点选加热炉");  
	    glRasterPos3f(0.0f,-15.0f,stat_z);
	    drawCNString(stat1);
	}
	
	if(step==2){
		if(ovendoor_t>10){
			sprintf(stat1,"方向键选择加热温度，回车确认");
	        glRasterPos3f(0.0f,0.0f,stat_z);
	        drawCNString(stat1);	
            sprintf(stat1,"当前：%d°C",oven_num);  
	        glRasterPos3f(0.0f,-5.0f,stat_z);
	        drawCNString(stat1);
		}
		if(ovendoor_t<10){
			sprintf(stat1,"左键点选数据采集仪连接热电偶");
			glRasterPos3f(0.0f,0.0f,stat_z);
			drawCNString(stat1);	
		}
	}
		
	if(step==3){
        if(collector_flag==0){
			sprintf(stat1,"左键点选数据采集仪连接热电偶");
			glRasterPos3f(0.0f,0.0f,stat_z);
			drawCNString(stat1);	
		}
	    if(collector_flag==1&&!screen_flag){
            sprintf(stat1,"连接完成");  
	        glRasterPos3f(0.0f,0.0f,stat_z);
	        drawCNString(stat1);
            sprintf(stat1,"左键点选显示屏查看温度输出结果");  
	        glRasterPos3f(0.0f,-5.0f,stat_z);
	        drawCNString(stat1);
	    }
	    if(screen_flag){
			sprintf(stat1,"若正负极接反，输出温度为异常值");  
			glRasterPos3f(0.0f,-0.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"上下方向键改变选中的热电偶");  
			glRasterPos3f(0.0f,-5.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"空格转换正负极");  
			glRasterPos3f(0.0f,-10.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"直到完全正确后按回车");  
			glRasterPos3f(0.0f,-15.0f,stat_z);
			drawCNString(stat1);
		}
	}
		
	if(step==4){
	    if(windtunnel_flag==0&&anemoscope_flag==0){
			sprintf(stat1,"强制冷却：左键点选风机");
			glRasterPos3f(0.0f,0.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"自然冷却：直接进入下一步");
			glRasterPos3f(0.0f,-5.0f,stat_z);
			drawCNString(stat1);	
		}
	    if(windtunnel_flag>0&&anemoscope_flag==0){
			sprintf(stat1,"上下方向键改变出风大小");
			glRasterPos3f(0.0f,0.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"当前%d档",windtunnel_flag);
			glRasterPos3f(0.0f,-5.0f,stat_z);
			drawCNString(stat1);
		}
	    if(anemoscope_flag==0){
			sprintf(stat1,"左键点选热线风速仪");
			glRasterPos3f(0.0f,-10.0f,stat_z);
			drawCNString(stat1);
		}
	    if(anemoscope_flag==1){
			sprintf(stat1,"方向键移动热线风速仪");  
			glRasterPos3f(0.0f,0.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"测量气流温度及速度");  
			glRasterPos3f(0.0f,-5.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"尽量靠近试样支架获得准确数据");  
			glRasterPos3f(0.0f,-10.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"温度:%.2f°C 风速:%.2f m/s",anemoscope_temp,anemoscope_speed);  
			glRasterPos3f(0.0f,-15.0f,stat_z);
			drawCNString(stat1);
			sprintf(stat1,"回车确认当前数据,开始冷却");  
			glRasterPos3f(0.0f,-20.0f,stat_z);
			drawCNString(stat1);
		}
	}	
			
	if(step==5&&!samplemove_flag2){
        sprintf(stat1,"冷却开始");
	    glRasterPos3f(0.0f,0.0f,stat_z);
	    drawCNString(stat1);
	}	
	

/*	glRasterPos3f(0.0f,80.0f,-80.0f);
	drawCNString(stat9);	  
	glRasterPos3f(0.0f,70.0f,-80.0f);
	drawCNString(stat10);*/

	glColor3f(0.0f, 0.0f, 0.0f);
		glLineWidth(5);
	glBegin(GL_LINES);
		glVertex3f(0.0f,500.0f,-0.0f);
		glVertex3f(0.0f,-500.0f,-0.0f);
	glEnd();
	glLineWidth(1);

	glutSwapBuffers();
}



void idle()
{   
	//sample move
	if(samplemove_flag1&&sample_t<90)++sample_t;
	else samplemove_flag1=false;
	if(samplemove_flag2&&sample_t> 0)--sample_t;
	else samplemove_flag2=false;

	if(ovenopen_flag &&ovendoor_t<150)ovendoor_t+=5;
	else ovenopen_flag=false;
	if(ovenclose_flag&&ovendoor_t> 0)ovendoor_t-=10;
	else ovenclose_flag=false;

	//数据显示
	for(int i=0;i<measure_point;++i){
		if(buffer[i].size()>=1050)buffer[i].pop_front();
		if(step==3||step==4){
			if(!collector_inverse[i])buffer[i].push_back(random(0,100));
			else buffer[i].push_back(ENVIRONMENT_T+random(-1,1));
		}
		else if(step==5&&!samplemove_flag2){
			static int durationi=0;
			int delay=(DELAY-speedChange+5);
			int step=durationi%delay;
			sample_time=durationi/delay;
			if(sample_time<duration-1){//(durationi<duration*DELAY-1){
				float bufferToPush= ( temperature_measure_vec[i][sample_time]*(delay-step) + temperature_measure_vec[i][sample_time+1]*step ) /float(delay);
				buffer[i].push_back(bufferToPush);
				durationi++;
			}
			else{
				buffer[i].push_back(temperature_measure_vec[i][duration-1]);//需要延时
			    sample_time=duration-1;
			}
		}
	}

	picked_sign_angle++;
	
	//oven_temperature
	if(step==3||step==4||(step==2&&ovendoor_t<10)){tao0+=0.05/(DELAY-speedChange);t0=oven_num-1.0/(tao0/125+1.0/float(oven_num-ENVIRONMENT_T));}
//	if((cool)&&(tao<19999))tao+=0.5;

	
	if ((tab_flag)&&(width1<width))  width1+=10;
    if ((!tab_flag)&&(width1>width-320)) width1-=10;
	glutPostRedisplay();
}


void MenuFunc(int MenuItem)// 菜单项处理函数
{
	
	switch(MenuItem)
	{
	case 0:scene=0;step=0;renderScene();break;
	case 1:scene=1;step=0;renderScene();break;
	case 2:scene=2;step=0;renderScene();break;
	case 3:scene=3;step=0;renderScene();break;
	case 4:scene=4;step=0;renderScene();break;
	case 5:scene=5;step=0;renderScene();break;
	case 6:scene=6;step=0;renderScene();break;
	case 11:scene=11;step=0;renderScene();break;
	case 12:scene=12;step=0;renderScene();break;
	case 13:scene=13;step=0;renderScene();break;
	case 14:scene=14;step=0;renderScene();break;
	case 15:scene=15;step=0;renderScene();break;
	case 16:scene=16;step=0;renderScene();break;

	case 21:mover_H_ai=2*mover_L_ai;scene=0;step=0;renderScene();break;
	case 22:mover_H_ai=3*mover_L_ai;scene=0;step=0;renderScene();break;
	case 23:mover_H_ai=4*mover_L_ai;scene=0;step=0;renderScene();break;


	default:break;
	}
	cursor=false;
}

void init_model(){

	myinit_model(1, "../../2.UGmodel/3DS/1_table.3DS");
myinit_model(2, "../../2.UGmodel/3DS/2_monitor.3DS");
myinit_model(3, "../../2.UGmodel/3DS/4_collector_blk.3DS");
myinit_model(4, "../../2.UGmodel/3DS/4_collector_blu.3DS");
myinit_model(5, "../../2.UGmodel/3DS/4_collector_wht.3DS");
myinit_model(6, "../../2.UGmodel/3DS/5_wind_tunnel_blk.3DS");
myinit_model(7, "../../2.UGmodel/3DS/5_wind_tunnel_gry.3DS");
// myinit_model(8,"../../2.UGmodel/3DS/5_wind_tunnel_none.3DS");
myinit_model(8, "../../2.UGmodel/3DS/5_wind_tunnel_wht.3DS");
myinit_model(9, "../../2.UGmodel/3DS/6_anemoscope_blu.3DS");
myinit_model(10, "../../2.UGmodel/3DS/6_anemoscope_blu_lgt.3DS");
myinit_model(11, "../../2.UGmodel/3DS/6_anemoscope_wht.3DS");
myinit_model(12, "../../2.UGmodel/3DS/6_anemoscope_wire_blk.3DS");
myinit_model(13, "../../2.UGmodel/3DS/6_anemoscope_wire_wht.3DS");
myinit_model(14, "../../2.UGmodel/3DS/8_shelf_wht_1.3DS");
myinit_model(15, "../../2.UGmodel/3DS/8_shelf_wht_2.3DS");
myinit_model(16, "../../2.UGmodel/3DS/9_oven_blu.3DS");
myinit_model(17, "../../2.UGmodel/3DS/9_oven_door_blu.3DS");
// myinit_model(19,"../../2.UGmodel/3DS/9_oven_door_none.3DS");
myinit_model(18, "../../2.UGmodel/3DS/9_oven_gry.3DS");
myinit_model(19, "../../2.UGmodel/3DS/9_oven_wht.3DS");
myinit_model(20, "../../2.UGmodel/3DS/C_box.3DS");
myinit_model(21, "../../2.UGmodel/3DS/D_screw_blu.3DS");
myinit_model(22, "../../2.UGmodel/3DS/D_screw_wht.3DS");
myinit_model(23, "../../2.UGmodel/3DS/e_scissors_wht.3DS");
myinit_model(24, "../../2.UGmodel/3DS/e_scissors_yel.3DS");
myinit_model(25, "../../2.UGmodel/3DS/f_room_celling.3DS");
myinit_model(26, "../../2.UGmodel/3DS/f_room_wall.3DS");
myinit_model(27, "../../2.UGmodel/3DS/f_room_floor.3DS");
myinit_model(28, "../../2.UGmodel/3DS/f_room_door.3DS");
myinit_model(29, "../../2.UGmodel/3DS/f_room_scene.3DS");

myinit_model(40, "../../2.UGmodel/3DS/5_wind_tunnel_none.3DS");
myinit_model(41, "../../2.UGmodel/3DS/9_oven_door_none.3DS");
BuildLists();// room box
#pragma endregion
}


void init() {
	
	glutKeyboardFunc(KeyProcess);
	glutSpecialFunc(SpecialKey);

	glutKeyboardUpFunc(KeyUpProcess);
	glutSpecialUpFunc(SpecialKeyUp);
	glutMouseFunc(Mouse);
	//	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassiveMotion);

	createGLUTMenus();

	glutIgnoreKeyRepeat(1);//无视连发
	ShowCursor(cursor);  // 隐藏鼠标指针

	pick_initial();

	init_model();

}


void main()
{     
	// init GLUT and create window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 定义窗口特征：双缓存、RGBA颜色模式
 	glutInitWindowPosition(0,0); // 窗口位置
	glutInitWindowSize(width,height); //窗口大小
	mainWindow=glutCreateWindow("try"); // 窗口标题
	glutFullScreen();
	
	//callbacks for mainwindow
	glutDisplayFunc(renderScene);
	glutReshapeFunc(Reshape);
	glutIdleFunc(idle);
	init();

	// enter GLUT event processing cycle
	glutMainLoop();

}
