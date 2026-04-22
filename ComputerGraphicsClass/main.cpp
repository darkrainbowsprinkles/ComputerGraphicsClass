/*
Práctica 7: Iluminación 1 
*/

#define STB_IMAGE_IMPLEMENTATION

//para probar el importer
//#include<assimp/Importer.hpp>

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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "main.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture carroTexture;
Texture llantaTexture;
Texture lamparaTexture;
Texture dadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model chasis;
Model cofre;
Model llanta;
Model lampara;

Skybox skybox;

Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// luz direccional
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

static double limitFPS = 1.0 / 60.0;
const float toRadians = 3.14159265f / 180.0f;
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;
static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.33f,  0.5f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.66f,	0.5f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.66f,	0.75f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.33f,	0.75f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.66f,  0.5f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.5f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	0.75f,	    -1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.66f,  0.75f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.66f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.33f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.33f,	0.25f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.66f,	0.25f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.5f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.33f,	0.5f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.33f,	0.75f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.75f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.66f,  0.25f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.33f,  0.25f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.33f,  0.5f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.66f,	0.5f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.33f,	0.75f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.66f,	0.75f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.66f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.33f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CreateLights()
{
	//directional light atardecer
	mainLight = DirectionalLight(0.6f, 0.3f, 0.5f,  
		0.05f, 0.2f,                                
		-1.0f, -0.2f, -0.5f); 

	////directional light dia
	//mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,  
	//	0.8f, 0.3f,                                
	//	0.0f, -1.0f, -0.5f); 

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//faro del coche
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -0.3f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz de helicoptero 
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f, 
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, -1.0f, 0.0f, 
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz de lampara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.8f, 2.0f,     
		0.0f, 0.0f, 0.0f,
		0.3f, 0.1f, 0.05f); 
	pointLightCount++;
}


void LoadTextures()
{
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	carroTexture = Texture("Textures/Carro1.jpg");
	carroTexture.LoadTextureA();
	llantaTexture = Texture("Textures/Llanta.jpg");
	llantaTexture.LoadTextureA();
	lamparaTexture = Texture("Textures/Lampara.png");
	lamparaTexture.LoadTextureA();
	dadoTexture = Texture("Textures/DadoEmociones.png");
	dadoTexture.LoadTextureA();
}

void LoadModels()
{
	chasis = Model();
	chasis.LoadModel("Models/Chasis.fbx");
	cofre = Model();
	cofre.LoadModel("Models/Cofre.fbx");
	llanta = Model();
	llanta.LoadModel("Models/Llanta.fbx");
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	lampara = Model();
	lampara.LoadModel("Models/lampara.fbx");
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

void UpdateDeltaTime(GLfloat& now)
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	deltaTime += (now - lastTime) / limitFPS;
	lastTime = now;
}

void RegisterUserEvents()
{
	glfwPollEvents();
	camera.keyControl(mainWindow.getsKeys(), deltaTime);
	camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
}

void ClearWindow(const glm::mat4& projection, GLuint& uniformModel, GLuint& uniformProjection, GLuint& uniformView, GLuint& uniformEyePosition, GLuint& uniformColor)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformColor = shaderList[0].getColorLocation();
}

void SetShaderInfo(GLuint& uniformSpecularIntensity, GLuint& uniformShininess, GLuint uniformProjection, glm::mat4& projection, GLuint uniformView, GLuint uniformEyePosition)
{
	//información en el shader de intensidad especular y brillo
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	//información al shader de fuentes de iluminación
	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount);
	shaderList[0].SetSpotLights(spotLights, spotLightCount);
}

void RenderPiso(glm::mat4& model, GLuint uniformModel, GLuint uniformColor, glm::vec3& color, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	pisoTexture.UseTexture();
	Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();
}

void SetLinterna(glm::vec3& lowerLight)
{
	lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
}

void HandleFaroCoche(glm::mat4& modelaux, glm::mat4& lightModelFaroCoche)
{
	lightModelFaroCoche = glm::translate(modelaux, glm::vec3(-1.05f, 0.0f, 0.0f));
	spotLights[1].SetPos(glm::vec3(lightModelFaroCoche[3]));

	switch (mainWindow.getFaroColorState())
	{
	case 0:
		spotLights[1].SetColor(glm::vec3(1.0f, 0.0f, 0.0f)); // Rojo
		break;
	case 1:
		spotLights[1].SetColor(glm::vec3(1.0f, 1.0f, 0.0f)); // Amarillo
		break;
	case 2:
		spotLights[1].SetColor(glm::vec3(0.0f, 1.0f, 0.0f)); // Verde
		break;
	case 3:
		spotLights[1].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); // Cian
		break;
	case 4:
		spotLights[1].SetColor(glm::vec3(0.0f, 0.0f, 1.0f)); // Azul
		break;
	case 5:
		spotLights[1].SetColor(glm::vec3(1.0f, 0.0f, 1.0f)); // Magenta
		break;
	}
}

void RenderCoche(glm::mat4& model, GLuint uniformModel, glm::mat4& modelaux, GLuint uniformSpecularIntensity, GLuint uniformShininess, glm::mat4& lightModelFaroCoche)
{
	//Chasis
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	carroTexture.UseTexture();
	chasis.RenderModel();

	//Cofre 
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(-0.95f, 0.4f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	carroTexture.UseTexture();
	cofre.RenderModel();

	// Faro 
	HandleFaroCoche(modelaux, lightModelFaroCoche);

	//Llanta delantera izquierda
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(-1.65f, -0.65f, 0.7f));
	modelaux = glm::rotate(modelaux, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llantaTexture.UseTexture();
	llanta.RenderModel();

	//Llanta delantera derecha
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(-1.65f, -0.65f, -0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llantaTexture.UseTexture();
	llanta.RenderModel();

	//Llanta trasera izquierda
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(1.25f, -0.65f, 0.7f));
	modelaux = glm::rotate(modelaux, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llantaTexture.UseTexture();
	llanta.RenderModel();

	//Llanta trasera derecha
	modelaux = model;
	modelaux = glm::translate(modelaux, glm::vec3(1.25f, -0.65f, -0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
	llantaTexture.UseTexture();
	llanta.RenderModel();
}

void RenderHelicoptero(glm::mat4& model, GLuint uniformModel)
{
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(mainWindow.getmuevex2(), 5.0f, 6.0f));

	spotLights[2].SetPos(glm::vec3(model[3]));

	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Blackhawk_M.RenderModel();
}

void RenderAgave(glm::mat4& model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
	//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//blending: transparencia o traslucidez
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	AgaveTexture.UseTexture();
	Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();
	glDisable(GL_BLEND);
}

void RenderLampara(glm::mat4& model, GLuint uniformModel, glm::mat4& lightModelLampara)
{
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, -10.0f));

	if (mainWindow.getLuzLampara()) 
	{
		lightModelLampara = glm::translate(model, glm::vec3(1.3f, 7.0f, 0.0f)); 
		pointLights[0].SetPos(glm::vec3(lightModelLampara[3]));
		pointLights[0].SetColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Encendida
	} 
	else 
	{
		pointLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f)); // Apagada
	}

	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	lamparaTexture.UseTexture();
	lampara.RenderModel();
}

void RenderDado(glm::mat4& model, GLuint uniformModel)
{
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dadoTexture.UseTexture();
	meshList[4]->RenderMesh();
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	CreateObjects();
	CrearDado();
	CreateShaders();
	LoadTextures();
	LoadModels();
	SetSkybox();
	CreateLights();

	GLfloat now = 0.0f;
	glm::vec3 lowerLight = glm::vec3(0.0f);
	glm::mat4 lightModelFaroCoche = glm::mat4(1.0f);
	glm::mat4 lightModelLampara = glm::mat4(1.0f);

	while (!mainWindow.getShouldClose())
	{
		UpdateDeltaTime(now);
		RegisterUserEvents();
		ClearWindow(projection, uniformModel, uniformProjection, uniformView, uniformEyePosition, uniformColor);
		SetShaderInfo(uniformSpecularIntensity, uniformShininess, uniformProjection, projection, uniformView, uniformEyePosition);

		SetLinterna(lowerLight);
		RenderPiso(model, uniformModel, uniformColor, color, uniformSpecularIntensity, uniformShininess);
		RenderCoche(model, uniformModel, modelaux, uniformSpecularIntensity, uniformShininess, lightModelFaroCoche);
		RenderHelicoptero(model, uniformModel);
		RenderLampara(model, uniformModel, lightModelLampara);
		RenderDado(model, uniformModel);
		RenderAgave(model, uniformModel, uniformSpecularIntensity, uniformShininess);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}