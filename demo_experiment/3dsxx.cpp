
#include <iostream>
#include "../GL/glut.h"
#include "../GL/glaux.h"
#include "../GL/3DS.H"
#include <vector>
using namespace std;
#pragma  comment(lib,"../GL/glaux.lib")

//#define FILE_NAME    "LUGIA.3ds"//"woman.3DS"//	"����.3ds"	"���췢��̨.3ds"	"�γ�0.3ds"   "�ɻ�1.3ds"      "FACE.3DS"							
#define MAX_TEXTURES 100								// ����������Ŀ
#define TEXT_POSITION "../../../Texture/"

UINT g_Texture[MAX_TEXTURES] = {0};						

CLoad3DS g_Load3ds[50];									
t3DModel g_3DModel[50];									

int   g_ViewMode	  = GL_TRIANGLES;

struct coordinatef{
	float x;
	float y;
	float z;
};
coordinatef coordinate_now;
vector<coordinatef> sample_coordinate;

//  ���ļ��д�������
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// ����޴��ļ�����ֱ�ӷ���
		return;
	
	char address_head[50]={0};
	strcat(address_head,TEXT_POSITION);
	strcat(address_head,strFileName);
	cout<<address_head;
	pBitmap = auxDIBImageLoadA(address_head);				// װ��λͼ������������
	
	if(pBitmap == NULL)									// ���װ��λͼʧ�ܣ����˳�
		exit(0);
	
	// ��������
	glGenTextures(1, &textureArray[textureID]);
	
	// �������ض����ʽ
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	if (pBitmap)										// �ͷ�λͼռ�õ���Դ
	{
		if (pBitmap->data)						
		{
			free(pBitmap->data);				
		}
		
		free(pBitmap);					
	}
}


void myinit_model(int n,char *FILE_NAME)
{
//	static int numOfTex=1;
	g_Load3ds[n].Import3DS(&g_3DModel[n], FILE_NAME);			// ��3ds�ļ�װ�뵽ģ�ͽṹ����

	// �������еĲ���
	for(int i = 0; i < g_3DModel[n].numOfMaterials; i++)
	{
		// �ж��Ƿ���һ���ļ���
		if(strlen(g_3DModel[n].pMaterials[i].strFile) > 0)
		{
			//  ʹ�������ļ�������װ��λͼ
			CreateTexture(g_Texture, g_3DModel[n].pMaterials[i].strFile, i);			
		}
		
		// ���ò��ʵ�����ID
		g_3DModel[n].pMaterials[i].texureId = i;
	}
	
}


	//glRotatef(angle,0,1,0);
	//glTranslatef(0,0,-50);
void DrawModel(int n,bool issample=false)
{
	for(int i = 0; i < g_3DModel[n].numOfObjects; i++)
	{
		// �������Ĵ�СС��0�����˳�
		if(g_3DModel[n].pObject.size() <= 0) break;
		
		// ��õ�ǰ��ʾ�Ķ���
		t3DObject *pObject = &g_3DModel[n].pObject[i];
		
		// �жϸö����Ƿ�������ӳ��
		if(pObject->bHasTexture) {
			
			// ������ӳ��
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			cout<<"ID:"<<pObject->materialID<<'\n';
			glBindTexture(GL_TEXTURE_2D,n);// g_Texture[pObject->materialID]);//ȡn����ʱ�������***************************************
		} 
		else {
			
			// �ر�����ӳ��
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		// ��ʼ��g_ViewModeģʽ����
		glBegin(g_ViewMode);					
		// �������е���
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			// ���������ε����е�
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// ������ÿ���������
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				// ����������
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				// ��������������
				if(pObject->bHasTexture) {
					
					// ȷ���Ƿ���UVW��������
					if(pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {
					
					if(g_3DModel[n].pMaterials.size() && pObject->materialID >= 0) 
					{
						BYTE *pColor = g_3DModel[n].pMaterials[pObject->materialID].color;
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				
				//�����������ģ�ͣ����Ի�ȡ�������꣬��δʹ��
				if(issample){
					coordinate_now.x=pObject->pVerts[ index ].x;
					coordinate_now.y=pObject->pVerts[ index ].y;
					coordinate_now.z=pObject->pVerts[ index ].z;

					sample_coordinate.push_back(coordinate_now);
				}
			}
		}
		
		glEnd();								// ���ƽ���

		
	}

}
