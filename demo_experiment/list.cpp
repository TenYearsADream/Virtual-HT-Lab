#include "../GL/glut.h"

GLuint	box;	
GLuint	top;

void DrawModel(int n,bool issample=false);

GLvoid BuildLists()					// 创建盒子的显示列表

{
	int range=50;
	box=glGenLists(range);				// 创建两个显示列表的名称
 			
	for (int i=1;i<=range;i++){
	glNewList(box+i,GL_COMPILE);			// 创建第一个显示列表
			DrawModel(i);
    glEndList();
	}

}

void DrawList(int i)
{

	glCallList(box+i);	

}