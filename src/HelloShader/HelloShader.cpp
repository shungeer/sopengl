#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void BuildShaderProgram();
void BuildModel();
void ClearContext();
void DrawNULLContext();
void CreateContext1();
void DrawContext1();
void DrawContext2();
void DrawContext3();
void DrawContext4();
void DrawContext5();
void DrawContext6();
void DrawContext7();
void DrawContext8();
void DrawContext9();
void DrawContextM();

// 渲染函数指针
void(*gfun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int vbTotal = 7;
const unsigned int vaTotal = 7;
const unsigned int ebTotal = 1;
const unsigned int spTotal = 4;
const unsigned int csTotal = 4;

unsigned int gVBOs[vbTotal];
unsigned int gVAOs[vaTotal];
unsigned int gEBOs[ebTotal];
unsigned int gShaderPrograms[spTotal];
CShader* gShaders[csTotal];

int main(int argc, CHAR* argv[])
{
	// 1.GLFW配置
	glfwInit();										// 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// 设置OpenGL主版本号是3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// 设置OpenGL次版本号是3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// 设置OpenGL使用核心模式
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// 这是mac osx系统确保上述设置起作用的代码

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
	glViewport(0, 0, 800, 600);		// 参数说明：左下角位置+宽度+高度

	// 5.注册回调函数（更改视口、处理手柄输入、处理错误消息等）
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// 更改视口
	glfwSetKeyCallback(window, key_callback);							// 键盘输入

	// 6.渲染上下文定义
	// 1) 创建着色器
	BuildShaderProgram();
	// 2) 创建顶点等对象
	BuildModel();

	// 7.循环渲染
	gfun = DrawNULLContext;
	while (!glfwWindowShouldClose(window))
	{
		// 渲染命令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		gfun();		// 绘制

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
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
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
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gfun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gfun = DrawContext1;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		gfun = DrawContext2;
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		gfun = DrawContext3;
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		gfun = DrawContext4;
	}
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		gfun = DrawContext5;
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		gfun = DrawContext6;
	}
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		gfun = DrawContext7;
	}
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		gfun = DrawContext8;
	}
	else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		gfun = DrawContext9;
	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		gfun = DrawContextM;
	}
}

// 创建所有着色器程序
void BuildShaderProgram()
{
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	const char *fragmentShaderSource1 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);\n"
		"}\n\0";
	const char *fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor;\n"
		"}\n\0";
	const char *vertexShaderSource1 =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\0";
	const char *fragmentShaderSource3 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, 1.0);\n"
		"}\n\0";


	// 创建着色器
	// 顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);		// 附加着色器代码
	glCompileShader(vertexShader);									// 编译着色器代码
	// 检查着色器是否编译成功
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 顶点着色器1
	unsigned int vertexShader1;
	vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader1, 1, &vertexShaderSource1, NULL);		// 附加着色器代码
	glCompileShader(vertexShader1);									// 编译着色器代码
	// 检查着色器是否编译成功
	glGetShaderiv(vertexShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 片元着色器1
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 片元着色器2
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 片元着色器3
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 片元着色器4
	unsigned int fragmentShader3;
	fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
	glCompileShader(fragmentShader3);
	glGetShaderiv(fragmentShader3, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader3, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG3::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 链接到一个使用定义着色器渲染的程序
	gShaderPrograms[0] = glCreateProgram();
	glAttachShader(gShaderPrograms[0], vertexShader);	// 这个着色器的输出作为下一个定义紧邻着色器的输入
	glAttachShader(gShaderPrograms[0], fragmentShader);
	glLinkProgram(gShaderPrograms[0]);
	// 检查着色器是否链接成功
	glGetProgramiv(gShaderPrograms[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 链接到一个使用定义着色器渲染的程序2
	gShaderPrograms[1] = glCreateProgram();
	glAttachShader(gShaderPrograms[1], vertexShader);	// 这个着色器的输出作为下一个定义紧邻着色器的输入
	glAttachShader(gShaderPrograms[1], fragmentShader1);
	glLinkProgram(gShaderPrograms[1]);
	// 检查着色器是否链接成功
	glGetProgramiv(gShaderPrograms[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 链接到一个使用定义着色器渲染的程序3
	gShaderPrograms[2] = glCreateProgram();
	glAttachShader(gShaderPrograms[2], vertexShader);	// 这个着色器的输出作为下一个定义紧邻着色器的输入
	glAttachShader(gShaderPrograms[2], fragmentShader2);
	glLinkProgram(gShaderPrograms[2]);
	// 检查着色器是否链接成功
	glGetProgramiv(gShaderPrograms[2], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[2], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 链接到一个使用定义着色器渲染的程序4
	gShaderPrograms[3] = glCreateProgram();
	glAttachShader(gShaderPrograms[3], vertexShader1);	// 这个着色器的输出作为下一个定义紧邻着色器的输入
	glAttachShader(gShaderPrograms[3], fragmentShader3);
	glLinkProgram(gShaderPrograms[3]);
	// 检查着色器是否链接成功
	glGetProgramiv(gShaderPrograms[3], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderPrograms[3], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM3::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 着色器链接成功后，可以删除相关着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader1);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);

	// 着色器程序
	gShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
	gShaders[1] = new CShader("shaders/vertex1.shader", "shaders/fragment.shader");
	gShaders[2] = new CShader("shaders/vertex2.shader", "shaders/fragment1.shader");
	gShaders[3] = new CShader("shaders/vertex3.shader", "shaders/fragment.shader");
}

// 创建所有渲染模型
void BuildModel()
{
	// 渲染属性
	float vertices1[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float vertices2[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices2[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	float vertices3[] =
	{
		-0.5f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	float vertices41[] =
	{
		-1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float vertices42[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		1.0f, 0.5f, 0.0f
	};

	float vertices5[] =
	{
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
	};

	float vertices51[] =
	{
		// 位置              // 颜色
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 
		0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 
	};

	// 配置OpenGL解释GPU内存的方式
	// 创建OpenGL对象
	glGenBuffers(vbTotal, gVBOs);		// 创建VBO缓冲对象
	glGenVertexArrays(vaTotal, gVAOs);	// VAO对象
	glGenBuffers(ebTotal, gEBOs);		// EBO对象
	// 先绑定VAO（此VAO保存当前设置）
	glBindVertexArray(gVAOs[0]);
	// 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);	// 为VBO指定缓存目标类型
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);	// 复制数据到当前绑定的VBO缓冲
	// glVertexAttribPointer从最近绑定到GL_ARRAY_BUFFER的VBO中获取数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 配置解析定义顶点的方式
	glEnableVertexAttribArray(0);		// 启用顶点属性（以顶点属性位置作为参数）
	// 当完成glVertexAttribPointer后，表示已经从当前绑定的VBO中获取数据，可以解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);					// 解绑VAO，配置完VAO，暂时还不使用，先解绑

	// 重复上述步骤(四边形)
	glBindVertexArray(gVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBOs[0]);	// 指定EBO的缓存目标类型
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);

	// 两个相连三角形：GLDrawArray
	glBindVertexArray(gVAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);

	// 不相连三角形
	glBindVertexArray(gVAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices41), vertices41, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);

	glBindVertexArray(gVAOs[4]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices42), vertices42, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);

	// 顶点颜色不同的三角形
	glBindVertexArray(gVAOs[5]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices5), vertices5, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);

	// 顶点颜色不同的三角形2
	glBindVertexArray(gVAOs[6]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices51), vertices51, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 解绑VBO
	glBindVertexArray(0);
}

// 清除渲染上下文
void ClearContext()
{
	glDeleteVertexArrays(vaTotal, gVAOs);
	glDeleteBuffers(vbTotal, gVBOs);
	glDeleteBuffers(ebTotal, gEBOs);
	for (unsigned int i = 0; i < spTotal; i++)
	{
		glDeleteProgram(gShaderPrograms[i]);
	}
	for (unsigned int i = 0; i < csTotal; i++)
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
	// 绘制
	glUseProgram(gShaderPrograms[0]);		// 激活着色器
	glBindVertexArray(gVAOs[0]);			// 绑定使用的VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// OpenGL绘制函数
	glBindVertexArray(0);				// 解绑VAOs
}

void DrawContext2()
{
	glUseProgram(gShaderPrograms[0]);
	glBindVertexArray(gVAOs[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void DrawContext3()
{
	glUseProgram(gShaderPrograms[0]);
	glBindVertexArray(gVAOs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawContext4()
{
	glUseProgram(gShaderPrograms[0]);
	glBindVertexArray(gVAOs[3]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glUseProgram(gShaderPrograms[1]);
	glBindVertexArray(gVAOs[4]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawContext5()
{
	glUseProgram(gShaderPrograms[2]);
	// 更新uniform变量（更新之前需要激活着色器）
	// 更新uniform颜色
	double timeValue = glfwGetTime();
	float redValue = (float)sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(gShaderPrograms[2], "ourColor");
	glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

	glBindVertexArray(gVAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawContext6()
{
	glUseProgram(gShaderPrograms[3]);
	glBindVertexArray(gVAOs[5]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawContext7()
{
	gShaders[0]->use();
	glBindVertexArray(gVAOs[6]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
void DrawContext8()
{
	gShaders[1]->use();
	glBindVertexArray(gVAOs[6]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
void DrawContext9()
{
	gShaders[2]->use();
	// 更新uniform变量（更新之前需要激活着色器）
	double timeValue = glfwGetTime();
	float flip = (float)sin(timeValue) / 2.0f;
	gShaders[2]->setFloat("flipx", flip);

	// 更新uniform颜色
	gShaders[2]->use();
	timeValue = glfwGetTime();
	float blueValue = (float)sin(timeValue) / 2.0f + 0.5f;
	gShaders[2]->setVecf4("ourColor", 0.0f, 0.0f, blueValue, 1.0f);

	glBindVertexArray(gVAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
void DrawContextM()
{
	gShaders[3]->use();
	glBindVertexArray(gVAOs[6]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}



