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
void BuildModel();
void BuildTexture();
void InitTexture();
void ClearContext();
void DrawNULLContext();
void DrawContext1();
void DrawContext2();
void DrawContext3();

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

// lighting
Vec3<float> lightPos(1.2f, 1.0f, 0.8f);

// ���
CellCamera camera(Vec3<float>(0.0f, 0.0f, 3.0f));

const unsigned int aTotal = 2;
const unsigned int bTotal = 1;
const unsigned int eTotal = 1;
const unsigned int sTotal = 4;
const unsigned int tTotal = 2;

// opengl����
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
	gShaders[0] = new CShader("shaders/lamp.vs", "shaders/lamp.fs");
	gShaders[1] = new CShader("shaders/color-dir.vs", "shaders/color-dir.fs");
	gShaders[2] = new CShader("shaders/color-att.vs", "shaders/color-att.fs");
	gShaders[3] = new CShader("shaders/color-spot.vs", "shaders/color-spot.fs");
}
// ����������Ⱦģ��
void BuildModel()
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// ��������
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cube vao
	// �󶨶���Ŀ�������
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ָ����������(����)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ָ����������(������)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ָ����������(��������)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// ���VBO
	glBindVertexArray(0);					// ���VAO��������VAO����ʱ����ʹ�ã��Ƚ��

	// light vao
	glBindVertexArray(gVAOs[1]);
	// ��VBO(ֱ��ʹ��cube vbo)
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	// ָ����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
	LoadTexImage("../../context/resource/container2.png", gTexs[0]);
	LoadTexImage("../../context/resource/container2_specular.png", gTexs[1]);
}
// Ϊ��ɫ���������ָ������Ԫ
void InitTexture()
{
	gShaders[1]->use();
	gShaders[1]->setInt("material.diffuse", 0);
	gShaders[1]->setInt("material.specular", 1);

	gShaders[2]->use();
	gShaders[2]->setInt("material.diffuse", 0);
	gShaders[2]->setInt("material.specular", 1);

	gShaders[3]->use();
	gShaders[3]->setInt("material.diffuse", 0);
	gShaders[3]->setInt("material.specular", 1);
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

// �����
void DrawContext1()
{
	// ��������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();
	//model.rotate((float)glfwGetTime() * 30.0f, Vec3<float>(0.5f, 1.0f, 0.0f));
	// �任����
	gShaders[1]->use();
	gShaders[1]->setVecf3("viewPos", camera._vPosition.x(), camera._vPosition.y(), camera._vPosition.z());
	gShaders[1]->setVecf3("lightDir", -0.2f, -1.0f, -0.3f);
	gShaders[1]->setMat4("projection", projection);
	gShaders[1]->setMat4("view", view);
	gShaders[1]->setFloat("material.shininess", 64);
	gShaders[1]->setVecf3("light.ambient", 0.2f, 0.2f, 0.2f);
	gShaders[1]->setVecf3("light.diffuse", 0.5f, 0.50f, 0.5f); // �����յ�����һЩ�Դ��䳡��
	gShaders[1]->setVecf3("light.specular", 1.0f, 1.0f, 1.0f);

	Vec3<float> cubePositions[] = {
		Vec3<float>(0.0f, 0.0f, 0.0f),
		Vec3<float>(2.0f, 5.0f, -15.0f),
		Vec3<float>(-1.5f, -2.2f, -2.5f),
		Vec3<float>(-3.8f, -2.0f, -12.3f),
		Vec3<float>(2.4f, -0.4f, -3.5f),
		Vec3<float>(-1.7f, 3.0f, -7.5f),
		Vec3<float>(1.3f, -2.0f, -2.5f),
		Vec3<float>(1.5f, 2.0f, -2.5f),
		Vec3<float>(1.5f, 0.2f, -1.5f),
		Vec3<float>(-1.3f, 1.0f, -1.5f)
	};

	glBindVertexArray(gVAOs[0]);
	for (unsigned int i = 0; i < 10; i++)
	{
		model.identity();
		model.translate(cubePositions[i]);
		float angle = 20.0f * i;
		model.rotate(angle, Vec3<float>(1.0f, 0.3f, 0.5f));
		gShaders[1]->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);

	//// ��Դ
	//gShaders[0]->use();
	//gShaders[0]->setMat4("projection", projection);
	//gShaders[0]->setMat4("view", view);
	//model.identity();
	//model.scale(0.05f);
	//lightPos[0] = 1.0f + sin((float)glfwGetTime()) * 2.0f;
	//lightPos[1] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	//model.translate(lightPos);
	//gShaders[0]->setMat4("model", model);
	//glBindVertexArray(gVAOs[1]);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
}

// ���
void DrawContext2()
{
	// ��������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();
	//model.rotate((float)glfwGetTime() * 30.0f, Vec3<float>(0.5f, 1.0f, 0.0f));
	// �任����
	gShaders[2]->use();
	gShaders[2]->setVecf3("viewPos", camera._vPosition.x(), camera._vPosition.y(), camera._vPosition.z());
	gShaders[2]->setVecf3("lightPos", lightPos.x(), lightPos.y(), lightPos.z());
	gShaders[2]->setMat4("projection", projection);
	gShaders[2]->setMat4("view", view);
	gShaders[2]->setFloat("material.shininess", 64);
	gShaders[2]->setVecf3("light.ambient", 0.2f, 0.2f, 0.2f);
	gShaders[2]->setVecf3("light.diffuse", 0.5f, 0.50f, 0.5f); // �����յ�����һЩ�Դ��䳡��
	gShaders[2]->setVecf3("light.specular", 1.0f, 1.0f, 1.0f);
	gShaders[2]->setFloat("light.constant", 1.0f);
	gShaders[2]->setFloat("light.linear", 0.09f);
	gShaders[2]->setFloat("light.quadratic", 0.032f);

	Vec3<float> cubePositions[] = {
		Vec3<float>(0.0f, 0.0f, 0.0f),
		Vec3<float>(2.0f, 5.0f, -15.0f),
		Vec3<float>(-1.5f, -2.2f, -2.5f),
		Vec3<float>(-3.8f, -2.0f, -12.3f),
		Vec3<float>(2.4f, -0.4f, -3.5f),
		Vec3<float>(-1.7f, 3.0f, -7.5f),
		Vec3<float>(1.3f, -2.0f, -2.5f),
		Vec3<float>(1.5f, 2.0f, -2.5f),
		Vec3<float>(1.5f, 0.2f, -1.5f),
		Vec3<float>(-1.3f, 1.0f, -1.5f)
	};

	glBindVertexArray(gVAOs[0]);
	for (unsigned int i = 0; i < 10; i++)
	{
		model.identity();
		model.translate(cubePositions[i]);
		float angle = 20.0f * i;
		model.rotate(angle, Vec3<float>(1.0f, 0.3f, 0.5f));
		gShaders[2]->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);

	// ��Դ
	gShaders[0]->use();
	gShaders[0]->setMat4("projection", projection);
	gShaders[0]->setMat4("view", view);
	model.identity();
	model.scale(0.05f);
	lightPos[0] = 1.0f + sin((float)glfwGetTime()) * 2.0f;
	lightPos[1] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	lightPos[2] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	model.translate(lightPos);
	gShaders[0]->setMat4("model", model);
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// �۹�
void DrawContext3()
{
	// ��������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();
	//model.rotate((float)glfwGetTime() * 30.0f, Vec3<float>(0.5f, 1.0f, 0.0f));
	// �任����
	gShaders[3]->use();
	gShaders[3]->setVecf3("viewPos", camera._vPosition.x(), camera._vPosition.y(), camera._vPosition.z());
	gShaders[3]->setVecf3("spot.position", lightPos.x(), lightPos.y(), lightPos.z());
	gShaders[3]->setVecf3("spot.direction", lightPos.x(), lightPos.y(), lightPos.z());
	gShaders[3]->setFloat("spot.cutOff", cos(DegreesToRadians(12.5f)));
	gShaders[3]->setFloat("spot.outCutOff", cos(DegreesToRadians(25.0f)));
	gShaders[3]->setMat4("projection", projection);
	gShaders[3]->setMat4("view", view);
	gShaders[3]->setFloat("material.shininess", 64);
	gShaders[3]->setVecf3("light.ambient", 0.2f, 0.2f, 0.2f);
	gShaders[3]->setVecf3("light.diffuse", 0.5f, 0.50f, 0.5f); // �����յ�����һЩ�Դ��䳡��
	gShaders[3]->setVecf3("light.specular", 1.0f, 1.0f, 1.0f);
	gShaders[3]->setFloat("light.constant", 1.0f);
	gShaders[3]->setFloat("light.linear", 0.09f);
	gShaders[3]->setFloat("light.quadratic", 0.032f);

	Vec3<float> cubePositions[] = {
		Vec3<float>(0.0f, 0.0f, 0.0f),
		Vec3<float>(2.0f, 5.0f, -15.0f),
		Vec3<float>(-1.5f, -2.2f, -2.5f),
		Vec3<float>(-3.8f, -2.0f, -12.3f),
		Vec3<float>(2.4f, -0.4f, -3.5f),
		Vec3<float>(-1.7f, 3.0f, -7.5f),
		Vec3<float>(1.3f, -2.0f, -2.5f),
		Vec3<float>(1.5f, 2.0f, -2.5f),
		Vec3<float>(1.5f, 0.2f, -1.5f),
		Vec3<float>(-1.3f, 1.0f, -1.5f)
	};

	glBindVertexArray(gVAOs[0]);
	for (unsigned int i = 0; i < 10; i++)
	{
		model.identity();
		model.translate(cubePositions[i]);
		float angle = 20.0f * i;
		model.rotate(angle, Vec3<float>(1.0f, 0.3f, 0.5f));
		gShaders[3]->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);

	// ��Դ
	gShaders[0]->use();
	gShaders[0]->setMat4("projection", projection);
	gShaders[0]->setMat4("view", view);
	model.identity();
	model.scale(0.05f);
	lightPos[0] = 1.0f + sin((float)glfwGetTime()) * 2.0f;
	lightPos[1] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	lightPos[2] = sin((float)glfwGetTime() / 2.0f) * 3.0f;
	model.translate(lightPos);
	gShaders[0]->setMat4("model", model);
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
