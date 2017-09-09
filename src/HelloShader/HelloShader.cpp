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
void DrawContext3();
void DrawContext4();
void DrawContext5();
void DrawContext6();

// ��Ⱦ����ָ��
void(*gfun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int vbTotal = 6;
const unsigned int vaTotal = 6;
const unsigned int ebTotal = 1;
const unsigned int spTotal = 4;

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
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		gfun = DrawContext3;
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		gfun = DrawContext4;
	}
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		gfun = DrawContext5;
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		gfun = DrawContext6;
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
	const char *fragmentShaderSource1 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);\n"
		"}\n\0";
	const char *fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor;\n"
		"}\n\0";
	const char *vertexShaderSource1 =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\0";
	const char *fragmentShaderSource3 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, 1.0);\n"
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
	// ������ɫ��1
	unsigned int vertexShader1;
	vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader1, 1, &vertexShaderSource1, NULL);		// ������ɫ������
	glCompileShader(vertexShader1);									// ������ɫ������
	// �����ɫ���Ƿ����ɹ�
	glGetShaderiv(vertexShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ƬԪ��ɫ��1
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ƬԪ��ɫ��2
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ƬԪ��ɫ��3
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ƬԪ��ɫ��4
	unsigned int fragmentShader3;
	fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
	glCompileShader(fragmentShader3);
	glGetShaderiv(fragmentShader3, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader3, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG3::COMPILATION_FAILED\n" << infoLog << std::endl;
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
	// ���ӵ�һ��ʹ�ö�����ɫ����Ⱦ�ĳ���2
	gShaderPrograms[1] = glCreateProgram();
	glAttachShader(gShaderPrograms[1], vertexShader);	// �����ɫ���������Ϊ��һ�����������ɫ��������
	glAttachShader(gShaderPrograms[1], fragmentShader1);
	glLinkProgram(gShaderPrograms[1]);
	// �����ɫ���Ƿ����ӳɹ�
	glGetProgramiv(gShaderPrograms[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ���ӵ�һ��ʹ�ö�����ɫ����Ⱦ�ĳ���3
	gShaderPrograms[2] = glCreateProgram();
	glAttachShader(gShaderPrograms[2], vertexShader);	// �����ɫ���������Ϊ��һ�����������ɫ��������
	glAttachShader(gShaderPrograms[2], fragmentShader2);
	glLinkProgram(gShaderPrograms[2]);
	// �����ɫ���Ƿ����ӳɹ�
	glGetProgramiv(gShaderPrograms[2], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[2], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ���ӵ�һ��ʹ�ö�����ɫ����Ⱦ�ĳ���4
	gShaderPrograms[3] = glCreateProgram();
	glAttachShader(gShaderPrograms[3], vertexShader1);	// �����ɫ���������Ϊ��һ�����������ɫ��������
	glAttachShader(gShaderPrograms[3], fragmentShader3);
	glLinkProgram(gShaderPrograms[3]);
	// �����ɫ���Ƿ����ӳɹ�
	glGetProgramiv(gShaderPrograms[3], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[3], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM3::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ��ɫ�����ӳɹ��󣬿���ɾ�������ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader1);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);
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

	float vertices3[] =
	{
		-0.5f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	float vertices41[] =
	{
		-1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float vertices42[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		1.0f, 0.5f, 0.0f
	};

	float vertices5[] =
	{
		// λ��              // ��ɫ
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // ����
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // ����
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

	// �ظ���������(�ı���)
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);	// ָ��EBO�Ļ���Ŀ������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);

	// �������������Σ�GLDrawArray
	glBindVertexArray(gVAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);

	// ������������
	glBindVertexArray(gVAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices41), vertices41, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);

	glBindVertexArray(gVAOs[4]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices42), vertices42, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);

	// ������ɫ��ͬ��������
	glBindVertexArray(gVAOs[5]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices5), vertices5, GL_STATIC_DRAW);
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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

void DrawContext3()
{
	glUseProgram(gShaderPrograms[0]);
	glBindVertexArray(gVAOs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawContext4()
{
	glUseProgram(gShaderPrograms[0]);
	glBindVertexArray(gVAOs[3]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glUseProgram(gShaderPrograms[1]);
	glBindVertexArray(gVAOs[4]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawContext5()
{
	glUseProgram(gShaderPrograms[2]);
	// ����uniform����������֮ǰ��Ҫ������ɫ����
	// ����uniform��ɫ
	double timeValue = glfwGetTime();
	float redValue = (float)sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(gShaderPrograms[2], "ourColor");
	glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

	glBindVertexArray(gVAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawContext6()
{
	glUseProgram(gShaderPrograms[3]);
	glBindVertexArray(gVAOs[5]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}




