// HelloBlend.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


// HelloStencil.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION		// ��֤stb_image�����


#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "CellCamera.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <map>
using std::map;

using namespace std;
using namespace cell;

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
void DrawContext2();
void(*gFun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float gLastX = SCR_WIDTH / 2.0f;
float gLastY = SCR_HEIGHT / 2.0f;
bool gFirstMouse = true;

// timing
float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

// ���
CellCamera camera(Vec3<float>(0.0f, 0.0f, 3.0f));

// opengl����
const unsigned int sTotal = 3;
CShader* gShaders[sTotal];
const unsigned int aTotal = 3;
unsigned int gVAOs[aTotal];
const unsigned int bTotal = 3;
unsigned int gVBOs[bTotal];
const unsigned int tTotal = 4;
unsigned int gTexs[tTotal];

// opengl�������
Vec3<float> gClearColor(0.2f, 0.3f, 0.3f);	// Ĭ��

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
	// tell GLFW to capture our mouse
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	// opengl״̬
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// ��Ⱦ����
		glClearColor(gClearColor.x(), gClearColor.y(), gClearColor.z(), 1.0f);
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
	static bool sbFill = false;
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
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
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.KeybordMove(CellCamera::FORWARD, gDeltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.KeybordMove(CellCamera::BACKWARD, gDeltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.KeybordMove(CellCamera::LEFT, gDeltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.KeybordMove(CellCamera::RIGHT, gDeltaTime);
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gFun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gClearColor.set(0.2f, 0.3f, 0.3f);
		gFun = DrawContext1;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		gClearColor.set(0.4f, 0.3f, 0.5f);
		gFun = DrawContext2;
	}
}

// �������
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

	camera.MouseMove(xoffset, yoffset);
}

// ����������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.MouseScroll((float)yoffset);
}

// ����������ɫ������
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.vs", "shaders/fragment.fs");
	gShaders[1] = new CShader("shaders/blend-grass.vs", "shaders/blend-grass.fs");
	gShaders[2] = new CShader("shaders/blend-window.vs", "shaders/blend-window.fs");
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
	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.5f, 0.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cube vao
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// plane vao
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// transparent vao
	glBindVertexArray(gVAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// ����ͼ��
void LoadTexImage(const char* path, unsigned int texid)
{
	// ����ͼƬ����
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);	// ��������ԭ�������½ǣ�ͼƬԭ�������Ͻǣ���Ҫ��ת
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;

		// ������Ŀ�꣬������������
		glBindTexture(GL_TEXTURE_2D, texid);
		// �����Χ��ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// ������˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// ��ͼ������
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// ����ָ�������mipmap
	}
	else
	{
		cout << "Failed to load texture path:" << path << endl;
	}
	// �ͷ�ͼ������
	stbi_image_free(data);
}
// ����������������
void BuildTexture()
{
	// ������
	glGenTextures(tTotal, gTexs);

	// texture����
	LoadTexImage("../../context/resource/marble.png", gTexs[0]);
	LoadTexImage("../../context/resource/metal.jpg", gTexs[1]);
	LoadTexImage("../../context/resource/grass.png", gTexs[2]);
	LoadTexImage("../../context/resource/blending_transparent_window.png", gTexs[3]);
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

// 
void DrawContext1()
{
	// transparent vegetation locations
	// --------------------------------
	Vec3<float> vegetation[] =
	{
		Vec3<float>(-1.5f, 0.0f, -0.48f),
		Vec3<float>(1.5f, 0.0f, 0.51f),
		Vec3<float>(0.0f, 0.0f, 0.7f),
		Vec3<float>(-0.3f, 0.0f, -2.3f),
		Vec3<float>(0.5f, 0.0f, -0.6f)
	};

	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();

	gShaders[0]->use();
	gShaders[0]->setMatf4("projection", projection);
	gShaders[0]->setMatf4("view", view);

	glActiveTexture(GL_TEXTURE0);

	// ����ʵ��
	// cube1
	glBindVertexArray(gVAOs[0]);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);
	model.identity();
	model.translate(Vec3<float>(-1.0f, 0.f, -1.f));
	gShaders[0]->use();
	gShaders[0]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// cube2
	model.identity();
	model.translate(Vec3<float>(2.0f, 0.f, 0.f));
	gShaders[0]->use();
	gShaders[0]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	// plane
	glBindVertexArray(gVAOs[1]);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	model.identity();
	gShaders[0]->use();
	gShaders[0]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	// vegetation
	gShaders[1]->use();
	gShaders[1]->setMatf4("projection", projection);
	gShaders[1]->setMatf4("view", view);
	glBindVertexArray(gVAOs[2]);
	glBindTexture(GL_TEXTURE_2D, gTexs[2]);
	for (int i = 0; i < 5; i++)
	{
		model.identity();
		model.scale(0.5f);
		model.translate(vegetation[i]);
		gShaders[1]->setMatf4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}/*

	model.identity();
	gShaders[1]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	model.identity();
	model.translate(0.5f, 0.0f, 0.0f);
	model.rotate(90.0f, Vec3<float>(0.0f, 1.0f, 0.0f));
	model.translate(-0.5f, 0.0f, 0.0f);
	gShaders[1]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);*/

	for (int i = 0; i < 5; i++)
	{
		model.identity();
		model.scale(0.5f);
		model.translate(vegetation[i]);

		// ��ת��������ƽ�Ƶ�ԭ�㣬����ת����ƽ�ƻ�ԭλ�ã����������������
		model.translate(0.5f, 0.0f, 0.0f);
		model.rotateY(90.0f);
		model.translate(-0.5f, 0.0f, 0.0f);
		gShaders[1]->setMatf4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);
}

int cmp(const void *a, const void *b)
{
	return (float*)a - (float*)b;
}

void DrawContext2()
{
	// transparent vegetation locations
	// --------------------------------
	Vec3<float> vegetation[] =
	{
		Vec3<float>(-1.5f, 0.0f, -0.48f),
		Vec3<float>(1.5f, 0.0f, 0.51f),
		Vec3<float>(0.0f, 0.0f, 0.7f),
		Vec3<float>(-0.3f, 0.0f, -2.3f),
		Vec3<float>(0.5f, 0.0f, -0.6f)
	};

	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();

	gShaders[0]->use();
	gShaders[0]->setMatf4("projection", projection);
	gShaders[0]->setMatf4("view", view);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);	// ���û��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ����ʵ��
	// cube1
	glBindVertexArray(gVAOs[0]);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);
	model.identity();
	model.translate(Vec3<float>(-1.0f, 0.f, -1.f));
	gShaders[0]->use();
	gShaders[0]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// cube2
	model.identity();
	model.translate(Vec3<float>(2.0f, 0.f, 0.f));
	gShaders[0]->use();
	gShaders[0]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	// plane
	glBindVertexArray(gVAOs[1]);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	model.identity();
	gShaders[0]->use();
	gShaders[0]->setMatf4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	// vegetation
	gShaders[2]->use();
	gShaders[2]->setMatf4("projection", projection);
	gShaders[2]->setMatf4("view", view);
	glBindVertexArray(gVAOs[2]);
	glBindTexture(GL_TEXTURE_2D, gTexs[3]);

	// ���� �������������
	map<float, Vec3<float>> disMap;
	float tmpDis;
	for (int i = 0; i < 5; i++)
	{
		tmpDis = sqrt(fabs(vegetation[i].dot(camera._vPosition)));
		disMap[tmpDis] = vegetation[i];
	}

	map<float, Vec3<float>>::reverse_iterator rit;
	for (rit = disMap.rbegin(); rit != disMap.rend(); rit++)
	{
		model.identity();
		model.translate((*rit).second);
		gShaders[2]->setMatf4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
}
