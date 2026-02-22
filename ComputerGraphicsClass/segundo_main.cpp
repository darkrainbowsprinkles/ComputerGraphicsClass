#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <glew.h>
#include <glfw3.h>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
const double TIME_PER_COLOR_CHANGE = 2.0;

GLuint VAO, VBO, shader;

struct Color
{
	float r, g, b;

	Color(float r, float g, float b) : r(r), g(g), b(b) { }
};


//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);	 			\n\
}";

void CrearTriangulo()
{
	GLfloat vertices[] = 
	{
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	// Generar y asignar VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generar y configurar VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configurar atributos de vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Desvincular
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearRomboYTrapecio()
{
	GLfloat vertices[] =
	{
		// Triángulo superior
		0.0f, 0.5f, 0.0f, 
		-0.3f, 0.0f, 0.0f, 
		0.3f, 0.0f, 0.0f, 

		// Triángulo inferior
		0.0f, -0.5f, 0.0f, 
		-0.3f, 0.0f, 0.0f,   
		0.3f, 0.0f, 0.0f,    

		// Triángulo izquierdo del trapecio
		0.5f, 0.4f, 0.0f,   
		0.4f, -0.4f, 0.0f,   
		0.8f, 0.3f, 0.0f,   

		// Triángulo derecho del trapecio
		0.4f, -0.4f, 0.0f,   
		0.9f, -0.4f, 0.0f,   
		0.8f, 0.3f, 0.0f    
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearLetrasIniciales()
{
	GLfloat vertices[] =
	{
		// LETRA O (abajo a la izquierda)

		// Rectangulo izquierdo vertical
		-0.80f, -0.40f, 0.0f,
		-0.75f, -0.40f, 0.0f,
		-0.80f, -0.70f, 0.0f,

		-0.75f, -0.40f, 0.0f,
		-0.75f, -0.70f, 0.0f,
		-0.80f, -0.70f, 0.0f,

		// Rectangulo derecho vertical
		-0.60f, -0.40f, 0.0f,
		-0.55f, -0.40f, 0.0f,
		-0.60f, -0.70f, 0.0f,

		-0.55f, -0.40f, 0.0f,
		-0.55f, -0.70f, 0.0f,
		-0.60f, -0.70f, 0.0f,

		// Rectangulo superior horizontal
		-0.80f, -0.40f, 0.0f,
		-0.55f, -0.40f, 0.0f,
		-0.80f, -0.45f, 0.0f,

		-0.55f, -0.40f, 0.0f,
		-0.55f, -0.45f, 0.0f,
		-0.80f, -0.45f, 0.0f,

		// Rectangulo inferior horizontal
		-0.80f, -0.65f, 0.0f,
		-0.55f, -0.65f, 0.0f,
		-0.80f, -0.70f, 0.0f,

		-0.55f, -0.65f, 0.0f,
		-0.55f, -0.70f, 0.0f,
		-0.80f, -0.70f, 0.0f,

		// LETRA C (centro) 

		// Rectangulo izquierdo vertical
		-0.20f, 0.20f, 0.0f,
		-0.15f, 0.20f, 0.0f,
		-0.20f, -0.10f, 0.0f,

		-0.15f, 0.20f, 0.0f,
		-0.15f, -0.10f, 0.0f,
		-0.20f, -0.10f, 0.0f,

		// Rectangulo superior horizontal
		-0.20f, 0.20f, 0.0f,
		0.05f, 0.20f, 0.0f,
		-0.20f, 0.15f, 0.0f,

		0.05f, 0.20f, 0.0f,
		0.05f, 0.15f, 0.0f,
		-0.20f, 0.15f, 0.0f,

		// Rectangulo inferior horizontal
		-0.20f, -0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		-0.20f, -0.10f, 0.0f,

		0.05f, -0.05f, 0.0f,
		0.05f, -0.10f, 0.0f,
		-0.20f, -0.10f, 0.0f,

		// LETRA T (arriba a la derecha)

		// Barra horizontal superior
		0.35f, 0.80f, 0.0f,
		0.75f, 0.80f, 0.0f,
		0.35f, 0.75f, 0.0f,

		0.75f, 0.80f, 0.0f,
		0.75f, 0.75f, 0.0f,
		0.35f, 0.75f, 0.0f,

		// Barra vertical central
		0.525f, 0.75f, 0.0f,
		0.575f, 0.75f, 0.0f,
		0.525f, 0.45f, 0.0f,

		0.575f, 0.75f, 0.0f,
		0.575f, 0.45f, 0.0f,
		0.525f, 0.45f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// Verificar errores de compilación
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders() 
{
	shader = glCreateProgram(); //se crea un programa

	if (!shader)
	{
		printf("Error creando el shader");
		return;

	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader

	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	glGetProgramiv(shader, GL_LINK_STATUS, &result); //verificaciones y prevención de errores

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

Color GetRandomColor()
{
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;
	return Color(r, g, b);
}

void LoopBackgroundColor(double& lastColorChangeTime)
{
	double currentTime = glfwGetTime();
	double elapsedTime = currentTime - lastColorChangeTime;

	if (elapsedTime >= TIME_PER_COLOR_CHANGE)
	{
		Color randomColor = GetRandomColor();
		glClearColor(randomColor.r, randomColor.g, randomColor.b, 1.0f);
		lastColorChangeTime = currentTime;
	}
}

int main()
{
	srand((unsigned int)time(NULL));

	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Llamada a las funciones creadas antes del main
	//CrearTriangulo();
	//CrearRomboYTrapecio();
	CrearLetrasIniciales();
	CompileShaders();

	Color initialColor = GetRandomColor();
	glClearColor(initialColor.r, initialColor.g, initialColor.b, 1.0f);

	double lastColorChangeTime = glfwGetTime();

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{	
		//Recibir eventos del usuario
		glfwPollEvents();

		LoopBackgroundColor(lastColorChangeTime);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 60);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		 
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}

	return 0;
}
