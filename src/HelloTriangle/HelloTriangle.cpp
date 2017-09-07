#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void BuildShaderProgram();
void BuildModel();
void ClearContext();
void DrawNULLContext();
void CreateContext1();
void DrawContext1();
void DrawContext2();

// ��Ⱦ����ָ��
void (*gfun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int vbTotal = 2;
const unsigned int vaTotal = 2;
const unsigned int ebTotal = 1;
const unsigned int spTotal = 1;

unsigned int gVBOs[vbTotal];
unsigned int gVAOs[vaTotal];
unsigned int gEBOs[ebTotal];
unsigned int gShaderPrograms[spTotal];

int main(int argc, CHAR* argv[])
{
	// 1.GLFW����
	glfwInit();										// ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// ����OpenGL���汾����3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// ����OpenGL�ΰ汾����3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// ����OpenGLʹ�ú���ģʽ
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// ����mac osxϵͳȷ���������������õĴ���

	// 2.�������ڣ�������������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);		// ���ڶ���
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		// ����������ڵ��������ǵ�ǰ�̵߳���������

	// 3.ʹ��GLAD���ȡOpenGL�ĺ�����ȷ��ַ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 4.����OpenGL���ӿ�
	// ʵ����Ҳ���Խ��ӿڵ�ά������Ϊ��GLFW��ά��С��������֮�����е�OpenGL��Ⱦ������һ����С�Ĵ�������ʾ�������ӵĻ�����Ҳ���Խ�һЩ����Ԫ����ʾ��OpenGL�ӿ�֮�⡣
	glViewport(0, 0, 800, 600);		// ����˵�������½�λ��+���+�߶�

	// 5.ע��ص������������ӿڡ������ֱ����롢���������Ϣ�ȣ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// �����ӿ�
	glfwSetKeyCallback(window, key_callback);							// ��������

	// 6.��Ⱦ�����Ķ���
	// 1) ������ɫ��
	BuildShaderProgram();
	// 2) ��������ȶ���
	BuildModel();

	// 7.ѭ����Ⱦ
	gfun = DrawNULLContext;
	while (!glfwWindowShouldClose(window))
	{
		// ��Ⱦ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		gfun();		// ����

		glfwPollEvents();		// ��鴥���¼������̡���꣩+���´���״̬������Ӧ���ûص�����
		glfwSwapBuffers(window);// ������ɫ����
	}

	// 7.�ͷŴ��������Դ
	ClearContext();
	glfwTerminate();
	return 0;
}

// ���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// ��������
// ����˵����key�����µİ�����action����ʾ�������������»��ͷţ���mode����ʾ�Ƿ���Ctrl��Shift��Alt��Super�Ȱ�ť�Ĳ���
// ���ò�ͬ�ļ���Ⱦ��ͬ��ģ��
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	static bool sbFill = false;
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{// �ж��߿�ģʽ
		if (sbFill)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		sbFill = !sbFill;
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gfun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gfun = DrawContext1;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		gfun = DrawContext2;
	}
}

// ����������ɫ������
void BuildShaderProgram()
{
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	// ������ɫ��
	// ������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);		// ������ɫ������
	glCompileShader(vertexShader);									// ������ɫ������
	// �����ɫ���Ƿ����ɹ�
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ƬԪ��ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ���ӵ�һ��ʹ�ö�����ɫ����Ⱦ�ĳ���
	gShaderPrograms[0] = glCreateProgram();
	glAttachShader(gShaderPrograms[0], vertexShader);	// �����ɫ���������Ϊ��һ�����������ɫ��������
	glAttachShader(gShaderPrograms[0], fragmentShader);
	glLinkProgram(gShaderPrograms[0]);
	// �����ɫ���Ƿ����ӳɹ�
	glGetProgramiv(gShaderPrograms[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ��ɫ�����ӳɹ��󣬿���ɾ�������ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// ����������Ⱦģ��
void BuildModel()
{
	// ��Ⱦ����
	float vertices1[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float vertices2[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	unsigned int indices2[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	// ����OpenGL����GPU�ڴ�ķ�ʽ
	// ����OpenGL����
	glGenBuffers(vbTotal, gVBOs);		// ����VBO�������
	glGenVertexArrays(vaTotal, gVAOs);	// VAO����
	glGenBuffers(ebTotal, gEBOs);		// EBO����
	// �Ȱ�VAO����VAO���浱ǰ���ã�
	glBindVertexArray(gVAOs[0]);
	// ���ƶ������鵽�����й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);	// ΪVBOָ������Ŀ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);	// �������ݵ���ǰ�󶨵�VBO����
	// glVertexAttribPointer������󶨵�GL_ARRAY_BUFFER��VBO�л�ȡ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// ���ý������嶥��ķ�ʽ
	glEnableVertexAttribArray(0);		// ���ö������ԣ��Զ�������λ����Ϊ������
	// �����glVertexAttribPointer�󣬱�ʾ�Ѿ��ӵ�ǰ�󶨵�VBO�л�ȡ���ݣ����Խ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);					// ���VAO��������VAO����ʱ����ʹ�ã��Ƚ��

	// �ظ���������
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);	// ָ��EBO�Ļ���Ŀ������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);
}

// �����Ⱦ������
void ClearContext()
{
	glDeleteVertexArrays(vaTotal, gVAOs);
	glDeleteBuffers(vbTotal, gVBOs);
	glDeleteBuffers(ebTotal, gEBOs);
	for (unsigned int i = 0; i < spTotal; i++)
	{
		glDeleteProgram(gShaderPrograms[i]);
	}
}

void DrawNULLContext()
{
	return;
}

void DrawContext1()
{
	// ����
	glUseProgram(gShaderPrograms[0]);		// ������ɫ��
	glBindVertexArray(gVAOs[0]);			// ��ʹ�õ�VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// OpenGL���ƺ���
	glBindVertexArray(0);				// ���VAOs
}

void DrawContext2()
{
	glUseProgram(gShaderPrograms[0]);
	glBindVertexArray(gVAOs[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



