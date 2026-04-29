#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	fuegoEncendido = true;
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	muevex2 = 0.0f; 
	pezPosX = 0.0f;
	pezPosY = 4.0f;
	pezPosZ = 10.0f;
	pezSpotDirX = 0.0f;
	pezSpotDirY = -1.0f;
	pezSpotDirZ = 0.0f;
	luzLampara = true;
	luzFocoPez = true;
	faroColorState = 0;
	naveAvanzando = false;
	naveRetrocediendo = false;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	fuegoEncendido = true;
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 8: Iluminacion 2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex -= 1.0;
	}

	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex += 1.0;
	}

	if (key == GLFW_KEY_I)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT) 
		{
			theWindow->naveAvanzando = true;
			theWindow->muevex2 -= 1.0;
		} 
		else if (action == GLFW_RELEASE) 
		{
			theWindow->naveAvanzando = false;
		}
	}

	if (key == GLFW_KEY_O) 
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT) 
		{
			theWindow->naveRetrocediendo = true;
			theWindow->muevex2 += 1.0;
		} 
		else if (action == GLFW_RELEASE) 
		{
			theWindow->naveRetrocediendo = false;
		}
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		theWindow->fuegoEncendido = !theWindow->fuegoEncendido;
	}

	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT)) 
	{
		if (theWindow->pezPosX - 0.1f >= -4.0f && theWindow->pezPosY - 0.1f >= -0.5f && theWindow->pezPosZ - 0.1f >= 6.0f)
		{
			theWindow->pezPosX -= 0.1f;
			theWindow->pezPosY -= 0.1f;
			theWindow->pezPosZ -= 0.1f;
		}
	}

	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT)) 
	{
		if (theWindow->pezPosX + 0.1f <= 4.0f && theWindow->pezPosY + 0.1f <= 8.5f && theWindow->pezPosZ + 0.1f <= 14.0f)
		{
			theWindow->pezPosX += 0.1f;
			theWindow->pezPosY += 0.1f;
			theWindow->pezPosZ += 0.1f;
		}
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		theWindow->luzLampara = !theWindow->luzLampara;
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		theWindow->luzFocoPez = !theWindow->luzFocoPez;
	}

	//Spotlight del Pez
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->pezSpotDirX += 0.1f;
	}
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->pezSpotDirX -= 0.1f;
	}
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->pezSpotDirY += 0.1f;
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->pezSpotDirY -= 0.1f;
	}
	if (key == GLFW_KEY_PERIOD && (action == GLFW_PRESS || action == GLFW_REPEAT)) 
	{
		theWindow->pezSpotDirZ += 0.1f;
	}
	if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT))  
	{
		theWindow->pezSpotDirZ -= 0.1f;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		theWindow->faroColorState = (theWindow->faroColorState + 1) % 6;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
