// HelloRTT.cpp : 定义控制台应用程序的入口点。
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

// 相机
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

	// 3.设置系统交互
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 注册回调函数（更改视口、处理手柄输入、处理错误消息等）
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// 更改视口
	glfwSetKeyCallback(window, key_callback);							// 键盘输入
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 4.使用GLAD库获取OpenGL的函数正确地址
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 5.渲染上下文定义
	// 1) 创建着色器
	BuildShader();
	// 2) 创建顶点等对象
	BuildModel();
	// 3) 创建纹理对象
	BuildTexture();
	// 4) 绑定初始化着色器
	InitShader();
	// 5) 创建渲染到纹理帧缓存
	BuildRTT();

	//// 4.设置OpenGL的视口
	//// 实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
	//// glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		// 参数说明：左下角位置+宽度+高度

	// 6.设置opengl初始全局状态
	glEnable(GL_DEPTH_TEST);

	// 7.渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 1) 获取一次渲染循环消耗的时间
		float currentFrame = (float)glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// 2) 处理输入设备影响
		glfwPollEvents();		// 检查触发事件（键盘、鼠标）+更新窗口状态，并相应调用回调函数

		// 3) 绑定需要使用的帧缓存对象
		glBindFramebuffer(GL_FRAMEBUFFER, gFBs[0]);
		glEnable(GL_DEPTH_TEST);	 // 因为后面关闭了深度测试

		// 4) 渲染到当前绑定的帧缓存对象
		// 先清空缓存内容
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 绑定着色器渲染
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

		// 设置默认vao
		glBindVertexArray(0);

		// 5) 解绑帧缓存对象，恢复默认
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 6) 将上述渲染得到的帧缓存纹理渲染到指定的多变形上
		glDisable(GL_DEPTH_TEST); // 保证指定的多变形不会因为深度测试被裁减掉
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		// 7) 渲染指定的多变形
		gShaders[1]->use();
		glBindVertexArray(gVAOs[2]);
		glBindTexture(GL_TEXTURE_2D, gRTTs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 8) 操作系统交互
		glfwSwapBuffers(window);
		// glfwPollEvents();
	}

	// 清空资源
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
	gShaders[0] = new CShader("shaders/scene.vs", "shaders/scene.fs");
	gShaders[1] = new CShader("shaders/screen.vs", "shaders/screen.fs");
}

// 着色器初始化配置
void InitShader()
{
	gShaders[0]->use();
	gShaders[0]->setInt("objTex", 0);

	gShaders[1]->use();
	gShaders[1]->setInt("screenTex", 0);
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

	// 缓存生成
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);

	// cube vao
	// 绑定对象目标和数据
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	// 指定顶点属性(顶点)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 指定顶点属性(纹理坐标)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// plane vao
	glBindVertexArray(gVAOs[1]);
	// 绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	// 指定顶点属性(顶点)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 指定顶点属性(纹理坐标)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// screen vao
	glBindVertexArray(gVAOs[2]);
	// 绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// 指定顶点属性(顶点)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 指定顶点属性(纹理坐标)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑
}

// 加载图像
void LoadTexImage(const char* path, unsigned int texid)
{
	// 加载图片数据
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);	// 纹理坐标原点在左下角，图片原点在右上角，需要反转
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;

		// 绑定纹理目标，配置纹理属性
		glBindTexture(GL_TEXTURE_2D, texid);
		// 纹理包围方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// 纹理过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 绑定图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	// 生成指定级别的mipmap
	}
	else
	{
		cout << "Failed to load texture path:" << path << endl;
	}
	// 释放图像数据
	stbi_image_free(data);
}

// 创建所有纹理属性
void BuildTexture()
{
	// 纹理缓存
	glGenTextures(tTotal, gTexs);

	// texture0配置
	LoadTexImage("../../context/resource/marble.png", gTexs[0]);
	LoadTexImage("../../context/resource/metal.jpg", gTexs[1]);
}

// 创建帧缓存
void BuildRTT()
{
	// 生成帧缓存
	glGenFramebuffers(fTotal, gFBs);
	glBindFramebuffer(GL_FRAMEBUFFER, gFBs[0]);
	// 创建一个颜色附件
	glGenTextures(rttTotal, gRTTs);
	glBindTexture(GL_TEXTURE_2D, gRTTs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gRTTs[0], 0);
	// 创建一个渲染缓存对象
	glGenRenderbuffers(rboTotal, gRBOs);
	glBindRenderbuffer(GL_RENDERBUFFER, gRBOs[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBOs[0]); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// 设为默认值
}

// 清除渲染上下文
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