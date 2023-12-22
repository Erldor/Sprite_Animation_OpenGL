#include <iostream>
#include <cmath>
#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>


#include <stb/stb_image.h>


//	������� ��� ��������� ��������� ������� ����
void framebuffer_size_callback(GLFWwindow*, int, int); 

//	������� ��� �������� �������� �� ���������� �����
std::string loadShader(const char*);

//	���������� ��� �������� ������� ����
int windowWidth = 1280;
int windowHeight = 720;


int main()
{
	//	���������� ��� �������� ����� ����
	float red, green, blue; 
	red = 0.35f;
	green = 0.38f;
	blue = 0.7f;

	glfwInit();

	//	�������� ��������� �������� �� ������������ ������ OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//	������ ����
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "TestWindow", NULL, NULL);
	
	//	��������, ������� �� ����
	if (window == NULL)
	{
		std::cout << "Failed to create the window" << std::endl;
		glfwTerminate();
	}

	//	������ ��������� ���� ���� �������
	glfwMakeContextCurrent(window);

	//	��������, �������� �� ���������� GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//	��������� ������� �������
	glViewport(0, 0, windowWidth, windowHeight);

	//	������������� ���������� ������� ��� ��������� ��������� ������� ����
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//	��������� ��������� ����� ������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	stbi_set_flip_vertically_on_load(true);

//	rect.Create(300.f, 300.f, "assets/textures/Attack.png");

	GLuint texture = 0;
	
	int indexSize = 0;

	//	��������� ��� ������� ��������������

	float float_vertices[] = 
	{
		-0.5f, -0.88f,	0.f, 0.f,
		-0.5f, 0.88f,	0.f, 1.f,
		0.5f, 0.88f,		0.2f, 1.f,
		0.5f, -0.88f,	0.2f, 0.0f
	};
	unsigned int indices[] = 
	{
		0, 1, 2,
		0, 3, 2
	};

	indexSize = 6;


	int success;
	char infoLog[512];

	//	�������� ���������� �������

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexShaderSrc = loadShader("assets/shaders/vertexShader.glsl");
	const GLchar* vertexShader_char = vertexShaderSrc.c_str();

	//	���������� ������� ������� � ����� ������� � ����������
	glShaderSource(vertexShader, 1, &vertexShader_char, NULL);
	glCompileShader(vertexShader);

	//	��������� ������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "GL_VERTEX_SHADER" << " COMPILE ERROR:" << std::endl << infoLog << std::endl;
	}

	//	�������� ������������ �������

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragmentShaderSrc = loadShader("assets/shaders/fragmentShader.glsl");
	const GLchar* fragmentShader_char = fragmentShaderSrc.c_str();

	//	���������� ������� ������� � ����� ������� � ����������
	glShaderSource(fragmentShader, 1, &fragmentShader_char, NULL);
	glCompileShader(fragmentShader);

	//	��������� ������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "GL_FRAGMENT_SHADER" << " COMPILE ERROR : " << std::endl << infoLog << std::endl;
	}

	unsigned int id = 0;

	//	�������� ��������� ��������
	id = glCreateProgram();
	glAttachShader(id, vertexShader); //  ������������� ���������� �������
	glAttachShader(id, fragmentShader); //	������������� ������������ �������
	glLinkProgram(id); //  ���������� ��������

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << " SHADER PROGRAM " << id << " LINK ERROR:" << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//	VAO | VBO | EBO

	unsigned int VAO = 0, VBO = 0, EBO = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ��������� ������ ������� ������ � ���������� VAO
	glBindVertexArray(VAO);	

	//	��������� ������ ������ ������ � ���������� VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	
	//	������� � �������������� ��������� ������ VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float_vertices), float_vertices, GL_STATIC_DRAW); 

	//	��������� ������ ������ �������� � ���������� EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
	//	������� � �������������� ��������� ������ EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//	����������� ������ ��������� �������
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//	�������� ������ ������� ���������� �������
	glEnableVertexAttribArray(0); 


	//	��������� ��������

	//	����������� ������ ��������� �������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	std::string texture_loc = "assets/textures/Attack.png";

	texture_loc = texture_loc.substr(texture_loc.length() - 3, texture_loc.length() - 1);

	int imgWidth, imgHeight, colChannels;

	unsigned char* img_info = stbi_load("assets/textures/Attack.png", &imgWidth, &imgHeight, &colChannels, 0);

	//	���������� � ���������� ��������
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	//	��������� ��������������� �������� � �������� ��������
	glBindTexture(GL_TEXTURE_2D, texture);

	//	��������� ���������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_info);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(img_info);
	glBindTexture(GL_TEXTURE_2D, 0);

	//	����������� �������� uniform ���������� tex0
	glUniform1i(glGetUniformLocation(id, "tex0"), 0);

	// ���������� VAO �� ������� ������� ������
	glBindVertexArray(0);

	glUseProgram(id); //	��������� ���������

	//	����� ������ �������
	double startTime = glfwGetTime();
	int animationCounter = 0; //	������� ���������� ����������� ��������
	int animationCount = 4; //	���������� ��������, ������� ����� ���������

	while (!glfwWindowShouldClose(window))
	{
		//	����� ���� ����
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (glfwGetTime() - startTime >= 0.12f)
		{
			if (animationCounter < animationCount)
			{
				animationCounter++;
			}
			else
			{
				animationCounter = 0;
			}

			//	����� �������� ���������� textureAdd_x � ����������� �������
			glUniform1f(glGetUniformLocation(id, "textureAdd_x"), 0.2f * animationCounter);

			startTime = glfwGetTime();
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glUseProgram(id);
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

		// ���������� VAO �� ������� ������� ������
		glBindVertexArray(0);

		//	���������� ������
		glfwSwapBuffers(window);
		//	��������� �������
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

//	������������� ������� ��� ��������� ������� ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ������������� ������� ��� �������� ��������
std::string loadShader(const char* fileDirectory)
{
	std::ifstream file;
	std::stringstream buf;

	std::string result = "";

	file.open(fileDirectory);

	if (file.is_open())
	{
		buf << file.rdbuf();
		result = buf.str();
	}
	else
	{
		std::cout << "Could not open " << fileDirectory << std::endl;
	}

	file.close();

	return result;
}


