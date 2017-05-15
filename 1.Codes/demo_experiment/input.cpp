#include <cmath>
#include "../GL/glut.h"
#include <iostream>	
#include<windows.h>
using namespace std;

#define width 1366
#define height 768
#define testspeed 5

#define DELAY         5//main 还有一个
//extern float t_record[];
extern int cursor_position,cursor_angle;
extern const int SAMPLE_DIVISION;
extern int sample_radius[];
extern int speedChange;

int mouse_p_x=0 ,mouse_p_y=0/*,mouse_p_x2=0 ,mouse_p_y2=0*/;
extern float ri_;
float speed_jump=5;float speed_jump_t=0;
//int path=10;
 int pick_i=1,pick_j=1;
 float angle =-90.0;
 float angle_C = 0;
 float angle_up=0;
 float z=0.0;
 float x=0.0;
 float y=0;
 int xtest,ytest,ztest;
 extern int ballx,ballz;
 extern int step,mover_L_ai,mover_H_ai,pic;
 extern float HPC_stator_blade_1,HPC_stator_blade_2,HPC_stator_blade_3,HPC_stator_blade_4;
 extern int f_x,f_z;
 extern int width1;
 float speed=500.0;
 extern float angle_c_up;
//extern bool idle_t_p;
bool mouseLeftDown=false,mouseLeftUp=false,mouseRightDown=false,mouseMiddleDown=false;
int click_x,click_y;
extern bool check,adjust,cool,moni,over,nature;
extern int f_Hz;
bool w_flag=false,s_flag=false,a_flag=false,d_flag=false,n_flag=false,u_flag=false;
bool f_flag=false,i_flag=false,o_flag=false,p_flag=false,q_flag=false,e_flag=false;
bool space_flag=false,tab_flag=false,enter_flag=false;
extern int oven_num;
extern int collector_flag,measure_point,windtunnel_flag,anemoscope_flag;
extern float anemoscope_x,anemoscope_z;

int flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0,flag9=0;
extern bool cursor,particle_end,particle_i;
bool test_mode=false;
void renderScene(void);
/*void DrawGLScene(void);
void initialparticles(void);*/
//extern float mover_H_angle,mover_L_angle,width0,scene;
/*void idle_t()
{
	mover_H_angle-=float(mover_H_ai)*0.033;
	if (mover_H_angle<-360) mover_H_angle+=360;
	mover_L_angle-=float(mover_L_ai)*0.033;
	if (mover_L_angle<-360) mover_L_angle+=360;
//	printf("%d\t%d\n",mover_H_angle,step);


	if (scene==1) idle_fanframe();
	if (scene==2) idle_LPC();
	if (scene==3) idle_LPT();
	if (scene==4) idle_HPC();
	if (scene==5) idle_HPT();
	if (scene==6) idle_nozzle();
	
	 glutPostRedisplay();
}*/

void timer(int id)  
{  
	switch(id)
	{
	case 1: if (speed_jump_t>-speed_jump){y+=speed_jump_t*0.033;speed_jump_t-=9.8*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,1);}break;
	case 2: if(!test_mode){if (w_flag==true) {x+=speed*cos(angle_C)*0.033; z+=speed*sin(angle_C)*0.033; y+=speed*tan(angle_c_up)*0.033; glutPostRedisplay(); glutTimerFunc(33,timer,2);}break;}
			else{if (w_flag==true) {x+=speed*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,2);}break;}//test_mode x-x  y-r z-theta
	
	case 3: if(!test_mode){if (s_flag==true) {x-=speed*cos(angle_C)*0.033; z-=speed*sin(angle_C)*0.033; y-=speed*tan(angle_c_up)*0.033; glutPostRedisplay(); glutTimerFunc(33,timer,3);}break;}
			else{if (s_flag==true) {x-=speed*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,3);}break;}	
	
	case 4: if(!test_mode){if (a_flag==true) {x+=speed*sin(angle_C)*0.033; z-=speed*cos(angle_C)*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,4);}break;}//x-y面与x-z反
			else{if ((a_flag==true)&&(y>0.033)) {y-=speed*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,4);}break;}	
	
	case 5: if(!test_mode){if (d_flag==true) {x-=speed*sin(angle_C)*0.033; z+=speed*cos(angle_C)*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,5);}break;}
			else{if (d_flag==true) {y+=speed*0.033;glutPostRedisplay(); glutTimerFunc(33,timer,5);}break;}	
	
	case 6: if (u_flag==true) {ballx+=10;HPC_stator_blade_1+=1; if(HPC_stator_blade_1>45){HPC_stator_blade_1-=90;}    glutTimerFunc(33,timer,6);}break;
	case 7: if (i_flag==true) {ballx-=10;HPC_stator_blade_2+=1; if(HPC_stator_blade_2>45){HPC_stator_blade_2-=90;}    glutTimerFunc(33,timer,7);}break;
	case 8: if (o_flag==true) {ballz+=10;HPC_stator_blade_3+=1; if(HPC_stator_blade_3>45){HPC_stator_blade_3-=90;}    glutTimerFunc(33,timer,8);}break;
	case 9: if (p_flag==true) {ballz-=10;HPC_stator_blade_4+=1; if(HPC_stator_blade_4>45){HPC_stator_blade_4-=90;}    glutTimerFunc(33,timer,9);}break;
	
	case 10: if(test_mode){if (q_flag==true) {z-=speed*0.00033;glutPostRedisplay(); glutTimerFunc(33,timer,10);}}break;
	
	
	case 11: if(test_mode){if (e_flag==true) {z+=speed*0.00033;glutPostRedisplay(); glutTimerFunc(33,timer,11);}}break;	

//	case 12: if(!particle_end){DrawGLScene();glutPostRedisplay();/*glutTimerFunc(33,timer,12);*/}break;

	}
}  
void input()
{
	if (w_flag==true) timer(2);
	if (s_flag==true) timer(3);
	if (a_flag==true) timer(4);
	if (d_flag==true) timer(5);
	if (u_flag==true) timer(6);
	if (i_flag==true) timer(7);
	if (o_flag==true) timer(8);
	if (p_flag==true) timer(9);
	if (q_flag==true) timer(10);
	if (e_flag==true) timer(11);

}
void KeyUpProcess(unsigned char key, int xx, int yy)
{
	switch (key)
	{
	case 'A':;
    case 'a': a_flag=false;	break;
	case 'D':;
	case 'd': d_flag=false;	break;
	case 'W':;
	case 'w': w_flag=false;	break;
	case 'S':;
	case 's': s_flag=false;	break;
	case 'N':;
	case 'n': n_flag=false;	break;
	case 'U':;
    case 'u': u_flag=false; break;
	case 'I':;
    case 'i': i_flag=false; break;
	case 'O':;
    case 'o': o_flag=false; break;
	case 'P':;
    case 'p': p_flag=false; break;
	case 'Q':;
    case 'q': q_flag=false; break;
	case 'E':;
    case 'e': e_flag=false; break;
	case 'F':;
	case 'f': f_flag=false;break;
	case ' ': space_flag=true;break;
	case  13: enter_flag=true;break;
	case 'T':;
	case 't': test_mode=!test_mode; break;
//	case ' ': space_flag=true; break;
	case '+':;
	case '=':if(speedChange<DELAY-1)speedChange++;break;
	case '_':;
	case '-':if(speedChange>0)speedChange--;break;

    }
}

void KeyProcess(unsigned char key, int xx, int yy)
{	
	switch (key)
	{
	case 27 :     exit(0);break;
//    case '1':     exit(0);break;
	case 'A':;
    case 'a': a_flag=true;break;
	case 'D':;
	case 'd': d_flag=true;break;
	case 'W':;
	case 'w': w_flag=true;break;
	case 'S':;
	case 's': s_flag=true;break;
	case 'N':;
	case 'n': n_flag=true;if(step==4)nature=!nature;break;//print parametre
	case 'M':;
	case 'm': x=800;y=400;z=2400;angle=-90;angle_C=0;angle_c_up=0;angle_up=0;break;
	case 'F':;
	case 'f': f_flag=true;break;
	case 'I':;
	case 'i': i_flag=true;break;
	case 'O':;
	case 'o': o_flag=true;if(step==4) over=1;break;
	case 'P':;
	case 'p': p_flag=true;break;
	case 'U':;
	case 'u': u_flag=true;break;
	case 'Q':;
    case 'q': q_flag=true; break;
	case 'E':;
    case 'e': e_flag=true;moni=!moni;break;
//	case ' ': space_flag=true;break;
	case '	': tab_flag=!tab_flag;break;
//	case  13 : enter_flag=true;break;//回车
	case '2': flag2+=5;if(flag2>10) flag2=0; break;
	case '3': flag3+=5;if(flag3>10) flag3=0; break;
	case '4': xtest-=testspeed;cout<<"xtest"<<xtest<<'\n';flag4+=5;if(flag4>10) flag4=0; break;
	case '5': ytest-=testspeed;cout<<"ytest"<<ytest<<'\n';flag5+=5;if(flag5>10) flag5=0; break;
	case '6': ztest-=testspeed;cout<<"ztest"<<ztest<<'\n';flag6+=5;if(flag6>10) flag6=0; break;
	case '7': xtest+=testspeed;cout<<"xtest"<<xtest<<'\n';flag7+=5;if(flag7>10) flag7=0; break;
	case '8': ytest+=testspeed;cout<<"ytest"<<ytest<<'\n';flag8+=5;if(flag8>10) flag8=0; break;
	case '9': ztest+=testspeed;cout<<"ztest"<<ztest<<'\n';flag9+=5;if(flag9>10) flag9=0; break;
//	case 'v': particle_end=true;break;
//	case 'b': particle_end=false;particle_i=true;break;
	case ',': pic-=1;break;
	case '.': pic+=1;break;
	case 'y': if(step==1)step=2;if(step==3)step=4;
		      if((step==4)&&check) check=0;
			  if((step==4)&&adjust){adjust=0;particle_i=1;particle_end=0;cool=1;}
			  if((step==4)&&moni){moni=0;}
			  break;
	}
	input();
//	if(n_flag)printf("angle=%0.2f, x=%0.2f, z=%0.2f\n双转子转速比%d：%d\n第一级可调导叶角度%f\t第二级可调导叶角度%f\t第三级可调导叶角度%f\t第四级可调导叶角度%f\n",angle,x,z,mover_L_ai,mover_H_ai,HPC_stator_blade_1,HPC_stator_blade_2,HPC_stator_blade_3,HPC_stator_blade_4);
	renderScene();
}

void SpecialKeyUp(int key,int xx,int yy)
{
	
}

void SpecialKey(int key,int xx,int yy)
{
	switch(key)
	{
	case GLUT_KEY_LEFT: 
/*		if (step>=0)
		step-=1;
		else step = 0;*/
//		ri_+=50;
		if(step==0){
			++cursor_position;
			if(cursor_position<SAMPLE_DIVISION-5)sample_radius[cursor_position]=sample_radius[cursor_position-1];
		}
		else if(step==1)++cursor_position;
		else if(step==2&&oven_num>40)oven_num-=10;
		else if(anemoscope_flag==1&&anemoscope_x<650)anemoscope_x+=50;

		break;
	case GLUT_KEY_RIGHT: 
//		if (step<=9)
/*		step+=1;*/
//		ri_-=50;
		if(step==0){
			--cursor_position;
			if(cursor_position>5)sample_radius[cursor_position]=sample_radius[cursor_position+1];
		}
		else if(step==1)--cursor_position;
		else if(step==2&&oven_num<80)oven_num+=10;
		else if(anemoscope_flag==1&&anemoscope_x>0)anemoscope_x-=50;


		break;
	case GLUT_KEY_UP:
		if(step==0){
			if(sample_radius[cursor_position]>5)	sample_radius[cursor_position]-=5;
		}
		else if(step==1)++cursor_angle;
		else if(step==2&&oven_num<90)++oven_num;
		else if(step==3&&collector_flag<measure_point) ++collector_flag;
		else if(step==4&&windtunnel_flag<9&&anemoscope_flag==0)++windtunnel_flag;
		else if(anemoscope_flag==1&&anemoscope_z<600)anemoscope_z+=50;


		break;
	case GLUT_KEY_DOWN:
		if(step==0){
			if(sample_radius[cursor_position]<50)	sample_radius[cursor_position]+=5;
		}
		else if(step==1)--cursor_angle;		
		else if(step==2&&oven_num>30)--oven_num;
		else if(step==3&&collector_flag>1) --collector_flag;
		else if(step==4&&windtunnel_flag>1&&anemoscope_flag==0)--windtunnel_flag;
		else if(anemoscope_flag==1&&anemoscope_z>0)anemoscope_z-=50;


		break;
	}
	renderScene();
}

void Mouse(int button,int state,int xx,int yy)
{
//	click_x=xx;click_y=yy;
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN) 
        {
            mouseLeftDown = true;
/*			     if((xx>1020)&&(xx<1080)&&(yy<715)&&(yy>647))exit(0);
			else if((xx>1100)&&(xx<1160)&&(yy<715)&&(yy>647))exit(0);
			else if((xx>1180)&&(xx<1240)&&(yy<715)&&(yy>647))exit(0);
			else if((xx>1265)&&(xx<1320)&&(yy<715)&&(yy>647))exit(0);*/

        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
		    mouseLeftUp=true;
			cursor=false;
    }		

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP){
            mouseRightDown = false;
			cursor=true;
		}
    }
	else if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;

        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
	}



	if (mouseMiddleDown)
	{
		x+=speed*(xx-click_x);
		y+=speed*(yy-click_y);

	}

	input();
	renderScene();
}
/*
void mouseMotion(int x, int y)
{
    if(mouseLeftDown)
    {


    }
    if(mouseRightDown)
    {

    }

    glutPostRedisplay();
	
}*/


void mousePassiveMotion(int xx, int yy)
{
	/*angle += atan (float(xx - 0.5*width));
	angle_up+=atan (float(0.5*height - yy));
	SetCursorPos1(0.5*width,0.5*height);
	*/
	if((abs(xx-mouse_p_x)<500)&&(abs(yy-mouse_p_y)<500)){
	if ((xx<0.8*width)&&(xx>0.2*width))
	angle += 0.5*float(xx - mouse_p_x);
	if (angle_up>-95)
	angle_up+=0.1* (float(mouse_p_y - yy));
	else angle_up=-29;
	if (angle_up<80)
	angle_up+=0.1*(float(mouse_p_y - yy));
	else angle_up=79;

	
////////////////////////////////////////////////////////////
	    if (xx<0.19*width){
			SetCursorPos(int(0.8*width),yy);}
		else if (xx>0.81*width){
			SetCursorPos(int(0.2*width),yy);}
		if (yy<0.1*height)
			SetCursorPos(xx,int(0.8*height));
		else if(yy>0.9*height)
			SetCursorPos(xx,int(0.2*height));

////////////////////////////////////////////////////////////
	
		glutPostRedisplay();}mouse_p_x=xx ;mouse_p_y=yy;
//if (q_flag=true) printf("%u        %u \n",xx,yy);
}

void dis(void)
{
}
