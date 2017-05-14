#include "../GL/glut.h"

extern int step;
float move_x[30],
	  move_y[20],
	  move_z[20],
	  move_a[20][200];
//list
void DrawList(int i);


void dis_translate_x(int s,int d){
 		if (step>s-1){
		glPushMatrix();
		        if (step==s){
					glTranslatef(move_x[s-1],0,0);	
				}
		DrawList(d);
	    glPopMatrix();
		}
}
void dis_translate_y(int s,int d){
 		if (step>s-1){
		glPushMatrix();
		        if (step==s){
					glTranslatef(0,move_y[s-1],0);	
				}
		DrawList(d);
	    glPopMatrix();
		}
}
void dis_translate_x_rotate(int s,int n,int d,int m,int bladeinall,int bladeinagroup,int group){
 if (step>s-1){
	if (step==s){
		for(int j=0;j<group;j++){
			for (int i=0;i<bladeinagroup;i++){
				glPushMatrix();
				glRotatef(360/float(bladeinall)*float(j*bladeinagroup+i),-1,0,0);
				glTranslatef(move_a[n][j]*(1+0.1*float(i)),0,0);
				DrawList(d);
				glPopMatrix();
			}
		}
	}
	else {if (m>0) DrawList(m);
	      else 	for(int j=0;j<group;j++){
			  for (int i=0;i<bladeinagroup;i++){
				  glPushMatrix();
				  glRotatef(360/float(bladeinall)*float(j*bladeinagroup+i),-1,0,0);
				  DrawList(d);
				  glPopMatrix();
			  }
		  }
	}
 }
}

void dis_translate_y_rotate(int s,int n,int d,int m,int bladeinall,int bladeinagroup,int group){
 if (step>s-1){
	if (step==s){
		for(int j=0;j<group;j++){
			for (int i=0;i<bladeinagroup;i++){
				glPushMatrix();
				glRotatef(360/float(bladeinall)*float(j*bladeinagroup+i),-1,0,0);
				glTranslatef(0,move_a[n][j]*(1+0.1*float(i)),0);
				DrawList(d);
				glPopMatrix();
			}
		}
	}
	else {if (m>0) DrawList(m);
	      else 	for(int j=0;j<group;j++){
			  for (int i=0;i<bladeinagroup;i++){
				  glPushMatrix();
				  glRotatef(360/float(bladeinall)*float(j*bladeinagroup+i),-1,0,0);
				  DrawList(d);
				  glPopMatrix();
			  }
		  }
	}
 }
}

void idle_translate_x(int s){	if (step==s) { if (move_x[s-1]<0 )   {move_x[s-1]+=10 ;} else move_x[s-1]=0;}else move_x[s-1]=-5000;}
void idle_translate_x_m(int s){	if (step==s) { if (move_x[s-1]>0 )   {move_x[s-1]-=10 ;} else move_x[s-1]=0;}else move_x[s-1]= 5000;}
void idle_translate_y(int s){	if (step==s) { if (move_y[s-1]<0 )   {move_y[s-1]+=10 ;} else move_y[s-1]=0;}else move_y[s-1]=-500;}
void idle_translate_y_m(int s){	if (step==s) { if (move_y[s-1]>0 )   {move_y[s-1]-=10 ;} else move_y[s-1]=0;}else move_y[s-1]= 500;}

void idle_translate_x_rotate(int s,int n,int group,float gap){
	if (step==s) {
		for(int j=0;j<group;j++){ 
			if (j==0) {if (move_a[n][0]<0) {move_a[n][0]+=10 ;} else move_a[n][0]=0;}
			else {
				if(move_a[n][j-1]<0) move_a[n][j]=move_a[n][j-1]-gap;
				else {if (move_a[n][j]<0) move_a[n][j]+=10;
				      else move_a[n][j]=0;}
			}

		}}
	else {for(int i=0;i<group;i++) {move_a[n][i]=-1000;}}
	}
void idle_translate_x_rotate_m(int s,int n,int group,float gap){
	if (step==s) {
		for(int j=0;j<group;j++){ 
			if (j==0) {if (move_a[n][0]>0) {move_a[n][0]-=10 ;} else move_a[n][0]=0;}
			else {
				if(move_a[n][j-1]>0) move_a[n][j]=move_a[n][j-1]+gap;
				else {if (move_a[n][j]>0) move_a[n][j]-=10;
				      else move_a[n][j]=0;}
			}

		}}
	else {for(int i=0;i<group;i++) {move_a[n][i]=1000;}}
	}



void idle_translate_y_rotate(int s,int n,int group,float gap){
	if (step==s) {
		for(int j=0;j<group;j++){ 
			if (j==0) {if (move_a[n][0]<0) {move_a[n][0]+=10 ;} else move_a[n][0]=0;}
			else {
				if(move_a[n][j-1]<0) move_a[n][j]=move_a[n][j-1]-gap;
				else {if (move_a[n][j]<0) move_a[n][j]+=10;
				      else move_a[n][j]=0;}
			}

		}}
	else {for(int i=0;i<group;i++) {move_a[n][i]=-300;}}
	}
void idle_translate_y_rotate_m(int s,int n,int group,float gap){
	if (step==s) {
		for(int j=0;j<group;j++){ 
			if (j==0) {if (move_a[n][0]>0) {move_a[n][0]-=10 ;} else move_a[n][0]=0;}
			else {
				if(move_a[n][j-1]>0) move_a[n][j]=move_a[n][j-1]+gap;
				else {if (move_a[n][j]>0) move_a[n][j]-=10;
				      else move_a[n][j]=0;}
			}

		}}
	else {for(int i=0;i<group;i++) {move_a[n][i]=300;}}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void dis_fanframe(void){
		glColor4f(0.9,0.9,1,1);
		DrawList(7);
		dis_translate_x(1,6);
		glColor4f(0.7,0.7,0,1);
		dis_translate_x(2,8);
		glColor4f(0.9,0.9,1,1);
		dis_translate_x(3,5);
		dis_translate_x(4,4);
}
void idle_fanframe(void){
	for (int i=1;i<=4;i++)idle_translate_x(i);
}

void dis_LPC(void){
		glColor4f(0.9,0.9,1,1);
		DrawList(70);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(1,66);
		glColor4f(0.9,0.9,1,1);
		dis_translate_x(2,69);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(3,65);
		glColor4f(0.9,0.9,1,1);
		dis_translate_x(4,68);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(5,64);
		glColor4f(0.9,0.9,1,1);
		dis_translate_x(6,67);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(7,3);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(8,10);
		dis_translate_x(9,9);

		glColor4f(0.7,0.7,0.7,1);
		dis_translate_x_rotate(10,0,2,1,46,1,46);
}
void idle_LPC(void){
	for (int i=1;i<=9;i++)idle_translate_x(i);

	idle_translate_x_rotate(10,0,46,200);

}

void dis_nozzle(void){
		glColor4f(0.5,0.5,0.5,1);
		DrawList(99);
		dis_translate_x(1,97);
		glColor4f(0.7,0.7,0.7,1);
		dis_translate_x(2,96);
		glColor4f(0.5,0.5,0.5,1);
		dis_translate_x(3,98);
}
void idle_nozzle(void){for (int i=1;i<=3;i++)idle_translate_x(i);}

void dis_HPT(void){
		glColor4f(0.7,0.7,0.5,1);
		DrawList(62);
		dis_translate_x(1,59);
		glColor4f(0.7,0.7,1,1);
		dis_translate_x(2,57);
		dis_translate_x_rotate(3,0,54,53,46,1,46);
		dis_translate_x(4,56);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(5,61);
		dis_translate_x(6,60);
		dis_translate_x_rotate(7,1,52,51,24,1,24);
		dis_translate_x(8,58);
		glColor4f(0.7,0.7,1,1);
		dis_translate_x(9,55);
		glColor4f(0.7,0.7,0.5,1);
		dis_translate_x(10,63);
}
void idle_HPT(void){
	for (int i=1;i<=2;i++)idle_translate_x(i);
	idle_translate_x_rotate(3,0,46,200);
	for (int i=4;i<=6;i++)idle_translate_x(i);
	idle_translate_x_rotate(7,1,24,200);
	for (int i=8;i<=10;i++)idle_translate_x(i);

}
void dis_LPT(void){ 
	glColor4f(0.4,0.4,0.3,1);
	for (int i=0;i<3;i++){
	dis_translate_x(1+i*5,72+i);
	dis_translate_x_rotate(2+i*5,0+i*3,76+i,80+i,180,6,30);
	dis_translate_x(3+i*5,84+i);
	dis_translate_x_rotate(4+i*5,1+i*3,89+2*i,0,24,3,8);
	dis_translate_y_rotate(5+i*5,2+i*3,90+2*i,0,8,1,8);}

	
	dis_translate_x(16,88);
	dis_translate_x(17,75);
	dis_translate_x_rotate(18,9,79,83,180,6,30);
	dis_translate_x(19,87);


	glColor4f(0.7,0.7,0.6,0.5);
	DrawList(71);
}


void idle_LPT(void){
	for (int i=0;i<3;i++){
	idle_translate_x_m(1+5*i);
	idle_translate_x_rotate_m(2+5*i,0+3*i,30,200);
	idle_translate_x_m(3+5*i);
	idle_translate_x_rotate_m(4+5*i,1+3*i,8,200);
	idle_translate_y_rotate_m(5+5*i,2+3*i,8,30);}
	idle_translate_x_m(16);
	idle_translate_x_m(17);
	idle_translate_x_rotate_m(18,9,30,200);
	idle_translate_x_m(19);

}



void dis_HPC(void){
	glColor4f(0.5,0.5,0.1,1);
	DrawList(11);DrawList(30);
	
	glColor4f(0.5,0.5,0.3,1);//9
	dis_translate_x_rotate(2,0,29,20,76,1,76);
	glColor4f(0.5,0.5,0.8,1);
	dis_translate_x_rotate(3,1,46,41,72,1,72);

	glColor4f(0.5,0.5,0.3,1);//8
	dis_translate_x_rotate(4,2,28,19,80,1,80);
	glColor4f(0.5,0.5,0.8,1);
	dis_translate_x_rotate(5,3,45,40,72,1,72);
	
	glColor4f(0.5,0.5,0.3,1);//7
	dis_translate_x_rotate(6,4,27,18,82,1,82);
	glColor4f(0.5,0.5,0.8,1);
	dis_translate_x_rotate(7,5,44,39,72,1,72);
	
	glColor4f(0.5,0.5,0.3,1);//6
	dis_translate_x_rotate(8 ,6,26,17,82,1,82);
	glColor4f(0.5,0.5,0.8,1);
	dis_translate_x_rotate(10,7,43,38,72,1,72);
	
	glColor4f(0.5,0.5,0.3,1);//5
	dis_translate_x_rotate(11,8,25,16,75,1,75);
	glColor4f(0.5,0.5,0.8,1);
	dis_translate_x_rotate(12,9,42,37,72,1,72);
	
	glColor4f(0.5,0.5,0.3,1);//4
	dis_translate_x_rotate(13,10,24,15,68,1,68);
	
	glColor4f(0.5,0.5,0.8,1);
	if(step>13){
	glPushMatrix();
	glRotatef(3.88,1,0,0);
	for(int n=0;n<36;n++){
			glPushMatrix();
			glRotatef(10*n,1,0,0);
	    	glTranslatef(846.73,0,-432.5);
	    	DrawList(36);//HPC_stator_blade_4_r432.5_l846.73
			glPopMatrix();
	}
	glPopMatrix();
	}
		
	glColor4f(0.5,0.5,0.3,1);//3
	dis_translate_x_rotate(14,11,23,14,60,1,60);
	
	glColor4f(0.5,0.5,0.8,1);
	if(step>14){
	glPushMatrix();
	glRotatef(3.88,1,0,0);
	for(int n=0;n<36;n++){
			glPushMatrix();
			glRotatef(10*n,1,0,0);
	    	glTranslatef(771.15,0,-442.5);
	    	DrawList(35);//HPC_stator_blade_3_r442.5_l771.15
			glPopMatrix();
	}
	glPopMatrix();
	}
			
	glColor4f(0.5,0.5,0.3,1);//2
	dis_translate_x_rotate(15,12,22,13,53,1,53);
	
	glColor4f(0.5,0.5,0.8,1);
	if(step>15){
	glPushMatrix();
	glRotatef(3.88,1,0,0);
	for(int n=0;n<36;n++){
			glPushMatrix();
			glRotatef(10*n,1,0,0);
	    	glTranslatef(676.07,0,-457.5);
	    	DrawList(34);//HPC_stator_blade_2_r457.5_l676.07
			glPopMatrix();
	}
	glPopMatrix();
	}
			
	glColor4f(0.5,0.5,0.3,1);//1
	dis_translate_x_rotate(16,13,21,12,38,1,38);
	
	glColor4f(0.5,0.5,0.8,1);
	if(step>16){
	glPushMatrix();
	glRotatef(3.88,1,0,0);
	for(int n=0;n<36;n++){
			glPushMatrix();
			glRotatef(10*n,1,0,0);
	    	glTranslatef(581.7,0,-462.5);
	    	DrawList(33);//HPC_stator_blade_1_r462.5_l581.7_degree3.88~36
			glPopMatrix();
	}
	glPopMatrix();
	}


	glColor4f(0.5,0.5,0.1,0.5);
	dis_translate_x(9,31);dis_translate_x(9,47);dis_translate_x(9,48);dis_translate_x(9,49);dis_translate_x(9,50);
	
	glColor4f(0.5,0.5,0.2,0.5);
	dis_translate_x(1,32);

}


void idle_HPC(void){
	
	idle_translate_x(1);
	idle_translate_x_rotate(2,0,76,200);
	idle_translate_x_rotate(3,1,72,200);
	idle_translate_x_rotate(4,2,80,200);
	idle_translate_x_rotate(5,3,72,200);
	idle_translate_x_rotate(6,4,82,200);
	idle_translate_x_rotate(7,5,72,200);
	idle_translate_x_rotate(8,6,82,200);
	idle_translate_x(9);
	idle_translate_x_rotate(10,7,72,200);
	idle_translate_x_rotate(11,8,75,200);
	idle_translate_x_rotate(12,9,72,200);
	idle_translate_x_rotate(13,10,68,200);
	idle_translate_x_rotate(14,11,60,200);
	idle_translate_x_rotate(15,12,53,200);
	idle_translate_x_rotate(16,13,38,200);
}