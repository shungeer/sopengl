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

// 全局相机
Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));	// 以相机位置初始化
float gLastX = (float)SCR_WIDTH / 2.0f;
float gLastY = (float)SCR_HEIGHT / 2.0f;
bool gFirstMouse = true;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

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
	glfwSetCursorPosCallback(window, mouse_callback);					// 鼠标滑动
	glfwSetScrollCallback(window, scroll_callback);						// 鼠标滚动

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
	// 开启OpenGL选项
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	while (!glfwWindowShouldClose(window))
	{
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
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
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
	gCamera._vPosition.y = 0.5f;	// 与xz平面平行
}

// 鼠标移动
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
// 鼠标滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gCamera.MouseScroll((float)yoffset);
}

// 创建所有着色器程序
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
}
// 创建所有渲染模型
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

	// 缓存生成
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cubevao
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// 指定顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);
	// 完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑

	// planevao
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
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
	unsigned char* data = stbi_load("../../context/resource/marble.png", &width, &height, &nrChannels, 0);
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
	data = stbi_load("../../context/resource/metal.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// 绑定图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
}
// 清除渲染上下文
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
	// 设定变换矩阵
	glm::mat4 model, view, projection;
	// 视图矩阵
	// view = gCamera.GetLookAt();// glm的lookat矩阵
	view = gCamera.CalculateLookAt();	// 手动计算的lookat矩阵
	// 投影矩阵
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