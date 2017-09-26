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
	while (!glfwWindowShouldClose(window))
	{
		// 渲染命令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		gFun();

		glm::vec4 vec;

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
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
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

// 创建所有着色器程序
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
}
// 创建所有渲染模型
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

	// 缓存生成
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);
	glGenBuffers(eTotal, gEBOs);

	// vao1
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 指定顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// 完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑
}
// 创建所有纹理属性
void BuildTexture()
{
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

void DrawContext1()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 变换操作
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// 设置窗口不显示鼠标，且光标不离开窗口

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
	glfwSetCursorPosCallback(window, mousemove_callback);					// 鼠标
	glfwSetScrollCallback(window, mousescroll_callback);

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
	// 开启OpenGL
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// 渲染命令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gFun();

		glm::vec4 vec;

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
	float cameraSpeed = 2.5f * gDeltaTime;
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
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

// 鼠标
void mousemove_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = (float)xpos;
		gLastY = (float)ypos;
		gFirstMouse = false;
	}
	float offsetX = (float)xpos - gLastX;
	float offsetY = gLastY - (float)ypos;	// y坐标底部到顶部增大
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

// 创建所有着色器程序
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
	gShaders[1] = new CShader("shaders/vertex1.shader", "shaders/fragment.shader");
}
// 创建所有渲染模型
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

	// 缓存生成
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);
	glGenBuffers(eTotal, gEBOs);

	// vao1
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 指定顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);
	// 完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑

	// vao2
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// 指定顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);
	// 完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑
}
// 创建所有纹理属性
void BuildTexture()
{
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
	gShaders[0]->use();
	gShaders[0]->setInt("texture1", 0);
	gShaders[0]->setInt("texture2", 1);

	gShaders[1]->use();
	gShaders[1]->setInt("texture1", 0);
	gShaders[1]->setInt("texture2", 1);
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

void DrawContext1()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 变换操作(T*R*O)
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
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 变换操作(R*T*O)
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
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 第一块
	// 变换操作(R*T*O)
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);

	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// 第二块
	// 变换操作(T*S*O)
	float s = (float)sin(glfwGetTime());
	transform = glm::mat4();	// 重置为单位矩阵
	transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
	transform = glm::scale(transform, glm::vec3(s, s, s));
	
	gShaders[0]->use();
	gShaders[0]->setMat4("transform", transform);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void DrawContext4()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	glm::mat4 model, view, projection;
	// 模型矩阵（逆时针旋转为正方向）
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 视图矩阵（整体向后平移3个单位，使得在投影视锥体内）
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	// 投影矩阵
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// 设置着色器
	gShaders[1]->use();
	gShaders[1]->setMat4("model", model);
	gShaders[1]->setMat4("view", view);
	gShaders[1]->setMat4("projection", projection);

	// 绑定VAO
	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext5()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	glm::mat4 model, view, projection;
	// 模型矩阵（逆时针旋转为正方向）
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	// 视图矩阵（整体向后平移3个单位，使得在投影视锥体内）
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	// 投影矩阵
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// 设置着色器
	gShaders[1]->use();
	gShaders[1]->setMat4("model", model);
	gShaders[1]->setMat4("view", view);
	gShaders[1]->setMat4("projection", projection);

	// 绑定VAO
	glBindVertexArray(gVAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DrawContext6()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 多个立方体
	// 位置
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
	// 绑定VAO
	glBindVertexArray(gVAOs[1]);

	// 设定变换矩阵
	glm::mat4 view, projection;
	// 视图矩阵（整体向后平移3个单位，使得在投影视锥体内）
	view = glm::translate(view, glm::vec3(gView1, gView2, gView3));
	// 投影矩阵
	projection = glm::perspective(glm::radians(gFov), (float)gViewWidth / (float)gViewHeight, 0.1f, 100.0f);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		float angle = 20.0f * i;
		// 模型矩阵（逆时针旋转为正方向）
		model = glm::translate(model, cubePositions[i]);
		if (i % 3 == 0)
		{
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		
		// 设置着色器
		gShaders[1]->setMat4("model", model);
		gShaders[1]->setMat4("view", view);
		gShaders[1]->setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void DrawContext7()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 多个立方体
	// 位置
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
	// 绑定VAO
	glBindVertexArray(gVAOs[1]);

	// 设定变换矩阵
	glm::mat4 view, projection;
	// 视图矩阵（整体向后平移3个单位，使得在投影视锥体内）
	//view = glm::translate(view, glm::vec3(gView1, gView2, gView3));
	float radius = 10.0f;
	float camX = (float)sin(glfwGetTime()) * radius;
	float camY = (float)sin(glfwGetTime()) * radius;
	float camZ = (float)cos(glfwGetTime()) * radius;
	// 摄像机位置、目标、表示世界空间中的向上向量
	view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// 投影矩阵
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		float angle = 20.0f * i;
		// 模型矩阵（逆时针旋转为正方向）
		model = glm::translate(model, cubePositions[i]);
		if ((i+1) % 2 == 0)
		{
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		

		// 设置着色器
		gShaders[1]->setMat4("model", model);
		gShaders[1]->setMat4("view", view);
		gShaders[1]->setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void DrawContext8()
{
	// 激活纹理单元并且绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);

	// 多个立方体
	// 位置
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
	// 绑定VAO
	glBindVertexArray(gVAOs[1]);

	// 设定变换矩阵
	glm::mat4 view, projection;
	// 视图矩阵（整体向后平移3个单位，使得在投影视锥体内）
	//view = glm::translate(view, glm::vec3(gView1, gView2, gView3));

	// 摄像机位置、目标、表示世界空间中的向上向量
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	// 投影矩阵
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		float angle = 20.0f * i;
		// 模型矩阵（逆时针旋转为正方向）
		model = glm::translate(model, cubePositions[i]);
		if ((i + 1) % 2 == 0)
		{
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		// 设置着色器
		gShaders[1]->setMat4("model", model);
		gShaders[1]->setMat4("view", view);
		gShaders[1]->setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	float curFrame = (float)glfwGetTime();
	gDeltaTime = curFrame - gLastFrame;
	gLastFrame = curFrame;
}