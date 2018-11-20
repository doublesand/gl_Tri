#include <Angel.h>

#pragma comment(lib, "glew32.lib")

const int NUM_POINTS = 4;
const int NUM_COLORS = 4;

void changeCenter(vec2 vertices[], vec2 center) {
	for (int i = 0; i < NUM_POINTS; i++) {
		vertices[i] += center;
	}
}

void init() {
	//һ�����ڵĿ����2�����ҷֱ���-1��1�����Գ���1�Ķ����ܿ���
	vec2 vertices[NUM_POINTS] = {
		vec2(-1.5,0),vec2(0.0, 1.0), vec2(1.0, 0), vec2(0.0, -1.0)
	};
	changeCenter(vertices, vec2(0.0, 0.1));
	vec3 colors[NUM_COLORS] = {
		vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0),vec3(0.0, 1.0, 0.0)
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
	// �������е�,GL_TRIANGLE_FAN�������������ε���˼��GL_TRIANGLES��ֻ��һ��������
	glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_POINTS);
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