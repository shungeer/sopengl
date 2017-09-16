#include <iostream>
#include <map>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ft2build.h"
#include "freetype/freetype.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void BuildFont();
void BuildShader();
void BuildModel();
void ClearContext();
void InitGlobalShader();
void RenderText(CShader &shader, const unsigned VAO, const unsigned VBO, const std::string& text, const glm::vec2& pos, float scale, const glm::vec3& color);
void DrawNULLContext();
void DrawContext1();

void(*gFun)();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Character
{
	GLuint TextureID;	// 纹理ID
	glm::ivec2 Size;	// 字形大小
	glm::ivec2 Beraing;	// 从基准线到字形左部/顶部的偏移值
	unsigned int Advance;	// 原点距下一个字形原点的距离
};

const unsigned sTotal = 1;
const unsigned aTotal = 1;
const unsigned bTotal = 1;

std::map<GLchar, Character> gMapChar;
CShader* gPShaders[sTotal];
unsigned gVAOs[aTotal];
unsigned gVBOs[bTotal];

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
	// 创建字体
	BuildFont();
	// 创建着色器
	BuildShader();
	// 创建顶点等对象
	BuildModel();
	// 初始化全局
	InitGlobalShader();

	// 7.循环渲染
	gFun = DrawNULLContext;
	// OpenGL选项
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		gFun = DrawNULLContext;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gFun = DrawContext1;
	}
}
//创建字体
void BuildFont()
{
	// 1. 初始化freetype库
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// 2. 加载字体到面
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	// 3. 定义字体大小(宽度为0表示通过高度自动计算字形宽度)
	FT_Set_Pixel_Sizes(face, 0, 48);

	// 4. 渲染到纹理
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
	for (unsigned char c = 0; c < 128; c++)
	{
		// 加载字符字形
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// 生成纹理
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// 设置纹理选项
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		gMapChar.insert(std::pair<GLchar, Character>(c, character));
		// 解绑
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// 5. 释放资源
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
// 创建所有着色器程序
void BuildShader()
{
	gPShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
}
// 创建所有渲染模型
void BuildModel()
{
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);
	// 1. vao0（先声明内存，再动态更新）
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 6 * 4, NULL, GL_DYNAMIC_DRAW);
	// 顶点属性指定
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 4, 0);
	glEnableVertexAttribArray(0);
	// 解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
// 初始化全局着色器变量
void InitGlobalShader()
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	gPShaders[0]->use();
	gPShaders[0]->setMat4("projection", projection);
}

// 清除渲染上下文
void ClearContext()
{
	glDeleteVertexArrays(aTotal, gVAOs);
	glDeleteBuffers(bTotal, gVBOs);
	for (unsigned i = 0; i < sTotal; i++)
	{
		delete gPShaders[i];
	}
	return;
}

void DrawNULLContext()
{
	return;
}

void RenderText(CShader &shader, const unsigned VAO, const unsigned VBO, const std::string& text, const glm::vec2& pos, float scale, const glm::vec3& color)
{
	// 设置颜色
	shader.setVecf3("textColor", color.x, color.y, color.z);
	// 激活纹理
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// 遍历字符
	float x = pos.x;
	float y = pos.y;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = gMapChar[*c];
		float posX = x + ch.Beraing.x * scale;
		float posY = y - (ch.Size.y - ch.Beraing.y) * scale;
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// 对每个字符更新VBO
		float vertices[] =
		{
			// 下三角形
			posX, posY + h, 0.0f, 0.0f,
			posX, posY, 0.0f, 1.0f,
			posX + w, posY, 1.0f, 1.0f,
			// 下三角形
			posX, posY + h, 0.0f, 0.0f,
			posX + w, posY, 1.0f, 1.0f,
			posX + w, posY + h, 1.0f, 0.0f
		};
		// 绑定纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// 更新VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		// 解绑
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 绘制
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
	}
	// 解绑
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawContext1()
{
	RenderText(*gPShaders[0], gVAOs[0], gVBOs[0], "This is sample text", glm::vec2(25.0f, 25.0f), 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	RenderText(*gPShaders[0], gVAOs[0], gVBOs[0], "(C) LearnOpenGL.com", glm::vec2(540.0f, 570.0f), 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
}
