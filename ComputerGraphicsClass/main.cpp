//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
#include "main.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//Shaders para la casa 3D
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* vShaderVerdeOscuro = "shaders/shaderverdeoscuro.vert";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};

	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_letraO[] = {
		//X		Y		Z					R		G		B
		-0.125f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.075f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,

		-0.075f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.075f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,

		0.075f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.125f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.075f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,

		0.125f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.125f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.075f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,

		-0.125f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.125f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.125f, 0.10f, 0.0f,				1.0f,	0.0f,	0.0f,

		0.125f, 0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.125f, 0.10f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.125f, 0.10f, 0.0f,				1.0f,	0.0f,	0.0f,

		-0.125f, -0.10f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.125f, -0.10f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,

		0.125f, -0.10f, 0.0f,				1.0f,	0.0f,	0.0f,
		0.125f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				1.0f,	0.0f,	0.0f,
	};

	MeshColor* letraO = new MeshColor();
	letraO->CreateMeshColor(vertices_letraO, 144);  
	meshColorList.push_back(letraO);

	GLfloat vertices_letraC[] = {
		//X		Y		Z					R		G		B
		-0.125f, 0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.075f, 0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				0.0f,	1.0f,	0.0f,

		-0.075f, 0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.075f, -0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				0.0f,	1.0f,	0.0f,

		-0.125f, 0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		0.125f, 0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.125f, 0.10f, 0.0f,				0.0f,	1.0f,	0.0f,

		0.125f, 0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		0.125f, 0.10f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.125f, 0.10f, 0.0f,				0.0f,	1.0f,	0.0f,

		-0.125f, -0.10f, 0.0f,				0.0f,	1.0f,	0.0f,
		0.125f, -0.10f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				0.0f,	1.0f,	0.0f,

		0.125f, -0.10f, 0.0f,				0.0f,	1.0f,	0.0f,
		0.125f, -0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
		-0.125f, -0.15f, 0.0f,				0.0f,	1.0f,	0.0f,
	};

	MeshColor* letraC = new MeshColor();
	letraC->CreateMeshColor(vertices_letraC, 108); 
	meshColorList.push_back(letraC);

	GLfloat vertices_letraT[] = {
		//X		Y		Z					R		G		B
		-0.20f, 0.175f, 0.0f,				0.0f,	0.0f,	1.0f,
		0.20f, 0.175f, 0.0f,				0.0f,	0.0f,	1.0f,
		-0.20f, 0.125f, 0.0f,				0.0f,	0.0f,	1.0f,

		0.20f, 0.175f, 0.0f,				0.0f,	0.0f,	1.0f,
		0.20f, 0.125f, 0.0f,				0.0f,	0.0f,	1.0f,
		-0.20f, 0.125f, 0.0f,				0.0f,	0.0f,	1.0f,

		-0.025f, 0.125f, 0.0f,				0.0f,	0.0f,	1.0f,
		0.025f, 0.125f, 0.0f,				0.0f,	0.0f,	1.0f,
		-0.025f, -0.175f, 0.0f,				0.0f,	0.0f,	1.0f,

		0.025f, 0.125f, 0.0f,				0.0f,	0.0f,	1.0f,
		0.025f, -0.175f, 0.0f,				0.0f,	0.0f,	1.0f,
		-0.025f, -0.175f, 0.0f,				0.0f,	0.0f,	1.0f
	};

	MeshColor* letraT = new MeshColor();
	letraT->CreateMeshColor(vertices_letraT, 72); 
	meshColorList.push_back(letraT);
}


void CreateShaders()
{
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader3->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); 
	shader4->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vShaderVerdeOscuro, fShader);
	shaderList.push_back(*shader7);
}

void DrawNameLetters(glm::mat4& model, GLuint uniformModel, GLuint uniformProjection, glm::mat4& projection)
{
	//letra O 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.4f, 0.0f, -3.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[7]->RenderMeshColor();

	//letra C 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[8]->RenderMeshColor();

	//letra T 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.4f, 0.0f, -3.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[9]->RenderMeshColor();
}

void Draw2DHouse(glm::mat4& model, glm::vec3& houseSize, GLuint uniformModel, GLuint uniformProjection, glm::mat4& projection, glm::vec3& windowSize, glm::vec3& roofSize, glm::vec3& logSize, glm::vec3& treeSize)
{
	//cuadrado rojo - casa
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
	model = glm::scale(model, houseSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[3]->RenderMeshColor();

	//cuadrado verde 1 - ventana izquierda
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.3f, -0.1f, -3.0f));
	model = glm::scale(model, windowSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[2]->RenderMeshColor();

	//cuadrado verde 2 - ventana derecha
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.3f, -0.1f, -3.0f));
	model = glm::scale(model, windowSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[2]->RenderMeshColor();

	//cuadrado verde 3 - puerta
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -0.8f, -3.0f));
	model = glm::scale(model, windowSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[2]->RenderMeshColor();

	//triangulo azul - techo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.6f, -3.0f));
	model = glm::scale(model, roofSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[4]->RenderMeshColor();

	//cuadrado cafe - tronco izquierdo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.8f, -0.9f, -3.0f));
	model = glm::scale(model, logSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[5]->RenderMeshColor();

	//cuadrado cafe - tronco derecho
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.8f, -0.9f, -3.0f));
	model = glm::scale(model, logSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[5]->RenderMeshColor();

	//triangulo verde - arbol izquierdo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.8f, -0.5f, -3.0f));
	model = glm::scale(model, treeSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[6]->RenderMeshColor();

	//triangulo verde - arbol derecho
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.8f, -0.5f, -3.0f));
	model = glm::scale(model, treeSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[6]->RenderMeshColor();
}

void DrawCubeAndPyramid(GLuint& uniformModel, GLuint& uniformProjection, glm::mat4& model, glm::mat4& projection)
{
	//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
	shaderList[0].useShader();
	uniformModel = shaderList[0].getModelLocation();
	uniformProjection = shaderList[0].getProjectLocation();
	angulo += 0.01;
	//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshList[1]->RenderMesh();

	//piramide
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-1.25f, 1.25f, -4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();

	//para usar el shader rojo
	shaderList[2].useShader();
	uniformModel = shaderList[2].getModelLocation();
	uniformProjection = shaderList[2].getProjectLocation();
}

void Draw3DHouse(glm::mat4& model, glm::vec3& houseSize, GLuint uniformModel, GLuint uniformProjection, glm::mat4& projection, glm::vec3& windowSize, glm::vec3& logSize, bool rotate)
{
	if (rotate) 
	{
		angulo += 0.01;
	}

	//shader rojo para la casa
	shaderList[2].useShader();
	uniformModel = shaderList[2].getModelLocation();
	uniformProjection = shaderList[2].getProjectLocation();

	//cubo rojo - casa
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}


	model = glm::scale(model, houseSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshList[1]->RenderMesh(); 

	//shader verde para ventanas y puerta
	shaderList[3].useShader();
	uniformModel = shaderList[3].getModelLocation();
	uniformProjection = shaderList[3].getProjectLocation();

	//cubo verde 1 - ventana izquierda
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.3f, -0.1f, -2.3f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, windowSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshList[1]->RenderMesh(); 

	//cubo verde 2 - ventana derecha
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.3f, -0.1f, -2.3f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, windowSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[1]->RenderMesh(); 

	//cubo verde 3 - puerta
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -0.8f, -2.3f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, windowSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[1]->RenderMesh(); 

	//shader azul para el techo
	shaderList[4].useShader();
	uniformModel = shaderList[4].getModelLocation();
	uniformProjection = shaderList[4].getProjectLocation();

	//piramide azul - techo

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.6f, -3.0f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, glm::vec3(1.5f, 0.7f, 1.5f)); // Techo más grande
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshList[0]->RenderMesh(); 

	//shader cafe para troncos
	shaderList[5].useShader();
	uniformModel = shaderList[5].getModelLocation();
	uniformProjection = shaderList[5].getProjectLocation();

	//cubo cafe - tronco izquierdo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.8f, -0.9f, -3.0f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, logSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshList[1]->RenderMesh(); // Cubo

	//cubo cafe - tronco derecho
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.8f, -0.9f, -3.0f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, logSize);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[1]->RenderMesh(); 

	//shader verde oscuro para arboles
	shaderList[6].useShader();
	uniformModel = shaderList[6].getModelLocation();
	uniformProjection = shaderList[6].getProjectLocation();

	//piramide verde oscuro - arbol izquierdo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.8f, -0.55f, -3.0f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshList[0]->RenderMesh(); 

	//piramide verde oscuro - arbol derecho
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.8f, -0.55f, -3.0f));

	if (rotate) 
	{
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[0]->RenderMesh(); 
}

int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();

	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Definir tamaños para la casa y las ventanas
	glm::vec3 houseSize = glm::vec3(1.2f, 1.5f, 1.2f);
	glm::vec3 windowSize = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 roofSize = glm::vec3(0.8f, 0.4f, 0.8f);
	glm::vec3 logSize = glm::vec3(0.15f, 0.2f, 0.15f);
	glm::vec3 treeSize = glm::vec3(0.18f, 0.3f, 0.18f);
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas

		//Dibujo de la casa: Practica 2 ejercicio
		//DrawHouse(model, houseSize, uniformModel, uniformProjection, projection, windowSize, roofSize, logSize, treeSize);

		//Dibujo de letras iniciales del nombre: Practica 2
		//DrawNameLetters(model, uniformModel, uniformProjection, projection);

		//DrawCubeAndPyramid(uniformModel, uniformProjection, model, projection);

		Draw3DHouse(model, houseSize, uniformModel, uniformProjection, projection, windowSize, logSize, false);

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}

// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/