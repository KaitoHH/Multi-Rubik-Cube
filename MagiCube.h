#pragma once
#include <windows.h> 
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <list>
#include <string>

//#define SHOW_COD
#define DEFAULT_DIM			3
#define MAX_DIM				max(DEFAULT_DIM, 7)
#define MIN_DIM				2
#define UNIT				1
#define TEXTURES			7
#define ROTATE_SPEED		200

#define SURFACE				6

/**************************************************************************

Class: MagiCube

Author: Huang Hui

Date: 2015-12-27

Description: 一个方便的绘制魔方类，具备任意棱旋转、贴图、视角转换、阶数转换的功能、自动演示功能

Copyright: Huang Hui 2015

Remark: The class is open sourced and has been uploaded in https://github.com/KaitoHH/Magic-Cube/,
        and will be updated IRREGULARLY

**************************************************************************/

namespace MagiCubeSpace
{
	enum rotateType
	{
		XROT, YROT, ZROT
	};
	enum degree
	{
		CLOCKWISE, BOTH, ANTICLOCKWISE
	};
	// 前后左右上下
	enum color
	{
		PUREWHITE, BLUE, GREEN, ORANGE, RED, YELLOW, WHITE
	};

}
using namespace MagiCubeSpace;


struct rotateMsg
{

	rotateType type;
	int offest;
	degree angle;
	int processing;

	rotateMsg(rotateType t = XROT, int off = 0, degree ang = CLOCKWISE, int speed = ROTATE_SPEED)
	{
		type = t;
		offest = off;
		angle = ang;
		processing = speed;
	}
};

class MagiCube
{

public:
	MagiCube();
	~MagiCube();

	// 设置纹理
	bool loadAllTexture(char** filenames);

	// 绘制魔方
	void drawCube();

	// 增加阶数
	bool increaseDimision();

	// 减少阶数
	bool decreaseDimision();

	// 拉进距离
	void decreaseDist();

	// 拉远距离
	void increaseDist();

	// XZ平面旋转
	void rotateX(int offest, degree ang);

	// YZ平面旋转
	void rotateY(int offest, degree ang);

	// XY平面旋转
	void rotateZ(int offest, degree ang);

	// 停止旋转或者复原魔方
	void initCube();

	// 鼠标按下事件，处理旋转
	void mouseDown();

	// 鼠标处理视角相对移动事件
	void mouseMove(int baseX, int baseY, int curX, int curY);

	// 读入自动演示
	bool rotateInStr(const char *str);

	/* 以下是有趣的魔方图案生成函数 */

	// 棋盘
	void stripe_interval();

	// 六面回子
	void stripe_circle();

	// 斜线
	void stripe_dign();

	// 随机旋转一次
	void stripe_ran();

	/* 以上 */

private:

	// 绘制单个立方体(左下角坐标)
	void drawSingleCube(float x, float y, float z, color *c);

	// 内部剔除
	bool isNeedDraw(int x, int y, int z);

	// 是否为外部方块
	bool isInRange(int d);

	// 贴图
	void putTexture(int x, int y, int z, color *c);

	// 重置全局位置
	void drawGlobalSence();

	// 旋转消息处理
	void rotateMsgProcess(rotateMsg *&msg, int x, int y, int z);

	// 旋转剩余的四个面
	void rotateCube(int offest, rotateType);

	// 确定绘制方法
	int drawMethod(int p, rotateType type, int token = 0);

	// 确定旋转平面
	int toPlane(int offest, rotateType type);

	// 顺时针旋转一个面
	void rotatePlane(int p);

	// 解析旋转消息
	bool parse_rotate_msg(const char *str,rotateMsg &);

	int dimension;
	GLuint texture[TEXTURES];				// 存储纹理
	color cube[SURFACE][MAX_DIM][MAX_DIM];	// 存储状态

	// 全局旋转量
	GLfloat xglrot;
	GLfloat yglrot;
	GLfloat zglrot;

	// 视野
	GLfloat dist;

	// 相对旋转量
	GLfloat txglrot;
	GLfloat tyglrot;
	GLfloat tzglrot;

	// 旋转消息旋转量
	GLfloat rot;

	// 旋转消息队列
	std::list<rotateMsg>rotateQueue;

	// 旋转速度
	int rotate_speed;
};
