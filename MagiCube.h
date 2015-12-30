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

Description: һ������Ļ���ħ���࣬�߱���������ת����ͼ���ӽ�ת��������ת���Ĺ��ܡ��Զ���ʾ����

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
	// ǰ����������
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

	// ��������
	bool loadAllTexture(char** filenames);

	// ����ħ��
	void drawCube();

	// ���ӽ���
	bool increaseDimision();

	// ���ٽ���
	bool decreaseDimision();

	// ��������
	void decreaseDist();

	// ��Զ����
	void increaseDist();

	// XZƽ����ת
	void rotateX(int offest, degree ang);

	// YZƽ����ת
	void rotateY(int offest, degree ang);

	// XYƽ����ת
	void rotateZ(int offest, degree ang);

	// ֹͣ��ת���߸�ԭħ��
	void initCube();

	// ��갴���¼���������ת
	void mouseDown();

	// ��괦���ӽ�����ƶ��¼�
	void mouseMove(int baseX, int baseY, int curX, int curY);

	// �����Զ���ʾ
	bool rotateInStr(const char *str);

	/* ��������Ȥ��ħ��ͼ�����ɺ��� */

	// ����
	void stripe_interval();

	// �������
	void stripe_circle();

	// б��
	void stripe_dign();

	// �����תһ��
	void stripe_ran();

	/* ���� */

private:

	// ���Ƶ���������(���½�����)
	void drawSingleCube(float x, float y, float z, color *c);

	// �ڲ��޳�
	bool isNeedDraw(int x, int y, int z);

	// �Ƿ�Ϊ�ⲿ����
	bool isInRange(int d);

	// ��ͼ
	void putTexture(int x, int y, int z, color *c);

	// ����ȫ��λ��
	void drawGlobalSence();

	// ��ת��Ϣ����
	void rotateMsgProcess(rotateMsg *&msg, int x, int y, int z);

	// ��תʣ����ĸ���
	void rotateCube(int offest, rotateType);

	// ȷ�����Ʒ���
	int drawMethod(int p, rotateType type, int token = 0);

	// ȷ����תƽ��
	int toPlane(int offest, rotateType type);

	// ˳ʱ����תһ����
	void rotatePlane(int p);

	// ������ת��Ϣ
	bool parse_rotate_msg(const char *str,rotateMsg &);

	int dimension;
	GLuint texture[TEXTURES];				// �洢����
	color cube[SURFACE][MAX_DIM][MAX_DIM];	// �洢״̬

	// ȫ����ת��
	GLfloat xglrot;
	GLfloat yglrot;
	GLfloat zglrot;

	// ��Ұ
	GLfloat dist;

	// �����ת��
	GLfloat txglrot;
	GLfloat tyglrot;
	GLfloat tzglrot;

	// ��ת��Ϣ��ת��
	GLfloat rot;

	// ��ת��Ϣ����
	std::list<rotateMsg>rotateQueue;

	// ��ת�ٶ�
	int rotate_speed;
};
