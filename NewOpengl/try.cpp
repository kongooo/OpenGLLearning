#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *VertexShaderSource = "#version 330 core\n"
	"layout ( location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n";

const char *FragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(0.8f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "StartAgain!", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to creat GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//init viewport
	glViewport(0, 0, 800, 600);

	//callback viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//vertices data
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ÓÒÉÏ½Ç
		0.5f, -0.5f, 0.0f,  // ÓÒÏÂ½Ç
		-0.5f, -0.5f, 0.0f, // ×óÏÂ½Ç
		-0.5f, 0.5f, 0.0f   // ×óÉÏ½Ç
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	//generate VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bind buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy the user data to the current bind buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//create vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach and compile
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//create fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check compile success
	int Vsuccess, Fsuccess;
	char VinfoLog[512], FinfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Vsuccess);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Fsuccess);
	if (!Vsuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, VinfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << VinfoLog << std::endl;
	}
	if (!Fsuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, FinfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << FinfoLog << std::endl;
	}

	//create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//link shaders
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int Psuccess;
	char PinfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Psuccess);
	if (!Psuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, PinfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << PinfoLog << std::endl;
	}

	//set vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//enable attribute
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window))
	{
		//input check
		processInput(window);

		//render
		glClearColor(0.5f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); 

		//active the program object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//exchange buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//release resources
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}