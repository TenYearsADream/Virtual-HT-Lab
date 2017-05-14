
#include "../GL/glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define BMP_Header_Length 54

//第二段代码是我们的重点。它包括两个函数。其中power_of_two比较简单，虽然实现手段有点奇特，但也并非无法理解（即使真的无法理解，读者也可以给出自己的解决方案，用一些循环以及多使用一些位操作也没关系。反正，这里不是重点啦）。另一个load_texture函数却是重头戏：打开BMP文件、读取其中的高度和宽度信息、计算像素数据所占的字节数、为像素数据分配空间、读取像素数据、对像素图象进行缩放（如果必要的话）、分配新的纹理编号、填写纹理参数、载入纹理，所有的功能都在同一个函数里面完成了。为了叙述方便，我把所有的解释都放在了注释里。
/* 函数power_of_two
* 检查一个整数是否为2的整数次方，如果是，返回1，否则返回0
* 实际上只要查看其二进制位中有多少个，如果正好有1个，返回1，否则返回0
* 在“查看其二进制位中有多少个”时使用了一个小技巧
* 使用n &= (n-1)可以使得n中的减少一个（具体原理大家可以自己思考）
*/
int power_of_two(int n)
{
    if( n <= 0 )
        return 0;
    return (n & (n-1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
     GLint width, height, total_bytes;
     GLubyte* pixels = 0;
     GLuint texture_ID = 0;
	 GLint  last_texture_ID;
     // 打开文件，如果失败，返回
     FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;

     // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

     // 计算每行像素所占字节数，并根据此数据计算总像素字节数
     {
         GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
             ++line_bytes;
         total_bytes = line_bytes * height;
     }

     // 根据总像素字节数分配内存
     pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
     {
        fclose(pFile);
        return 0;
     }

     // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 )
     {
        free(pixels);
        fclose(pFile);
        return 0;
     }

     // 在旧版本的OpenGL中
     // 如果图象的宽度和高度不是的整数次方，则需要进行缩放
     // 这里并没有检查OpenGL版本，出于对版本兼容性的考虑，按旧版本处理
     // 另外，无论是旧版本还是新版本，
     // 当图象的宽度和高度超过当前OpenGL实现所支持的最大值时，也要进行缩放
     {
         GLint max;
         glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width)
          || !power_of_two(height)
          || width > max
          || height > max )
         {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
             GLint new_line_bytes, new_total_bytes;
             GLubyte* new_pixels = 0;

             // 计算每行需要的字节数和总字节数
             new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                 ++new_line_bytes;
             new_total_bytes = new_line_bytes * new_height;

             // 分配内存
             new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
             {
                free(pixels);
                fclose(pFile);
                return 0;
             }

             // 进行像素缩放
             gluScaleImage(GL_RGB,
                 width, height, GL_UNSIGNED_BYTE, pixels,
                 new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

             // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
             pixels = new_pixels;
             width = new_width;
             height = new_height;
         }
     }

     // 分配一个新的纹理编号
     glGenTextures(1, &texture_ID);
    if( texture_ID == 0 )
     {
        free(pixels);
        fclose(pFile);
        return 0;
     }

     // 绑定新的纹理，载入纹理并设置纹理参数
     // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
     glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
     glBindTexture(GL_TEXTURE_2D, texture_ID);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
         GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
     glBindTexture(GL_TEXTURE_2D, last_texture_ID);

     // 之前为pixels分配的内存可在使用glTexImage2D以后释放
     // 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）
    free(pixels);
    return texture_ID;
}
//第三段代码是关于显示的部分，以及main函数。注意，我们只在main函数中读取了两幅纹理，并把它们保存在各自的纹理对象中，以后就再也不载入纹理。每次绘制时使用glBindTexture在不同的纹理对象中切换。另外，我们使用了超过1.0的纹理坐标，由于GL_TEXTURE_WRAP_S和GL_TEXTURE_WRAP_T参数都被设置为GL_REPEAT，所以得到的效果就是纹理像素的重复，有点向地板砖的花纹那样。读者可以试着修改“墙”的纹理坐标，将5.0修改为10.0，看看效果有什么变化。
/* 两个纹理对象的编号


GLuint texGround;
GLuint texWall;




     glBindTexture(GL_TEXTURE_2D, texGround);
     glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);
         glTexCoord2f(0.0f, 5.0f); glVertex3f(-8.0f, 8.0f, 0.0f);
         glTexCoord2f(5.0f, 5.0f); glVertex3f(8.0f, 8.0f, 0.0f);load_texture("C:/Users/czq/Desktop/1.bmp");
         glTexCoord2f(5.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);
     glEnd();

     texGround = load_texture("ground.bmp");
     texWall = load_texture("wall.bmp");

	 */
extern GLuint texPart;
extern GLuint texInfo;
extern GLuint texTool;

char partnum='1';
int step_pre_part,step_pre_info,step_pre_tool;

void part_tex(int step)
{
	char c[3];
	sprintf(c,"%d",step);
	char part[40]="C:/Users/czq/Desktop/pics/Part/";
    char *b =".bmp";
	strcat(part,c);
	strcat(part,b);
	const char *texpartadd=part;
    if ((texPart ==0)||(step_pre_part!=step))
	    texPart = load_texture(texpartadd);
	step_pre_part = step;
}

void Info_tex(int step)
{
	char c[3];
	sprintf(c,"%d",step);
	char info[40]="C:/Users/czq/Desktop/pics/Info/";
    char *b =".bmp";
	strcat(info,c);
	strcat(info,b);
	const char *texinfoadd=info;
    if ((texInfo ==0)||(step_pre_info!=step))
	    texInfo = load_texture(texinfoadd);
	step_pre_info = step;
}

void Tool_tex(int step)
{
	char c[3];
	sprintf(c,"%d",step);
	char tool[40]="C:/Users/czq/Desktop/pics/Tool/";
    char *b =".bmp";
	strcat(tool,c);
	strcat(tool,b);
	const char *textooladd=tool;
    if ((texTool ==0)||(step_pre_tool!=step))
	    texTool = load_texture(textooladd);
	step_pre_tool = step;
}