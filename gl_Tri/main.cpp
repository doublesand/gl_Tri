#include <Angel.h>

#pragma comment(lib, "glew32.lib")

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

//主窗口
GLuint win;
const int width = 512;
const int height = 512;

GLuint menu;
double angle = 0;

void changeCenter(vec2 vertices[], vec2 center) {
	for (int i = 0; i < NUM_POINTS; i++) {
		vertices[i] += center;
	}
}

void changeShape(vec2 vertices[], double rate) {
	for (int i = 0; i < NUM_POINTS; i++) {
		vertices[i] = vertices[i] * rate;
	}
}

void setSqure(vec2 vertices[]) {
	for (int i = 0; i < NUM_POINTS; i++) {
		double currentAngle = (M_PI / 4 + (M_PI / 2 * i)) + angle;
		vertices[i] = vec2(sin(currentAngle), cos(currentAngle)) * 0.5;
	}
}

void init() {
	//一个窗口的宽度是2，左右分别是-1和1，所以超过1的都不能看见
	vec2 vertices[NUM_POINTS];
	setSqure(vertices);
	vec3 colors[NUM_COLORS] = {
		PURPLE, BLUE, RED, ORANGE
	};


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
	// 白色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void idle() {  //空闲回调函数
	angle += M_PI / 180;
	if (angle > M_PI*2) {
		angle - M_PI * 2;
	}
	glutPostRedisplay();
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
	
	glutPostRedisplay();  //做了修改后就进行重新绘制窗口
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
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}