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
	GLuint TextureID;	// ����ID
	glm::ivec2 Size;	// ���δ�С
	glm::ivec2 Beraing;	// �ӻ�׼�ߵ�������/������ƫ��ֵ
	unsigned int Advance;	// ԭ�����һ������ԭ��ľ���
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

	// 6.��Ⱦ�����Ķ���
	// ��������
	BuildFont();
	// ������ɫ��
	BuildShader();
	// ��������ȶ���
	BuildModel();
	// ��ʼ��ȫ��
	InitGlobalShader();

	// 7.ѭ����Ⱦ
	gFun = DrawNULLContext;
	// OpenGLѡ��
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window))
	{
		// ��Ⱦ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
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
//��������
void BuildFont()
{
	// 1. ��ʼ��freetype��
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// 2. �������嵽��
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	// 3. ���������С(���Ϊ0��ʾͨ���߶��Զ��������ο��)
	FT_Set_Pixel_Sizes(face, 0, 48);

	// 4. ��Ⱦ������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������
	for (unsigned char c = 0; c < 128; c++)
	{
		// �����ַ�����
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// ��������
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// ��������ѡ��
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
		// ���
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// 5. �ͷ���Դ
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
// ����������ɫ������
void BuildShader()
{
	gPShaders[0] = new CShader("shaders/vertex.shader", "shaders/fragment.shader");
}
// ����������Ⱦģ��
void BuildModel()
{
	glGenVertexArrays(aTotal, gVAOs);
	glGenBuffers(bTotal, gVBOs);
	// 1. vao0���������ڴ棬�ٶ�̬���£�
	glBindVertexArray(gVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 6 * 4, NULL, GL_DYNAMIC_DRAW);
	// ��������ָ��
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 4, 0);
	glEnableVertexAttribArray(0);
	// ���
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
// ��ʼ��ȫ����ɫ������
void InitGlobalShader()
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	gPShaders[0]->use();
	gPShaders[0]->setMat4("projection", projection);
}

// �����Ⱦ������
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
	// ������ɫ
	shader.setVecf3("textColor", color.x, color.y, color.z);
	// ��������
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// �����ַ�
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
		// ��ÿ���ַ�����VBO
		float vertices[] =
		{
			// ��������
			posX, posY + h, 0.0f, 0.0f,
			posX, posY, 0.0f, 1.0f,
			posX + w, posY, 1.0f, 1.0f,
			// ��������
			posX, posY + h, 0.0f, 0.0f,
			posX + w, posY, 1.0f, 1.0f,
			posX + w, posY + h, 1.0f, 0.0f
		};
		// ������
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// ����VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		// ���
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// ����
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
	}
	// ���
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawContext1()
{
	RenderText(*gPShaders[0], gVAOs[0], gVBOs[0], "This is sample text", glm::vec2(25.0f, 25.0f), 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	RenderText(*gPShaders[0], gVAOs[0], gVBOs[0], "(C) LearnOpenGL.com", glm::vec2(540.0f, 570.0f), 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
}
