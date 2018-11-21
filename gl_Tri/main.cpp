#include <Angel.h>

#pragma comment(lib, "glew32.lib")

//��ɫ
const vec3 WHITE(1.0, 1.0, 1.0);
const vec3 BLACK(0.0, 0.0, 0.0);
const vec3 RED(1.0, 0.0, 0.0);
const vec3 GREEN(0.0, 1.0, 0.0);
const vec3 BLUE(0.0, 0.0, 1.0);
const vec3 YELLOW(1.0, 1.0, 0.0);
const vec3 ORANGE(1.0, 0.65, 0.0);
const vec3 PURPLE(0.8, 0.0, 0.8);

//�����������ɫ����
const int NUM_POINTS = 4;
const int NUM_COLORS = 4;

//������
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
	//һ�����ڵĿ����2�����ҷֱ���-1��1�����Գ���1�Ķ����ܿ���
	vec2 vertices[NUM_POINTS];
	setSqure(vertices);
	vec3 colors[NUM_COLORS] = {
		PURPLE, BLUE, RED, ORANGE
	};


	//���������������
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// ��������ʼ�����㻺�����
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(colors), NULL, GL_STATIC_DRAW); //������������ڴ�

	//��ȡ���ݵ��ڴ��У��ֱ������ͣ�ƫ�ƣ���С�����ݵ�ַ
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// ��ȡ��ɫ����ʹ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// �Ӷ�����ɫ���г�ʼ�������λ��
	GLuint plocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(plocation);
	glVertexAttribPointer(plocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// ��ƬԪ��ɫ���г�ʼ���������ɫ
	GLuint clocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(clocation);
	glVertexAttribPointer(clocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
	// ��ɫ����
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void idle() {  //���лص�����
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
	
	glutPostRedisplay();  //�����޸ĺ�ͽ������»��ƴ���
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
	// ������
	glClear(GL_COLOR_BUFFER_BIT);
	// �������е�,GL_TRIANGLE_FAN�� �����������ε���˼��GL_TRIANGLES��ֻ��һ��������
	glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_POINTS);
	glutSwapBuffers(); //���ڿ�����˫���棬����ʹ���������������glFlush
	//glFlush();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);  //ͨ��ʹ��˫���棬ʹ�û��������˳
	glutInitWindowSize(width, height);
	win = glutCreateWindow("��ɫ������");

	// ����Ƿ�ʹ����freeglut��������Ƿ�ʹ�õ���OpenGL 3.3
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