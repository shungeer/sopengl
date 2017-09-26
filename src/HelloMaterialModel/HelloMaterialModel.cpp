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

// 相机
CellCamera camera(Vec3<float>(0.0f, 0.0f, 3.0f));

const unsigned int aTotal = 2;
const unsigned int bTotal = 1;
const unsigned int eTotal = 1;
const unsigned int sTotal = 2;
const unsigned int tTotal = 2;

// opengl对象
unsigned int gVAOs[aTotal];
unsigned int gVBOs[bTotal];
unsigned int gEBOs[eTotal];
CShader* gShaders[sTotal];
unsigned int gTexs[tTotal];

int main(int argc, CHAR* argv[])
{
	// 1.GLFW配置
	glfwInit();										// 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// 设置OpenGL主版本号是3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// 设置OpenGL次版本号是3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// 设置OpenGL使用核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// 2.创建窗口，设置主上下文
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);		// 窗口对象
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		// 设置这个窗口的上下文是当前线程的主上下文
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 3.使用GLAD库获取OpenGL的函数正确地址
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 4.设置OpenGL的视口
	// 实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		// 参数说明：左下角位置+宽度+高度

	// 5.注册回调函数（更改视口、处理手柄输入、处理错误消息等）
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// 更改视口
	glfwSetKeyCallback(window, key_callback);							// 键盘输入
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 6.渲染上下文定义
	// 1) 创建着色器
	BuildShader();
	// 2) 创建顶点等对象
	BuildModel();
	// 3) 创建纹理对象
	BuildTexture();
	// 4) 绑定全局的纹理对象
	InitTexture();

	// 7.循环渲染
	gFun = DrawNULLContext;
	// opengl状态
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// 渲染命令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gFun();

		glfwPollEvents();		// 检查触发事件（键盘、鼠标）+更新窗口状态，并相应调用回调函数
		glfwSwapBuffers(window);// 交换颜色缓冲
	}

	// 7.释放窗口相关资源
	ClearContext();
	glfwTerminate();
	return 0;
}

// 当用户改变窗口的大小的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// 按键输入
// 参数说明：key：按下的按键；action：表示按键动作（按下或释放）；mode：表示是否有Ctrl、Shift、Alt、Super等按钮的操作
// 设置不同的键渲染不同的模型
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	static bool sbFill = false;
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{// 判断线框模式
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

// 鼠标输入
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

// 鼠标滚轮输入
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.MouseScroll((float)yoffset);
}

// 创建所有着色器程序
void BuildShader()
{
	gShaders[0] = new CShader("shaders/emerald.vs", "shaders/emerald.fs");
	gShaders[1] = new CShader("shaders/lamp.vs", "shaders/lamp.fs");
}
// 创建所有渲染模型
void BuildModel()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	// 缓存生成
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cube vao
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 指定顶点属性(顶点)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 指定顶点属性(法向量)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑

	// light vao
	glBindVertexArray(gVAOs[1]);
	// 绑定VBO(直接使用cube vbo)
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	// 指定顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑
}
// 创建所有纹理属性
void BuildTexture()
{
	return;
	// 纹理缓存
	glGenTextures(tTotal, gTexs);

	// texture0配置
	// 绑定纹理目标，配置纹理属性
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	// 纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图片数据
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);	// 纹理坐标原点在左下角，图片原点在右上角，需要反转
	unsigned char* data = stbi_load("../../context/resource/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// 绑定图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// 生成指定级别的mipmap
	}
	else
	{
		cout << "Failed to load texture0" << endl;
	}
	// 释放图像数据
	stbi_image_free(data);

	// texture1配置
	// 绑定纹理目标，配置纹理属性
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);
	// 纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图片数据
	stbi_set_flip_vertically_on_load(true);	// 纹理坐标原点在左下角，图片原点在右上角，需要反转
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// 绑定图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// 生成指定级别的mipmap
	}
	else
	{
		cout << "Failed to load texture1" << endl;
	}
	// 释放图像数据
	stbi_image_free(data);
}
// 为着色器纹理变量指定纹理单元
void InitTexture()
{
	return;
	gShaders[0]->use();
	gShaders[0]->setInt("texture1", 0);
	gShaders[0]->setInt("texture2", 1);
}
// 清除渲染上下文
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

// 绿宝石材质
void DrawContext1()
{
	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();
	model.rotate((float)glfwGetTime() * 30.0f, Vec3<float>(0.5f, 1.0f, 0.0f));
	// 变换操作
	gShaders[0]->use();
	gShaders[0]->setVecf3("viewPos", camera._vPosition.x(), camera._vPosition.y(), camera._vPosition.z());
	gShaders[0]->setVecf3("lightPos", lightPos.x(), lightPos.y(), lightPos.z());
	gShaders[0]->setVecf3("objectColor", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setVecf3("lightColor", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setMat4("projection", projection);
	gShaders[0]->setMat4("view", view);
	gShaders[0]->setMat4("model", model);
	gShaders[0]->setVecf3("material.ambient", 0.0215f, 0.1745f, 0.0215f);
	gShaders[0]->setVecf3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
	gShaders[0]->setVecf3("material.specular", 0.633f, 0.727811f, 0.633f);
	gShaders[0]->setFloat("material.shininess", 0.6f);
	gShaders[0]->setVecf3("light.ambient", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setVecf3("light.diffuse", 1.0f, 1.0f, 1.0f); // 将光照调暗了一些以搭配场景
	gShaders[0]->setVecf3("light.specular", 1.0f, 1.0f, 1.0f);

	glBindVertexArray(gVAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// 
	gShaders[1]->use();
	gShaders[1]->setMat4("projection", projection);
	gShaders[1]->setMat4("view", view);
	model.identity();
	model.scale(0.05f);
	lightPos[0] = 1.0f + sin((float)glfwGetTime()) * 2.0f;
	lightPos[1] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	model.translate(lightPos);
	gShaders[1]->setMat4("model", model);
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// 珍珠白材质
void DrawContext2()
{
	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();
	model.rotate((float)glfwGetTime() * 30.0f, Vec3<float>(0.5f, 1.0f, 0.0f));
	// 变换操作
	gShaders[0]->use();
	gShaders[0]->setVecf3("viewPos", camera._vPosition.x(), camera._vPosition.y(), camera._vPosition.z());
	gShaders[0]->setVecf3("lightPos", lightPos.x(), lightPos.y(), lightPos.z());
	gShaders[0]->setVecf3("objectColor", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setVecf3("lightColor", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setMat4("projection", projection);
	gShaders[0]->setMat4("view", view);
	gShaders[0]->setMat4("model", model);
	gShaders[0]->setVecf3("material.ambient", 0.25f, 0.20725f, 0.20725f);
	gShaders[0]->setVecf3("material.diffuse", 1.f, 0.829f, 0.829f);
	gShaders[0]->setVecf3("material.specular", 0.296648f, 0.296648f, 0.296648f);
	gShaders[0]->setFloat("material.shininess", 0.088f);
	gShaders[0]->setVecf3("light.ambient", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setVecf3("light.diffuse", 1.0f, 1.0f, 1.0f); // 将光照调暗了一些以搭配场景
	gShaders[0]->setVecf3("light.specular", 1.0f, 1.0f, 1.0f);

	glBindVertexArray(gVAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// 
	gShaders[1]->use();
	gShaders[1]->setMat4("projection", projection);
	gShaders[1]->setMat4("view", view);
	model.identity();
	model.scale(0.05f);
	lightPos[0] = 1.0f + sin((float)glfwGetTime()) * 2.0f;
	lightPos[1] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	model.translate(lightPos);
	gShaders[1]->setMat4("model", model);
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// 黄金材质
void DrawContext3()
{
	Mat4<float> projection;
	Mat4<float> view;
	Mat4<float> model;
	projection.perspective(camera._fZoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetLookAt();
	model.rotate((float)glfwGetTime() * 30.0f, Vec3<float>(0.5f, 1.0f, 0.0f));
	// 变换操作
	gShaders[0]->use();
	gShaders[0]->setVecf3("viewPos", camera._vPosition.x(), camera._vPosition.y(), camera._vPosition.z());
	gShaders[0]->setVecf3("lightPos", lightPos.x(), lightPos.y(), lightPos.z());
	gShaders[0]->setVecf3("objectColor", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setVecf3("lightColor", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setMat4("projection", projection);
	gShaders[0]->setMat4("view", view);
	gShaders[0]->setMat4("model", model);
	gShaders[0]->setVecf3("material.ambient", 0.24725f, 0.1995f, 0.0745f);
	gShaders[0]->setVecf3("material.diffuse", 0.75164f, 0.60648f, 0.22648f);
	gShaders[0]->setVecf3("material.specular", 0.628281f, 0.555802f, 0.366065f);
	gShaders[0]->setFloat("material.shininess", 0.4f);
	gShaders[0]->setVecf3("light.ambient", 1.0f, 1.0f, 1.0f);
	gShaders[0]->setVecf3("light.diffuse", 1.0f, 1.0f, 1.0f); // 将光照调暗了一些以搭配场景
	gShaders[0]->setVecf3("light.specular", 1.0f, 1.0f, 1.0f);

	glBindVertexArray(gVAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// 
	gShaders[1]->use();
	gShaders[1]->setMat4("projection", projection);
	gShaders[1]->setMat4("view", view);
	model.identity();
	model.scale(0.05f);
	lightPos[0] = 1.0f + sin((float)glfwGetTime()) * 2.0f;
	lightPos[1] = sin((float)glfwGetTime() / 2.0f) * 1.0f;
	model.translate(lightPos);
	gShaders[1]->setMat4("model", model);
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}