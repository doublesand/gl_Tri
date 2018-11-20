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
	//一个窗口的宽度是2，左右分别是-1和1，所以超过1的都不能看见
	vec2 vertices[NUM_POINTS] = {
		vec2(-1.5,0),vec2(0.0, 1.0), vec2(1.0, 0), vec2(0.0, -1.0)
	};
	changeCenter(vertices, vec2(0.0, 0.1));
	vec3 colors[NUM_COLORS] = {
		vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0),vec3(0.0, 1.0, 0.0)
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

void display(void)
{
	// 清理窗口
	glClear(GL_COLOR_BUFFER_BIT);
	// 绘制所有点,GL_TRIANGLE_FAN是连续画三角形的意思，GL_TRIANGLES是只画一个三角形
	glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_POINTS);
	glFlush();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	// 检测是否使用了freeglut，并检测是否使用到了OpenGL 3.3
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("红色三角形");

	glewExperimental = GL_TRUE;
	glewInit();

	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}