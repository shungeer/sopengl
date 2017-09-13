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
void mousemove_callback(GLFWwindow* window, double xpos, double ypos);
void mousescroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
void DrawContext5();
void DrawContext6();
void DrawContext7();
void DrawContext8();

void(*gFun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int aTotal = 2;
const unsigned int bTotal = 2;
const unsigned int eTotal = 1;
const unsigned int sTotal = 2;
const unsigned int tTotal = 2;

unsigned int gVAOs[aTotal];
unsigned int gVBOs[bTotal];
unsigned int gEBOs[eTotal];
CShader* gShaders[sTotal];
unsigned int gTexs[tTotal];

float gFov = 45.0f;
float gViewWidth = SCR_WIDTH;
float gViewHeight = SCR_HEIGHT;
float gView1 = 0.0f;
float gView2 = 0.0f;
float gView3 = -3.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;
float gPitch = 0.0f;
float gYaw = -90.0f;
float gLastX = SCR_WIDTH / 2;
float gLastY = SCR_HEIGHT/ 2;
bool gFirstMouse = true;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// ���ô��ڲ���ʾ��꣬�ҹ�겻�뿪����

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
	glfwSetCursorPosCallback(window, mousemove_callback);					// ���
	glfwSetScrollCallback(window, mousescroll_callback);

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
	// ����OpenGL
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// ��Ⱦ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	float cameraSpeed = 2.5f * gDeltaTime;
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		gFov += 10.0f;
		if (gFov > 85.0f)
		{
			gFov = 85.0f;
		}
	}
	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		gFov -= 10.0f;
		if (gFov < 5.0f)
		{
			gFov = 5.0f;
		}
	}
	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		gViewWidth += 100.0f; 
		gViewHeight += 100.0f;
		if (gViewWidth > 1600.0f)
		{
			gViewWidth = 1600.0f;
		}
		if (gViewHeight > 1200.0f)
		{
			gViewHeight = 1200.0f;
		}
	}
	else if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		gViewWidth -= 100.0f;
		gViewHeight -= 100.0f;
		if (gViewWidth < 8.0f)
		{
			gViewWidth = 8.0f;
		}
		if (gViewHeight < 6.0f)
		{
			gViewHeight = 6.0f;
		}
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		gView1 += 0.1f;
		if (gView1 > 1.0f)
		{
			gView1 = 1.0f;
		}
	}
	else if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		gView1 -= 0.1f;
		if (gView1 < -1.0f)
		{
			gView1 = -1.0f;
		}
	}
	else if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		gView2 += 0.1f;
		if (gView2 > 1.0f)
		{
			gView2 = 1.0f;
		}
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		gView2 -= 0.1f;
		if (gView2 < -1.0f)
		{
			gView2 = -1.0f;
		}
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		gView3 -= 10.f;
		if (gView3 < -110.0f)
		{
			gView3 = -100.0f;
		}
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		gView3 += 10.f;
		if (gView3 > 0.0f)
		{
			gView3 = -1.0f;
		}
	}
	else if (key == GLFW_KEY_W && GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (key == GLFW_KEY_S && GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		gFun = DrawContext5;
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		gFun = DrawContext6;
	}
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		gFun = DrawContext7;
	}
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		gFun = DrawContext8;
	}
}

// ���
void mousemove_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = (float)xpos;
		gLastY = (float)ypos;
		gFirstMouse = false;
	}
	float offsetX = (float)xpos - gLastX;
	float offsetY = gLastY - (float)ypos;	// y����ײ�����������
	gLastX = (float)xpos;
	gLastY = (float)ypos;

	float sensitivity = 0.05f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	gYaw += offsetX;
	gPitch += offsetY;

	if (gPitch > 89.0f)
	{
		gPitch = 89.0f;
	}
	if (gPitch < -89.0f)
	{
		gPitch = -89.0f;
	}

	glm::vec3 front;
	front.x = (float)cos(glm::radians(gPitch)) * cos(glm::radians(gYaw));
	front.y = (float)sin(glm::radians(gPitch));
	front.z = (float)cos(glm::radians(gPitch)) * sin(glm::radians(gYaw));
	cameraFront = glm::normalize(front);
}

void mousescroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (gFov >= 1.0f && gFov <= 45.0f)
		gFov -= (float)yoffset;
	if (gFov <= 1.0f)
		gFov = 1.0f;
	if (gFov >= 45.0f)
		gFov = 45.0f;
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

	float vertices1[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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

	// vao2
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
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

void DrawContext5()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	glm::mat4 model, view, projection;
	// ģ�;�����ʱ����תΪ������
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
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
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DrawContext6()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// ���������
	// λ��
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};


	gShaders[1]->use();
	// ��VAO
	glBindVertexArray(gVAOs[1]);

	// �趨�任����
	glm::mat4 view, projection;
	// ��ͼ�����������ƽ��3����λ��ʹ����ͶӰ��׶���ڣ�
	view = glm::translate(view, glm::vec3(gView1, gView2, gView3));
	// ͶӰ����
	projection = glm::perspective(glm::radians(gFov), (float)gViewWidth / (float)gViewHeight, 0.1f, 100.0f);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		float angle = 20.0f * i;
		// ģ�;�����ʱ����תΪ������
		model = glm::translate(model, cubePositions[i]);
		if (i % 3 == 0)
		{
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		
		// ������ɫ��
		gShaders[1]->setMat4("model", model);
		gShaders[1]->setMat4("view", view);
		gShaders[1]->setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void DrawContext7()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// ���������
	// λ��
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};


	gShaders[1]->use();
	// ��VAO
	glBindVertexArray(gVAOs[1]);

	// �趨�任����
	glm::mat4 view, projection;
	// ��ͼ�����������ƽ��3����λ��ʹ����ͶӰ��׶���ڣ�
	//view = glm::translate(view, glm::vec3(gView1, gView2, gView3));
	float radius = 10.0f;
	float camX = (float)sin(glfwGetTime()) * radius;
	float camZ = (float)cos(glfwGetTime()) * radius;
	// �����λ�á�Ŀ�ꡢ��ʾ����ռ��е���������
	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// ͶӰ����
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		float angle = 20.0f * i;
		// ģ�;�����ʱ����תΪ������
		model = glm::translate(model, cubePositions[i]);
		if ((i+1) % 2 == 0)
		{
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		

		// ������ɫ��
		gShaders[1]->setMat4("model", model);
		gShaders[1]->setMat4("view", view);
		gShaders[1]->setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void DrawContext8()
{
	// ��������Ԫ���Ұ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// ���������
	// λ��
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};


	gShaders[1]->use();
	// ��VAO
	glBindVertexArray(gVAOs[1]);

	// �趨�任����
	glm::mat4 view, projection;
	// ��ͼ�����������ƽ��3����λ��ʹ����ͶӰ��׶���ڣ�
	//view = glm::translate(view, glm::vec3(gView1, gView2, gView3));

	// �����λ�á�Ŀ�ꡢ��ʾ����ռ��е���������
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	// ͶӰ����
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		float angle = 20.0f * i;
		// ģ�;�����ʱ����תΪ������
		model = glm::translate(model, cubePositions[i]);
		if ((i + 1) % 2 == 0)
		{
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		// ������ɫ��
		gShaders[1]->setMat4("model", model);
		gShaders[1]->setMat4("view", view);
		gShaders[1]->setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	float curFrame = (float)glfwGetTime();
	gDeltaTime = curFrame - gLastFrame;
	gLastFrame = curFrame;
}