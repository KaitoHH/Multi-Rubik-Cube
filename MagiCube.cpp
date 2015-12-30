#include "MagiCube.h"
#include <cmath>
#include <ctime>
#include <cstring>
#include <sstream>
int draw_martix[SURFACE][4][3] = {
	{ { 0,1, 0 },{ 1,1, 0 },{ 1,0, 0 },{ 0,0, 0 } },//前
	{ { 0,1,-1 },{ 1,1,-1 },{ 1,0,-1 },{ 0,0,-1 } },//后
	{ { 0,1,-1 },{ 0,1, 0 },{ 0,0, 0 },{ 0,0,-1 } },//左
	{ { 1,1,-1 },{ 1,1, 0 },{ 1,0, 0 },{ 1,0,-1 } },//右
	{ { 0,1,-1 },{ 1,1,-1 },{ 1,1, 0 },{ 0,1, 0 } },//上
	{ { 0,0,-1 },{ 1,0,-1 },{ 1,0, 0 },{ 0,0, 0 } },//下
};
int texture_martix[4][2] = { { 0,1 },{ 1,1 },{ 1,0 },{ 0,0 } };
int rotate_martix[3][3] = { {0,1,0},{1,0,0},{0,0,1} };
int rotate_angle[3] = { 90,180,-90 };
int rotate_cube_matrix[3][4] = { {0,3,1,2},{0,5,1,4},{3,4,2,5} };
int rotate_plane[6] = { 5,4,2,3,0,1 };
bool reverse_cord[SURFACE] = { false,true,true,false,false,true };
bool cube_draw;
int draw_method[3][6] = {
	{0,1,1,0,0,1},
	{2,3,3,2,2,3},
	{2,3,3,2,0,1}
};
MagiCube::MagiCube()
{
	dimension = DEFAULT_DIM;
	xglrot = 30;
	yglrot = -30;
	initCube();
	srand(time(0));
}


MagiCube::~MagiCube()
{
}

// 设置纹理
bool MagiCube::loadAllTexture(char** filenames)
{
	AUX_RGBImageRec *TextureImage[TEXTURES];			// 创建纹理的存储空间
	TextureImage[0] = NULL;
	for (int i = 0; i < TEXTURES; i++) {
		if (TextureImage[i] = auxDIBImageLoad(filenames[i])) {
			glGenTextures(1, &texture[i]);					// 创建纹理
			glBindTexture(GL_TEXTURE_2D, texture[i]);		// 使用来自位图数据生成 的典型纹理

			//生成纹理											
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);

			//GL_LINEAR 线性   GL_NEAREST 就近 GL_LINEAR_MIPMAP_NEAREST高质量
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} else {
			return FALSE;
		}
		if (TextureImage[i])							// 纹理是否存在
		{
			if (TextureImage[i]->data)					// 纹理图像是否存在
			{
				free(TextureImage[i]->data);			// 释放纹理图像占用的内存
			}
			free(TextureImage[i]);						// 释放图像结构
		}
	}

	return TRUE;
}


// 增加阶数
bool MagiCube::increaseDimision()
{
	bool ret = false;
	int dim = dimension + 1;
	if (dim >= MIN_DIM&&dim <= MAX_DIM) {
		dimension = dim;
		ret = true;
		initCube();
		initCube();
	}
	return ret;
}

// 减少阶数
bool MagiCube::decreaseDimision()
{
	bool ret = false;
	int dim = dimension - 1;
	if (dim >= MIN_DIM&&dim <= MAX_DIM) {
		dimension = dim;
		ret = true;
		initCube();
		initCube();
	}
	return ret;
}

// 拉近距离
void MagiCube::decreaseDist()
{
	dist++;
}

// 拉远距离
void MagiCube::increaseDist()
{
	dist--;
}

// 绘制魔方
void MagiCube::drawCube()
{
	cube_draw = false;
	float sx = -dimension / 2.0*UNIT;
	float sy = sx;
	float sz = sx + dimension*UNIT;

	color c[SURFACE];

	rotateMsg *cur = NULL;
	if (!rotateQueue.empty()) {
		cur = &rotateQueue.front();
		cur->processing--;
	}

	for (int x = 0; x < dimension; x++) {
		for (int y = 0; y < dimension; y++) {
			for (int z = 0; z < dimension; z++) {
				// 内部方块剔除
				if (isNeedDraw(x, y, z)) {
					drawGlobalSence();
					if (cur != NULL) {
						rotateMsgProcess(cur, x, y, z);
					}
					memset(c, 0, sizeof(c));	//清空纹理
					putTexture(x, y, z, c);		//贴图
					drawSingleCube(sx + x*UNIT, sy + y*UNIT, sz - z*UNIT, c);
				}
			}
		}
	}
}

// 旋转消息处理
void MagiCube::rotateMsgProcess(rotateMsg *&msg, int x, int y, int z)
{
	static int cnt = 0;
	bool rotate = false;
	switch (msg->type) {
		case XROT:
			if (msg->offest == y)
				rotate = true;
			break;
		case YROT:
			if (msg->offest == x)
				rotate = true;
			break;
		case ZROT:
			if (msg->offest == z)
				rotate = true;
			break;
	}

	if (rotate) {
		if (!cube_draw) {
			rot -= 1.0*rotate_angle[msg->angle] / rotate_speed;
			cnt = 1;
			cube_draw = true;
		}
		glRotatef(rot, rotate_martix[msg->type][0],
			rotate_martix[msg->type][1], rotate_martix[msg->type][2]);

		// 本次旋转完成
		if (!msg->processing) {
			for (int i = 0; i <= msg->angle; i++) {
				rotatePlane(toPlane(msg->offest, msg->type));
				rotateCube(msg->offest, msg->type);
			}
			rotateQueue.pop_front();
			msg = NULL;
			rot = 0;
			//Sleep(1000);
		}
	}
}

// 旋转剩余的四个面
void MagiCube::rotateCube(int offest, rotateType type)
{
	color temp[MAX_DIM][MAX_DIM];
	for (int i = 0; i < 4; i++) {
		int p = rotate_cube_matrix[type][i];
		for (int j = 0; j < dimension; j++) {
			switch (drawMethod(p, type)) {
				case 0:
					temp[i][j] = cube[p][j][offest];
					break;
				case 1:
					temp[i][j] = cube[p][dimension - 1 - j][offest];
					break;
				case 2:
					temp[i][j] = cube[p][offest][j];
					break;
				case 3:
					temp[i][j] = cube[p][offest][dimension - 1 - j];
					break;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		int p = rotate_cube_matrix[type][i];
		for (int j = 0; j < dimension; j++) {
			switch (drawMethod(p, type, 1)) {
				case 0:
					cube[p][j][offest] = temp[(i + 1) % 4][j];
					break;
				case 1:
					cube[p][j][offest] = temp[(i + 1) % 4][dimension - 1 - j];
					break;
				case 2:
					cube[p][offest][j] = temp[(i + 1) % 4][j];
					break;
				case 3:
					cube[p][offest][j] = temp[(i + 1) % 4][dimension - 1 - j];
					break;
			}
		}
	}
}

// 确定绘制方法
int MagiCube::drawMethod(int p, rotateType type, int token)
{
	int id = draw_method[type][p];
	if (type == ZROT && token)
		id = id & 1 ? id - 1 : id + 1;
	return id;
}

// 确定旋转平面
int MagiCube::toPlane(int offest, rotateType type)
{
	if (offest == 0 || offest == dimension - 1) {
		return rotate_plane[type * 2 + (offest ? 1 : 0)];
	}
	return -1;
}

// 顺时针旋转一个面
void MagiCube::rotatePlane(int p)
{
	if (!~p)	return;
	color c[MAX_DIM][MAX_DIM];
	for (int x = 0; x < dimension; x++) {
		for (int y = 0; y < dimension; y++) {
			c[x][y] = cube[p][x][y];
		}
	}
	for (int x = 0; x < dimension; x++) {
		for (int y = 0; y < dimension; y++) {
			cube[p][y][dimension - 1 - x] = c[x][y];
		}
	}
}


// 绘制单个立方体
void MagiCube::drawSingleCube(float x, float y, float z, color * c)
{
	for (int i = 0; i < SURFACE; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[c[i]]);
		glBegin(GL_QUADS);
		for (int j = 0; j < 4; j++) {
			int mx = texture_martix[j][0];
			int my = texture_martix[j][1];
			float tx = x + draw_martix[i][j][0] * UNIT;
			float ty = y + draw_martix[i][j][1] * UNIT;
			float tz = z + draw_martix[i][j][2] * UNIT;
			glTexCoord2f(mx, my);  glVertex3f(tx, ty, tz);
		}
		glEnd();
	}
}

// 内部方块剔除
bool MagiCube::isNeedDraw(int x, int y, int z)
{
	return isInRange(x) || isInRange(y) || isInRange(z);
}

// 是否为外部方块
bool MagiCube::isInRange(int d)
{
	return d == 0 || d == dimension - 1;
}

// 贴图
void MagiCube::putTexture(int x, int y, int z, color *c)
{
	if (z == 0) {							//前面
		c[0] = cube[0][x][y];
	} else if (z == dimension - 1) {		//后面
		c[1] = cube[1][x][y];
	}

	if (x == 0) {							//左面
		c[2] = cube[2][z][y];
	} else if (x == dimension - 1) {		//右面
		c[3] = cube[3][z][y];
	}

	if (y == dimension - 1) {				//上面
		c[4] = cube[4][x][z];
	} else if (y == 0) {					//下面
		c[5] = cube[5][x][z];
	}

}

// 重置全局位置
void MagiCube::drawGlobalSence()
{
	glLoadIdentity();
	glTranslatef(0, 0, dist);
	glRotatef(xglrot, 1.0f, 0.0f, 0.0f);						// 绕X轴旋转
	glRotatef(yglrot, 0.0f, 1.0f, 0.0f);						// 绕Y轴旋转
	glRotatef(zglrot, 0.0f, 0.0f, 1.0f);						// 绕Z轴旋转

#ifdef SHOW_COD		
	glBegin(GL_LINES);											// 坐标轴绘制
	glVertex3f(-100000, 0, 0);
	glVertex3f(100000, 00, 0);
	glVertex3f(0, -100000, 0);
	glVertex3f(0, 100000, 0);
	glVertex3f(0, 0, -100000);
	glVertex3f(0, 0, 100000);
	glEnd();
#endif
}

// XZ平面旋转
void MagiCube::rotateX(int offest, degree ang)
{
	rotateQueue.push_back(rotateMsg(XROT, offest, ang, rotate_speed));
}


// YZ平面旋转
void MagiCube::rotateY(int offest, degree ang)
{
	rotateQueue.push_back(rotateMsg(YROT, offest, ang, rotate_speed));
}


// XY平面旋转
void MagiCube::rotateZ(int offest, degree ang)
{
	rotateQueue.push_back(rotateMsg(ZROT, offest, ang, rotate_speed));
}

// 清空旋转队列或复原魔方
void MagiCube::initCube()
{
	if (!rotateQueue.empty()) {
		rotateQueue.clear();
		return;
	}
	rotateQueue.clear();
	for (int i = 0; i < SURFACE; i++) {
		for (int j = 0; j < dimension; j++) {
			for (int k = 0; k < dimension; k++) {
				cube[i][j][k] = color(i + 1);
			}
		}
	}
	dist = -7 - dimension;
	rot = 0;
	rotate_speed = ROTATE_SPEED - 30 * (dimension - DEFAULT_DIM);
}

// 鼠标按下事件，处理旋转
void MagiCube::mouseDown()
{
	txglrot = xglrot;
	tyglrot = yglrot;
	tzglrot = zglrot;
}

void MagiCube::mouseMove(int baseX, int baseY, int curX, int curY)
{
	double fx, fy;
	fx = txglrot + (curY - baseY) / 2.0;
	fy = tyglrot + (curX - baseX) / 2.0;
	if (abs(fx) <= 90)
		xglrot = fx;
	yglrot = fy;
}

// 读入自动演示
bool MagiCube::rotateInStr(const char *str)
{
	std::string cur;
	std::stringstream ss(str);
	while (ss >> cur) {
		rotateMsg msg;
		if (parse_rotate_msg(cur.c_str(), msg)) {
			rotateQueue.push_back(msg);
		} else {
			return false;
		}
	}
	return true;
}

// 解析旋转消息
bool MagiCube::parse_rotate_msg(const char * str, rotateMsg &msg)
{
	int off = 0;
	int cur = 0;
	int len = strlen(str);
	if (isdigit(str[0])) {
		off = str[0] - '0';
		if (off >= dimension || off < 0)
			return false;
		cur++;
	}
	rotateType type;
	switch (str[cur]) {
		case 'R':
			off = dimension - 1 - off;
		case 'L':
			type = YROT;
			break;
		case 'U':
			off = dimension - 1 - off;
		case 'D':
			type = XROT;
			break;
		case 'B':
			off = dimension - 1 - off;
		case 'F':
			type = ZROT;
			break;
		default:
			return false;
	}
	degree angle;
	switch (str[cur + 1]) {
		case '2':
			angle = BOTH;
			break;
		case '\'':
			angle = ANTICLOCKWISE;
			break;
		case '\0':
			angle = CLOCKWISE;
			break;
		default:
			return false;
	}

	if (str[cur] == 'L' || str[cur] == 'D' || str[cur] == 'B')
		angle = degree(ANTICLOCKWISE - angle);

	msg = rotateMsg(type, off, angle, rotate_speed);

	return true;
}

/* 以下是有趣的魔方图案的生成函数 */

// 棋盘
void MagiCube::stripe_interval()
{
	for (int i = 0; i < dimension; i += 2) {
		rotateX(i, BOTH);
	}
	for (int i = 0; i < dimension; i += 2) {
		rotateY(i, BOTH);
	}
	for (int i = 0; i < dimension; i += 2) {
		rotateZ(i, BOTH);

	}
}

// 六面回字
void MagiCube::stripe_circle()
{
	rotateX(dimension - 1, ANTICLOCKWISE);
	rotateX(0, ANTICLOCKWISE);
	rotateZ(0, ANTICLOCKWISE);
	rotateZ(dimension - 1, ANTICLOCKWISE);
	rotateY(0, ANTICLOCKWISE);
	rotateY(dimension - 1, ANTICLOCKWISE);
	rotateX(dimension - 1, ANTICLOCKWISE);
	rotateX(0, ANTICLOCKWISE);
}

// 斜线
void MagiCube::stripe_dign()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < dimension; j += 2) {
			rotateZ(j, j < dimension / 2 + 1 ? CLOCKWISE : ANTICLOCKWISE);
		}
		for (int j = 0; j < dimension; j += 2) {
			rotateY(j, j < dimension / 2 + 1 ? ANTICLOCKWISE : CLOCKWISE);
		}
	}
}

// 随机旋转一次
void MagiCube::stripe_ran()
{
	int r = rand() % 3;
	int s = rand() % dimension;
	int ang = rand() % 3;
	switch (r) {
		case 0:
			rotateX(s, degree(ang));
			break;
		case 1:
			rotateY(s, degree(ang));
			break;
		case 2:
			rotateZ(s, degree(ang));
			break;
	}
}