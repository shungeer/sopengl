#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

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
void DrawContext5();
void DrawContext6();
void DrawContext7();

void(*gFun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int sTotal = 4;
const unsigned int bTotal = 3;
const unsigned int aTotal = 3;
const unsigned int eTotal = 3;
const unsigned int tTotal = 5;

CShader *gShaders[sTotal];
unsigned int gVBOs[bTotal];
unsigned int gVAOs[aTotal];
unsigned int gEBOs[eTotal];
unsigned int gTexs[tTotal];
float gMixDegree = 0.1f;

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
	// 3) 创建所有纹理属性
	BuildTexture();
	// 4) 初始化全局纹理
	InitTexture();

	// 7.循环渲染
	gFun = DrawNULLContext;
	while (!glfwWindowShouldClose(window))
	{
		// 渲染命令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		gMixDegree += 0.1f;
		if (gMixDegree > 1.0f)
		{
			gMixDegree = 1.0f;
		}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		gMixDegree -= 0.1f;
		if (gMixDegree < 0.0f)
		{
			gMixDegree = 0.0f;
		}
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
}

// 创建所有着色器程序
void BuildShader()
{
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
	gShaders[1] = new CShader("shaders/vertex.shader", "shaders/fragment1.shader");
	gShaders[2] = new CShader("shaders/vertex.shader", "shaders/fragment2.shader");
	gShaders[3] = new CShader("shaders/vertex.shader", "shaders/fragment3.shader");
}
// 创建所有渲染模型
void BuildModel()
{
	float vertices1[] = 
	{
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
	};
	unsigned int indices1[] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	float vertices11[] = 
	{
		// positions          // colors           // texture coords (note that we changed them to 2.0f!)
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f  // top left 
	};
	float vertices12[] = {
		// positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.55f, 0.55f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.55f, 0.45f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.45f, 0.45f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.45f, 0.55f  // top left 
	};

	glGenVertexArrays(aTotal, gVAOs);	// VAO
	glGenBuffers(bTotal, gVBOs);		// VBO
	glGenBuffers(eTotal, gEBOs);	// EBO

	// VAO0
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// 顶点属性0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));	// 顶点颜色属性1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// 顶点纹理坐标属性2
	glEnableVertexAttribArray(2);

	// VAO2
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices11), vertices11, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// 顶点属性0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	// 顶点颜色属性1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// 顶点纹理坐标属性2
	glEnableVertexAttribArray(2);

	// VAO3
	glBindVertexArray(gVAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices12), vertices12, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// 顶点属性0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	// 顶点颜色属性1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// 顶点纹理坐标属性2
	glEnableVertexAttribArray(2);
	return;
}
// 创建所有纹理属性
void BuildTexture()
{
	glGenTextures(tTotal, gTexs);	// 创建纹理缓存
	// 纹理1
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);	// 绑定2D纹理对象目标（和GL_ARRAY_BUFFER一样属于opengl目标之一）
	// 设置纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图像
	int width, height, nrChannels;
	// OpenGL默认y轴原点坐标在如偏的底部，但是图片的y轴原点坐标通常在顶部，所以需要翻转
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../../context/resource/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// 第二个参数如果希望单独手动设置每个多级渐远纹理的级别，0表示基本级别
		// 第三个参数表示希望OpenGL将纹理数据以何种数据格式存储
		// 第七个参数表示源图的格式
		// 第八个参数表示源图的数据类型
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	// 纹理2
	glBindTexture(GL_TEXTURE_2D, gTexs[1]);	// 绑定2D纹理对象
	// 设置纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图像
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	// 纹理3
	glBindTexture(GL_TEXTURE_2D, gTexs[2]);	// 绑定2D纹理对象
	// 设置纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图像
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	// 纹理4
	glBindTexture(GL_TEXTURE_2D, gTexs[3]);	// 绑定2D纹理对象
	// 设置纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图像
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	// 纹理5
	glBindTexture(GL_TEXTURE_2D, gTexs[4]);	// 绑定2D纹理对象
	// 设置纹理包围方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图像
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../../context/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
}
// 为着色器纹理变量指定纹理单元
void InitTexture()
{
	gShaders[1]->use();
	gShaders[1]->setInt("texture1", 0);	// 指定纹理单元0
	gShaders[1]->setInt("texture2", 1);	// 指定纹理单元1

	gShaders[2]->use();
	gShaders[2]->setInt("texture1", 0);	// 指定纹理单元0
	gShaders[2]->setInt("texture2", 1);	// 指定纹理单元1
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
	// 绑定纹理（默认纹理单元0是被激活的）
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);

	gShaders[0]->use();
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

	gShaders[1]->use();
	gShaders[1]->setInt("texture1", 0);	// 指定纹理单元0
	gShaders[1]->setInt("texture2", 1);	// 指定纹理单元1
	gShaders[1]->setFloat("degree", gMixDegree);	// 指定混合程度
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

	gShaders[2]->use();
	gShaders[2]->setInt("texture1", 0);	// 指定纹理单元0
	gShaders[2]->setInt("texture2", 1);	// 指定纹理单元1
	glBindVertexArray(gVAOs[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext4()
{
	// 绑定纹理
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gTexs[2]);

	gShaders[3]->use();
	gShaders[3]->setInt("texture1", 2);	// 指定纹理单元2
	glBindVertexArray(gVAOs[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext5()
{
	// 绑定纹理
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gTexs[3]);

	gShaders[3]->use();
	gShaders[3]->setInt("texture1", 3);	// 指定纹理单元2
	glBindVertexArray(gVAOs[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext6()
{
	// 绑定纹理
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gTexs[4]);

	gShaders[3]->use();
	gShaders[3]->setInt("texture1", 4);	// 指定纹理单元2
	glBindVertexArray(gVAOs[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawContext7()
{
	// 绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexs[0]);

	gShaders[3]->use();
	gShaders[3]->setInt("texture1", 0);	// 指定纹理单元2
	glBindVertexArray(gVAOs[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}