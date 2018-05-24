// HelloRTT.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "CellCamera.h"

#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void BuildShader();
void InitShader();
void BuildModel();
void BuildTexture();
void BuildRTT();
void ClearContext();

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

const unsigned int sTotal = 2;
CShader* gShaders[sTotal];

const unsigned int aTotal = 3;
unsigned int gVAOs[aTotal];

const unsigned int bTotal = 3;
unsigned int gVBOs[bTotal];

const unsigned int tTotal = 2;
unsigned int gTexs[tTotal];

const unsigned int fTotal = 1;
unsigned int gFBs[fTotal];

const unsigned int rttTotal = 1;
unsigned int gRTTs[rttTotal];

const unsigned int rboTotal = 1;
unsigned int gRBOs[rboTotal];

int _tmain(int argc, _TCHAR* argv[])
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

	// 3.����ϵͳ����
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// ע��ص������������ӿڡ������ֱ����롢���������Ϣ�ȣ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// �����ӿ�
	glfwSetKeyCallback(window, key_callback);							// ��������
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 4.ʹ��GLAD���ȡOpenGL�ĺ�����ȷ��ַ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 5.��Ⱦ�����Ķ���
	// 1) ������ɫ��
	BuildShader();
	// 2) ��������ȶ���
	BuildModel();
	// 3) �����������
	BuildTexture();
	// 4) �󶨳�ʼ����ɫ��
	InitShader();
	// 5) ������Ⱦ������֡����
	BuildRTT();

	//// 4.����OpenGL���ӿ�
	//// ʵ����Ҳ���Խ��ӿڵ�ά������Ϊ��GLFW��ά��С��������֮�����е�OpenGL��Ⱦ������һ����С�Ĵ�������ʾ�������ӵĻ�����Ҳ���Խ�һЩ����Ԫ����ʾ��OpenGL�ӿ�֮�⡣
	//// glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		// ����˵�������½�λ��+���+�߶�

	// 6.����opengl��ʼȫ��״̬
	glEnable(GL_DEPTH_TEST);

	// 7.��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// 1) ��ȡһ����Ⱦѭ�����ĵ�ʱ��
		float currentFrame = (float)glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// 2) ���������豸Ӱ��
		glfwPollEvents();		// ��鴥���¼������̡���꣩+���´���״̬������Ӧ���ûص�����

		// 3) ����Ҫʹ�õ�֡�������
		glBindFramebuffer(GL_FRAMEBUFFER, gFBs[0]);
		glEnable(GL_DEPTH_TEST);	 // ��Ϊ����ر�����Ȳ���

		// 4) ��Ⱦ����ǰ�󶨵�֡�������
		// ����ջ�������
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ����ɫ����Ⱦ
		Mat4<float> projection;
		Mat4<float> view;
		Mat4<float> model;
		projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetLookAt();

		gShaders[0]->use();
		gShaders[0]->setMat4("projection", projection);
		gShaders[0]->setMat4("view", view);

		// cube
		glBindVertexArray(gVAOs[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gTexs[0]);
		model.translate(Vec3<float>(-1.0f, 0.0f, -1.0f));
		gShaders[0]->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model.identity();
		model.translate(Vec3<float>(2.0f, 0.0f, 0.0f));
		gShaders[0]->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// plane
		glBindVertexArray(gVAOs[1]);
		glBindTexture(GL_TEXTURE_2D, gTexs[1]);
		model.identity();
		gShaders[0]->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ����Ĭ��vao
		glBindVertexArray(0);

		// 5) ���֡������󣬻ָ�Ĭ��
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 6) ��������Ⱦ�õ���֡����������Ⱦ��ָ���Ķ������
		glDisable(GL_DEPTH_TEST); // ��ָ֤���Ķ���β�����Ϊ��Ȳ��Ա��ü���
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		// 7) ��Ⱦָ���Ķ����
		gShaders[1]->use();
		glBindVertexArray(gVAOs[2]);
		glBindTexture(GL_TEXTURE_2D, gRTTs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 8) ����ϵͳ����
		glfwSwapBuffers(window);
		// glfwPollEvents();
	}

	// �����Դ
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
	gShaders[0] = new CShader("shaders/scene.vs", "shaders/scene.fs");
	gShaders[1] = new CShader("shaders/screen.vs", "shaders/screen.fs");
}

// ��ɫ����ʼ������
void InitShader()
{
	gShaders[0]->use();
	gShaders[0]->setInt("objTex", 0);

	gShaders[1]->use();
	gShaders[1]->setInt("screenTex", 0);
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
		// positions          // texture Coords 
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	//float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	//	// positions   // texCoords
	//	0.75f, -0.75f, 0.0f, 1.0f,
	//	0.75f, -1.0f, 0.0f, 0.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f,

	//	0.75f, -0.75f, 0.0f, 1.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f,
	//	1.0f, -0.75f, 1.0f, 1.0f
	//};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	// ��������
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cube vao
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	// ָ����������(����)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ָ����������(��������)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// plane vao
	glBindVertexArray(gVAOs[1]);
	// ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	// ָ����������(����)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ָ����������(��������)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// screen vao
	glBindVertexArray(gVAOs[2]);
	// ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// ָ����������(����)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ָ����������(��������)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ���glVertexAttribPointer�󣬱�ʾ�Ѿ��ӵ�ǰ�󶨵�VBO�л�ȡ���ݣ����Խ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);					// ���VAO��������VAO����ʱ����ʹ�ã��Ƚ��
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	// texture0����
	LoadTexImage("../../context/resource/marble.png", gTexs[0]);
	LoadTexImage("../../context/resource/metal.jpg", gTexs[1]);
}

// ����֡����
void BuildRTT()
{
	// ����֡����
	glGenFramebuffers(fTotal, gFBs);
	glBindFramebuffer(GL_FRAMEBUFFER, gFBs[0]);
	// ����һ����ɫ����
	glGenTextures(rttTotal, gRTTs);
	glBindTexture(GL_TEXTURE_2D, gRTTs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gRTTs[0], 0);
	// ����һ����Ⱦ�������
	glGenRenderbuffers(rboTotal, gRBOs);
	glBindRenderbuffer(GL_RENDERBUFFER, gRBOs[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBOs[0]); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// ��ΪĬ��ֵ
}

// �����Ⱦ������
void ClearContext()
{
	glDeleteFramebuffers(fTotal, gFBs);
	glDeleteTextures(rttTotal, gRTTs);
	glDeleteRenderbuffers(rboTotal, gRBOs);
	glDeleteVertexArrays(aTotal, gVAOs);
	glDeleteBuffers(bTotal, gVBOs);
	glDeleteTextures(tTotal, gTexs);

	for (unsigned int i = 0; i < sTotal; i++)
	{
		delete gShaders[i];
	}
}