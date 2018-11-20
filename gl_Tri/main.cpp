#include <Angel.h>

#pragma comment(lib, "glew32.lib")

const int NUM_POINTS = 3;
const int NUM_COLORS = 3;

void init() {
	vec2 vertices[3] = {
		vec2(-0.75,0),vec2(0.0, 0.75), vec2(0.75, 0)
	};
	vec3 colors[3] = {
		vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)
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

void display(void)
{
	// ������
	glClear(GL_COLOR_BUFFER_BIT);
	// �������е�
	glDrawArrays(GL_TRIANGLES, 0, NUM_POINTS);
	glFlush();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	// ����Ƿ�ʹ����freeglut��������Ƿ�ʹ�õ���OpenGL 3.3
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("��ɫ������");

	glewExperimental = GL_TRUE;
	glewInit();

	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}