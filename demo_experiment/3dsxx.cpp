
#include <iostream>
#include "../GL/glut.h"
#include "../GL/glaux.h"
#include "../GL/3DS.H"
#include <vector>
using namespace std;
#pragma  comment(lib,"../GL/glaux.lib")

//#define FILE_NAME    "LUGIA.3ds"//"woman.3DS"//	"铲车.3ds"	"航天发射台.3ds"	"轿车0.3ds"   "飞机1.3ds"      "FACE.3DS"							
#define MAX_TEXTURES 100								// 最大的纹理数目
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

//  从文件中创建纹理
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// 如果无此文件，则直接返回
		return;
	
	char address_head[50]={0};
	strcat(address_head,TEXT_POSITION);
	strcat(address_head,strFileName);
	cout<<address_head;
	pBitmap = auxDIBImageLoadA(address_head);				// 装入位图，并保存数据
	
	if(pBitmap == NULL)									// 如果装入位图失败，则退出
		exit(0);
	
	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);
	
	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	if (pBitmap)										// 释放位图占用的资源
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
	g_Load3ds[n].Import3DS(&g_3DModel[n], FILE_NAME);			// 将3ds文件装入到模型结构体中

	// 遍历所有的材质
	for(int i = 0; i < g_3DModel[n].numOfMaterials; i++)
	{
		// 判断是否是一个文件名
		if(strlen(g_3DModel[n].pMaterials[i].strFile) > 0)
		{
			//  使用纹理文件名称来装入位图
			CreateTexture(g_Texture, g_3DModel[n].pMaterials[i].strFile, i);			
		}
		
		// 设置材质的纹理ID
		g_3DModel[n].pMaterials[i].texureId = i;
	}
	
}


	//glRotatef(angle,0,1,0);
	//glTranslatef(0,0,-50);
void DrawModel(int n,bool issample=false)
{
	for(int i = 0; i < g_3DModel[n].numOfObjects; i++)
	{
		// 如果对象的大小小于0，则退出
		if(g_3DModel[n].pObject.size() <= 0) break;
		
		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel[n].pObject[i];
		
		// 判断该对象是否有纹理映射
		if(pObject->bHasTexture) {
			
			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			cout<<"ID:"<<pObject->materialID<<'\n';
			glBindTexture(GL_TEXTURE_2D,n);// g_Texture[pObject->materialID]);//取n可暂时解决问题***************************************
		} 
		else {
			
			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		// 开始以g_ViewMode模式绘制
		glBegin(g_ViewMode);					
		// 遍历所有的面
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			// 遍历三角形的所有点
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// 获得面对每个点的索引
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				// 给出法向量
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				// 如果对象具有纹理
				if(pObject->bHasTexture) {
					
					// 确定是否有UVW纹理坐标
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
				
				//如果导入试样模型，可以获取点云坐标，暂未使用
				if(issample){
					coordinate_now.x=pObject->pVerts[ index ].x;
					coordinate_now.y=pObject->pVerts[ index ].y;
					coordinate_now.z=pObject->pVerts[ index ].z;

					sample_coordinate.push_back(coordinate_now);
				}
			}
		}
		
		glEnd();								// 绘制结束

		
	}

}
