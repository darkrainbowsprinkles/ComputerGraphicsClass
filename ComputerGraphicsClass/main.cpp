/*
Práctica 5: Optimización y Carga de Modelos
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "main.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Model chasis;
Model cofre;
Model llanta;

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static double limitFPS = 1.0 / 60.0;
static const char* vShader = "shaders/shader_m.vert";
static const char* fShader = "shaders/shader_m.frag";
const float toRadians = 3.14159265f / 180.0f;

void CreateObjects()
{
	unsigned int indices[] = 
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = 
	{
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = 
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = 
	{
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void SetSkybox()
{
	std::vector<std::string> skyboxFaces;

	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);
}

void InicializaModelsCoche()
{
	chasis = Model();
	chasis.LoadModel("Models/Coche_Chasis.obj");

	cofre = Model();
	cofre.LoadModel("Models/Coche_Cofre.obj");

	llanta = Model();
	llanta.LoadModel("Models/Coche_Llanta.obj");
}

void DibujaPiso(GLuint uniformColor, GLuint uniformModel, glm::vec3& color, glm::mat4& model)
{
	color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[2]->RenderMesh();
}

void DibujaCoche(GLuint uniformColor, GLuint uniformModel, glm::vec3 color, glm::mat4 model, glm::mat4 modelaux)
{
	//Chasis color azul
	color = glm::vec3(0.0f, 0.0f, 1.0f); 
	model = glm::mat4(1.0);

	//Traslacion en la raiz de la jerarquia
	model = glm::translate(model, glm::vec3(mainWindow.getMovimientoCoche(), 0.0f, 0.0f)); 

	//Posicion base del coche (chasis)
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	chasis.RenderModel();

	//Cofre color rojo 
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(-0.95f, 0.4f, 0.0f));
	modelaux = glm::rotate(modelaux, glm::radians(-mainWindow.getRotacionCofre()), glm::vec3(0.0f, 0.0f, 1.0f)); 
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	cofre.RenderModel();

	//Llantas color negro
	color = glm::vec3(0.1f, 0.1f, 0.1f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));

	//Llanta delantera izquierda
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(-1.65f, -0.65f, 0.7f));
	modelaux = glm::rotate(modelaux, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llanta.RenderModel();

	//Llanta delantera derecha
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(-1.65f, -0.65f, -0.7f));
	modelaux = glm::rotate(modelaux, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llanta.RenderModel();

	//Llanta trasera izquierda
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(1.25f, -0.65f, 0.7f));
	modelaux = glm::rotate(modelaux, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llanta.RenderModel();

	//Llanta trasera derecha
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(1.25f, -0.65f, -0.7f));
	modelaux = glm::rotate(modelaux, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llanta.RenderModel();
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	InicializaModelsCoche();
	SetSkybox();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		DibujaPiso(uniformColor, uniformModel, color, model);
		DibujaCoche(uniformColor, uniformModel, color, model, modelaux);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
