#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

const unsigned int aTotal = 2;
const unsigned int bTotal = 2;
const unsigned int sTotal = 1;
const unsigned int tTotal = 2;

unsigned int gVAOs[aTotal];
unsigned int gVBOs[bTotal];
CShader* gShaders[sTotal];
unsigned int gTexs[tTotal];

// ȫ�����
Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));	// �����λ�ó�ʼ��
float gLastX = (float)SCR_WIDTH / 2.0f;
float gLastY = (float)SCR_HEIGHT / 2.0f;
bool gFirstMouse = true;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	glfwSetCursorPosCallback(window, mouse_callback);					// ��껬��
	glfwSetScrollCallback(window, scroll_callback);						// ������

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
	// ����OpenGLѡ��
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	while (!glfwWindowShouldClose(window))
	{
		// ��Ⱦ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gFun();

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
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCamera.KeybordMove(Camera::FORWARD, gDeltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCamera.KeybordMove(Camera::BACKWARD, gDeltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCamera.KeybordMove(Camera::LEFT, gDeltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCamera.KeybordMove(Camera::RIGHT, gDeltaTime);
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gFun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gFun = DrawContext1;
	}
	gCamera._vPosition.y = 0.5f;	// ��xzƽ��ƽ��
}

// ����ƶ�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = (float)xpos;
		gLastY = (float)ypos;
		gFirstMouse = false;
	}

	float xoffset = (float)xpos - gLastX;
	float yoffset = gLastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	gLastX = (float)xpos;
	gLastY = (float)ypos;

	gCamera.MouseMove(xoffset, yoffset);
}
// ������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gCamera.MouseScroll((float)yoffset);
}

// ����������ɫ������
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
}
// ����������Ⱦģ��
void BuildModel()
{
	float cubeVertices[] = {
		// positions          // texture Coords
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
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	// ��������
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cubevao
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// ָ����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);
	// ���glVertexAttribPointer�󣬱�ʾ�Ѿ��ӵ�ǰ�󶨵�VBO�л�ȡ���ݣ����Խ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);					// ���VAO��������VAO����ʱ����ʹ�ã��Ƚ��

	// planevao
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
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
	unsigned char* data = stbi_load("../../context/resource/marble.png", &width, &height, &nrChannels, 0);
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
	data = stbi_load("../../context/resource/metal.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// ��ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
}
// �����Ⱦ������
void ClearContext()
{
	glDeleteVertexArrays(aTotal, gVAOs);
	glDeleteBuffers(bTotal, gVBOs);
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
	gShaders[0]->use();
	// �趨�任����
	glm::mat4 model, view, projection;
	// ��ͼ����
	// view = gCamera.GetLookAt();// glm��lookat����
	view = gCamera.CalculateLookAt();	// �ֶ������lookat����
	// ͶӰ����
	projection = glm::perspective(glm::radians(gCamera._fZoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	gShaders[0]->setMat4("view", view);
	gShaders[0]->setMat4("projection", projection);
	// cubes
	glBindVertexArray(gVAOs[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	gShaders[0]->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	gShaders[0]->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// floor
	glBindVertexArray(gVAOs[1]);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	gShaders[0]->setMat4("model", glm::mat4());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	float currentFrame = (float)glfwGetTime();
	gDeltaTime = currentFrame - gLastFrame;
	gLastFrame = currentFrame;
}