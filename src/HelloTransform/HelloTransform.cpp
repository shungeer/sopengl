/*#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void BuildShader();
void BuildModel();
void BuildTexture();
void InitTexture();
void ClearContext();
void DrawNULLContext();
void DrawContext1();

void(*gFun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int aTotal = 1;
const unsigned int bTotal = 1;
const unsigned int eTotal = 1;
const unsigned int sTotal = 1;
const unsigned int tTotal = 2;

unsigned int gVAOs[aTotal];
unsigned int gVBOs[bTotal];
unsigned int gEBOs[eTotal];
CShader* gShaders[sTotal];
unsigned int gTexs[tTotal];

int main(int argc, CHAR* argv[])
{
	// 1.GLFW����
	glfwInit();										// ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// ����OpenGL���汾����3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// ����OpenGL�ΰ汾����3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// ����OpenGLʹ�ú���ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		// ����˵�������½�λ��+���+�߶�

	// 5.ע��ص������������ӿڡ������ֱ����롢���������Ϣ�ȣ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// �����ӿ�
	glfwSetKeyCallback(window, key_callback);							// ��������

	// 6.��Ⱦ�����Ķ���
	// 1) ������ɫ��
	BuildShader();
	// 2) ��������ȶ���
	BuildModel();
	// 3) �����������
	BuildTexture();
	// 4) ��ȫ�ֵ��������
	InitTexture();

	// 7.ѭ����Ⱦ
	gFun = DrawNULLContext;
	while (!glfwWindowShouldClose(window))
	{
		// ��Ⱦ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		gFun();

		glm::vec4 vec;

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
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gFun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gFun = DrawContext1;
	}
}

// ����������ɫ������
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
}
// ����������Ⱦģ��
void BuildModel()
{
	float vertices[] = {
		// positions          // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// ��������
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);
	glGenBuffers(eTotal, gEBOs);

	// vao1
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// ָ����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// ���glVertexAttribPointer�󣬱�ʾ�Ѿ��ӵ�ǰ�󶨵�VBO�л�ȡ���ݣ����Խ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);					// ���VAO��������VAO����ʱ����ʹ�ã��Ƚ��
}
// ����������������
void BuildTexture()
{
	// ������
	glGenTextures(tTotal, gTexs);

	// texture0����
	// ������Ŀ�꣬������������
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	// �����Χ��ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ����ͼƬ����
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);	// ��������ԭ�������½ǣ�ͼƬԭ�������Ͻǣ���Ҫ��ת
	unsigned char* data = stbi_load("../../context/resource/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// ��ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// ����ָ�������mipmap
	}
	else
	{
		cout << "Failed to load texture0" << endl;
	}
	// �ͷ�ͼ������
	stbi_image_free(data);

	// texture1����
	// ������Ŀ�꣬������������
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);
	// �����Χ��ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ����ͼƬ����
	stbi_set_flip_vertically_on_load(true);	// ��������ԭ�������½ǣ�ͼƬԭ�������Ͻǣ���Ҫ��ת
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// ��ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// ����ָ�������mipmap
	}
	else
	{
		cout << "Failed to load texture1" << endl;
	}
	// �ͷ�ͼ������
	stbi_image_free(data);
}
// Ϊ��ɫ���������ָ������Ԫ
void InitTexture()
{
	gShaders[0]->use();
	gShaders[0]->setInt("texture1", 0);
	gShaders[0]->setInt("texture2", 1);
}
// �����Ⱦ������
void ClearContext()
{
	glDeleteVertexArrays(aTotal, gVAOs);
	glDeleteBuffers(bTotal, gVBOs);
	glDeleteBuffers(eTotal, gEBOs);
	glDeleteTextures(tTotal, gTexs);

	for (unsigned int i = 0; i < sTotal; i++)
	{
		delete gShaders[i];
	}
}

void DrawNULLContext()
{
	return;
}

void DrawContext1()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// �任����
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);

	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}*/


#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void BuildShader();
void BuildModel();
void BuildTexture();
void InitTexture();
void ClearContext();
void DrawNULLContext();
void DrawContext1();
void DrawContext2();
void DrawContext3();
void DrawContext4();

void(*gFun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int aTotal = 1;
const unsigned int bTotal = 1;
const unsigned int eTotal = 1;
const unsigned int sTotal = 2;
const unsigned int tTotal = 2;

unsigned int gVAOs[aTotal];
unsigned int gVBOs[bTotal];
unsigned int gEBOs[eTotal];
CShader* gShaders[sTotal];
unsigned int gTexs[tTotal];

int main(int argc, CHAR* argv[])
{
	// 1.GLFW����
	glfwInit();										// ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// ����OpenGL���汾����3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// ����OpenGL�ΰ汾����3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// ����OpenGLʹ�ú���ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		// ����˵�������½�λ��+���+�߶�

	// 5.ע��ص������������ӿڡ������ֱ����롢���������Ϣ�ȣ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// �����ӿ�
	glfwSetKeyCallback(window, key_callback);							// ��������

	// 6.��Ⱦ�����Ķ���
	// 1) ������ɫ��
	BuildShader();
	// 2) ��������ȶ���
	BuildModel();
	// 3) �����������
	BuildTexture();
	// 4) ��ȫ�ֵ��������
	InitTexture();

	// 7.ѭ����Ⱦ
	gFun = DrawNULLContext;
	while (!glfwWindowShouldClose(window))
	{
		// ��Ⱦ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		gFun();

		glm::vec4 vec;

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
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gFun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gFun = DrawContext1;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		gFun = DrawContext2;
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		gFun = DrawContext3;
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		gFun = DrawContext4;
	}
}

// ����������ɫ������
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
	gShaders[1] = new CShader("shaders/vertex1.shader", "shaders/fragment.shader");
}
// ����������Ⱦģ��
void BuildModel()
{
	float vertices[] = {
		// positions          // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// ��������
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);
	glGenBuffers(eTotal, gEBOs);

	// vao1
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// ָ����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);
	// ���glVertexAttribPointer�󣬱�ʾ�Ѿ��ӵ�ǰ�󶨵�VBO�л�ȡ���ݣ����Խ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);					// ���VAO��������VAO����ʱ����ʹ�ã��Ƚ��
}
// ����������������
void BuildTexture()
{
	// ������
	glGenTextures(tTotal, gTexs);

	// texture0����
	// ������Ŀ�꣬������������
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	// �����Χ��ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ����ͼƬ����
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);	// ��������ԭ�������½ǣ�ͼƬԭ�������Ͻǣ���Ҫ��ת
	unsigned char* data = stbi_load("../../context/resource/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// ��ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// ����ָ�������mipmap
	}
	else
	{
		cout << "Failed to load texture0" << endl;
	}
	// �ͷ�ͼ������
	stbi_image_free(data);

	// texture1����
	// ������Ŀ�꣬������������
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);
	// �����Χ��ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ����ͼƬ����
	stbi_set_flip_vertically_on_load(true);	// ��������ԭ�������½ǣ�ͼƬԭ�������Ͻǣ���Ҫ��ת
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// ��ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// ����ָ�������mipmap
	}
	else
	{
		cout << "Failed to load texture1" << endl;
	}
	// �ͷ�ͼ������
	stbi_image_free(data);
}
// Ϊ��ɫ���������ָ������Ԫ
void InitTexture()
{
	gShaders[0]->use();
	gShaders[0]->setInt("texture1", 0);
	gShaders[0]->setInt("texture2", 1);

	gShaders[1]->use();
	gShaders[1]->setInt("texture1", 0);
	gShaders[1]->setInt("texture2", 1);
}
// �����Ⱦ������
void ClearContext()
{
	glDeleteVertexArrays(aTotal, gVAOs);
	glDeleteBuffers(bTotal, gVBOs);
	glDeleteBuffers(eTotal, gEBOs);
	glDeleteTextures(tTotal, gTexs);

	for (unsigned int i = 0; i < sTotal; i++)
	{
		delete gShaders[i];
	}
}

void DrawNULLContext()
{
	return;
}

void DrawContext1()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// �任����(T*R*O)
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);

	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext2()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// �任����(R*T*O)
	glm::mat4 transform;
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);

	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext3()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// ��һ��
	// �任����(R*T*O)
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);

	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// �ڶ���
	// �任����(T*S*O)
	float s = (float)sin(glfwGetTime());
	transform = glm::mat4();	// ����Ϊ��λ����
	transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
	transform = glm::scale(transform, glm::vec3(s, s, s));
	
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void DrawContext4()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	glm::mat4 model, view, projection;
	// ģ�;�����ʱ����תΪ������
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// ��ͼ�����������ƽ��3����λ��ʹ����ͶӰ��׶���ڣ�
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	// ͶӰ����
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// ������ɫ��
	gShaders[1]->use();
	gShaders[1]->setMat4("model", model);
	gShaders[1]->setMat4("view", view);
	gShaders[1]->setMat4("projection", projection);

	// ��VAO
	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}