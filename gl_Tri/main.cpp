#include <Angel.h>
#include <iostream>
#pragma comment(lib, "glew32.lib")

using namespace std;

//颜色
const vec3 WHITE(1.0, 1.0, 1.0);
const vec3 BLACK(0.0, 0.0, 0.0);
const vec3 RED(1.0, 0.0, 0.0);
const vec3 GREEN(0.0, 1.0, 0.0);
const vec3 BLUE(0.0, 0.0, 1.0);
const vec3 YELLOW(1.0, 1.0, 0.0);
const vec3 ORANGE(1.0, 0.65, 0.0);
const vec3 PURPLE(0.8, 0.0, 0.8);

//顶点个数和颜色个数
const int NUM_POINTS = 4;
const int NUM_COLORS = 4;
vec2 vertices[NUM_POINTS];
vec3 colors[NUM_COLORS] = {
	PURPLE, BLUE, RED, ORANGE
};

//主窗口
GLuint win;
const int width = 512;
const int height = 512;
GLuint menu;
double angle = 0;

//子窗口
GLuint subWin;
vec2 subVertices[3] = {vec2(-0.75,-0.75), vec2(0,0.75) ,vec2(0.75,-0.75)};
vec3 subColors[3] = {BLUE, BLUE, BLUE};

void setSqure(vec2 vertices[]) {     //改变角度
	for (int i = 0; i < NUM_POINTS; i++) {
		double currentAngle = (M_PI / 4 + (M_PI / 2 * i)) + angle;
		vertices[i] = vec2(sin(currentAngle), cos(currentAngle)) * 0.5;
	}
}

void init() {
	//一个窗口的宽度是2，左右分别是-1和1，所以超过1的都不能看见
	setSqure(vertices);

	//创建顶点数组对象
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 创建并初始化顶点缓存对象
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(colors), NULL, GL_STATIC_DRAW); //向服务器分配内存

	//读取数据到内存中，分别是类型，偏移，大小，数据地址
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// 读取着色器并使用
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// 从顶点着色器中初始化顶点的位置
	GLuint plocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(plocation);
	glVertexAttribPointer(plocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// 从片元着色器中初始化顶点的颜色
	GLuint clocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(clocation);
	glVertexAttribPointer(clocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
	// 黑色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void idle() {  //空闲回调函数
	angle += M_PI / 180;
	if (angle > M_PI*2) {
		angle - M_PI * 2;
	}
	glutPostWindowRedisplay(win);
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		glutIdleFunc(idle);
	}
	else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP){
		glutIdleFunc(NULL);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '\033':
		exit(EXIT_SUCCESS);
		break;
	case 'b':
		colors[0] = BLUE;
		break;
	case 'o':
		colors[0] = ORANGE;
	}
	glutPostWindowRedisplay(win);
}

void menuEvents(int id) {
	switch (id)
	{
	case 1:
		exit(0);
		break;
	case 2:
		glutIdleFunc(idle);
		break;
	case 3:
		glutIdleFunc(NULL);
		break;
	}
	glutPostWindowRedisplay(win);
}

void setupMenu() {
	menu = glutCreateMenu(menuEvents);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("start rotation", 2);
	glutAddMenuEntry("stop rotation", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void display(void)
{
	init();
	// 清理窗口
	glClear(GL_COLOR_BUFFER_BIT);
	// 绘制所有点,GL_TRIANGLE_FAN是 连续画三角形的意思，GL_TRIANGLES是只画一个三角形
	glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_POINTS);
	glutSwapBuffers(); //由于开启了双缓存，所以使用这个函数而不是glFlush
	//glFlush();
}

void subKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'g':
		subColors[0] = GREEN;
		subColors[1] = GREEN;
		subColors[2] = GREEN;
		cout << "进来了，变绿" << endl;
		break;
	case 'b':
		subColors[0] = BLUE;
		subColors[1] = BLUE;
		subColors[2] = BLUE;
		break;
	}
	// 标记subWindow子窗口进行重绘
	glutPostWindowRedisplay(subWin);
}

void subInit() {
	//创建数组对象并绑定
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//创建并初始化顶点缓存对象缓存
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(subVertices) + sizeof(subColors), NULL, GL_STATIC_DRAW);

	// 分别读取数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(subVertices), subVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(subVertices), sizeof(subColors), subColors);

	// 读取着色器并复用
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// 从顶点着色器中初始化顶点的位置
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	// 从片元着色器中初始化顶点的颜色
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(subVertices)));

	// 设置子窗口背景颜色为白色
	glClearColor(1.0, 1.0, 1.0, 1.0);
}



void subDisplay() {
	subInit();    // 重绘时写入新的颜色数据
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);  //通过使用双缓存，使得画面更加柔顺
	glutInitWindowSize(width, height);
	win = glutCreateWindow("红色三角形");

	// 检测是否使用了freeglut，并检测是否使用到了OpenGL 3.3
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glewExperimental = GL_TRUE;
	glewInit();

	init();
	setupMenu();
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	//添加子窗口,以下语义都是给子窗口的
	subWin = glutCreateSubWindow(win, 0, 0, width / 4, height / 4);  
	subInit();
	glutDisplayFunc(subDisplay);
	glutKeyboardFunc(subKeyboard);

	glutMainLoop();
	return 0;
}