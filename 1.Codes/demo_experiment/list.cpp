#include "../GL/glut.h"

GLuint	box;	
GLuint	top;

void DrawModel(int n,bool issample=false);

GLvoid BuildLists()					// �������ӵ���ʾ�б�

{
	int range=50;
	box=glGenLists(range);				// ����������ʾ�б������
 			
	for (int i=1;i<=range;i++){
	glNewList(box+i,GL_COMPILE);			// ������һ����ʾ�б�
			DrawModel(i);
    glEndList();
	}

}

void DrawList(int i)
{

	glCallList(box+i);	

}