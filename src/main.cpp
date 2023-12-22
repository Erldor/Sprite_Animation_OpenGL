#include <iostream>
#include <cmath>
#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>


#include <stb/stb_image.h>


//	Функция для обработки изменения размера окна
void framebuffer_size_callback(GLFWwindow*, int, int); 

//	Функция для загрузки шейдеров из отдельного файла
std::string loadShader(const char*);

//	Переменные для хранения размера окна
int windowWidth = 1280;
int windowHeight = 720;


int main()
{
	//	Переменные для хранения цвета окна
	float red, green, blue; 
	red = 0.35f;
	green = 0.38f;
	blue = 0.7f;

	glfwInit();

	//	Передача программе сведений об используемом версии OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//	Создаём окно
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "TestWindow", NULL, NULL);
	
	//	Проверка, создано ли окно
	if (window == NULL)
	{
		std::cout << "Failed to create the window" << std::endl;
		glfwTerminate();
	}

	//	Делаем созданное нами окно текущим
	glfwMakeContextCurrent(window);

	//	Проверка, работает ли библиотека GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//	Установка области видения
	glViewport(0, 0, windowWidth, windowHeight);

	//	Инициализация внутренной функции для обработки изменения размера окна
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//	Включение поддержки альфа канала
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	stbi_set_flip_vertically_on_load(true);

//	rect.Create(300.f, 300.f, "assets/textures/Attack.png");

	GLuint texture = 0;
	
	int indexSize = 0;

	//	Настройки для рендера прямоугольника

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

	//	Создание вершинного шейдера

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexShaderSrc = loadShader("assets/shaders/vertexShader.glsl");
	const GLchar* vertexShader_char = vertexShaderSrc.c_str();

	//	Связывание объекта шейдера с кодом шейдера и компиляция
	glShaderSource(vertexShader, 1, &vertexShader_char, NULL);
	glCompileShader(vertexShader);

	//	Обработка ошибки
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "GL_VERTEX_SHADER" << " COMPILE ERROR:" << std::endl << infoLog << std::endl;
	}

	//	Создание фрагментного шейдера

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragmentShaderSrc = loadShader("assets/shaders/fragmentShader.glsl");
	const GLchar* fragmentShader_char = fragmentShaderSrc.c_str();

	//	Связывание объекта шейдера с кодом шейдера и компиляция
	glShaderSource(fragmentShader, 1, &fragmentShader_char, NULL);
	glCompileShader(fragmentShader);

	//	Обработка ошибки
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "GL_FRAGMENT_SHADER" << " COMPILE ERROR : " << std::endl << infoLog << std::endl;
	}

	unsigned int id = 0;

	//	Создание программы шейдеров
	id = glCreateProgram();
	glAttachShader(id, vertexShader); //  присоединение вершинного шейдера
	glAttachShader(id, fragmentShader); //	присоединение фрагментного шейдера
	glLinkProgram(id); //  Связывание шейдеров

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

	// Связываем объект массива вершин с переменной VAO
	glBindVertexArray(VAO);	

	//	Связываем объект буфера вершин с переменной VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	
	//	Создаем и инициализируем хранилище данных VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float_vertices), float_vertices, GL_STATIC_DRAW); 

	//	Связываем объект буфера индексов с переменной EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
	//	Создаем и инициализируем хранилище данных EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//	Настраиваем первый вершинный атрибут
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//	Включаем первый атрибут вершинного шейдера
	glEnableVertexAttribArray(0); 


	//	Наложение текстуры

	//	Настраиваем второй вершинный атрибут
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	std::string texture_loc = "assets/textures/Attack.png";

	texture_loc = texture_loc.substr(texture_loc.length() - 3, texture_loc.length() - 1);

	int imgWidth, imgHeight, colChannels;

	unsigned char* img_info = stbi_load("assets/textures/Attack.png", &imgWidth, &imgHeight, &colChannels, 0);

	//	Генерируем и активируем текстуру
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	//	Связываем сгенерированную текстуру с объектом текстуры
	glBindTexture(GL_TEXTURE_2D, texture);

	//	Настройка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_info);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(img_info);
	glBindTexture(GL_TEXTURE_2D, 0);

	//	Присваиваем значение uniform переменной tex0
	glUniform1i(glGetUniformLocation(id, "tex0"), 0);

	// Отвязываем VAO от объекта массива вершин
	glBindVertexArray(0);

	glUseProgram(id); //	Запускаем программу

	//	Время начала отсчета
	double startTime = glfwGetTime();
	int animationCounter = 0; //	Счетчик количества проигранных анимаций
	int animationCount = 4; //	Количество анимаций, которые нужно проиграть

	while (!glfwWindowShouldClose(window))
	{
		//	Задаём цвет окна
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

			//	Задаём значение переменной textureAdd_x в фрагментном шейдере
			glUniform1f(glGetUniformLocation(id, "textureAdd_x"), 0.2f * animationCounter);

			startTime = glfwGetTime();
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glUseProgram(id);
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

		// Отвязываем VAO от объекта массива вершин
		glBindVertexArray(0);

		//	Обновление буфера
		glfwSwapBuffers(window);
		//	Обработка событий
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

//	Инициализация функции для изменения размера окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Инициализация функции для загрузки шейдеров
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


