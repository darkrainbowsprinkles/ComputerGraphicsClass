//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
#include "practica3.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

void DrawHouse(glm::mat4& model, glm::vec3& color, GLuint uniformColor, GLuint uniformModel)
{
	//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
	model = glm::mat4(1.0f);
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	//Opcional duplicar esta traslación inicial para posicionar en -Z a los objetos en el mismo punto
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

	// estructura
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();

	// piso
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.0f, -0.8f, -2.5f));
	model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();

	// techo
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.0f, 1.1f, -1.4f));
	model = glm::scale(model, glm::vec3(1.8f, 0.8f, 4.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[1]->RenderMesh();

	// ventana 1
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(-0.4f, 0.35f, -1.8f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();


	// ventana 2
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.4f, 0.35f, -1.8f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();

	// ventana 3
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(-0.7f, 0.35f, -2.8f));
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();


	// ventana 4
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(-0.7f, 0.35f, -2.1f));
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();


	// ventana 3
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.7f, 0.35f, -2.8f));
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();


	// ventana 4
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.7f, 0.35f, -2.1f));
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMesh();


	// puerta
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.0f, -0.45f, -1.8f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[0]->RenderMeshGeometry();

	// tronco 1
	model = glm::mat4(1.0f);
	color = glm::vec3(0.478f, 0.255f, 0.067f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(-1.5f, -0.6f, -2.3f));
	model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[2]->RenderMeshGeometry();


	// tronco 2
	model = glm::mat4(1.0f);
	color = glm::vec3(0.478f, 0.255f, 0.067f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(1.5f, -0.6f, -2.3f));
	model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[2]->RenderMeshGeometry();


	// arbol 1
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 0.5f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(-1.5f, -0.0f, -2.3f));
	model = glm::scale(model, glm::vec3(0.2f, 0.8f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[3]->RenderMeshGeometry();

	// arbol 2
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 0.5f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(1.5f, -0.0f, -2.3f));
	model = glm::scale(model, glm::vec3(0.2f, 0.8f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	meshList[3]->RenderMeshGeometry();

	// esfera trasera
	model = glm::mat4(1.0f);
	color = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	sp.render();
}

const float pieceSize = 0.4f;
const float yOffset = -0.07f;
const float faceAngle = 63.4f;
const float pieceSpacing = 0.15f;
const float faceThickness = 0.02f;
const float faceDepthOffset = 0.2f;
const float backFaceOffset = 0.1f;
const float flipAngle = 180.0f;
const float perpendicularAngle = 90.0f;

void DrawPiece(glm::mat4& model, GLuint uniformColor, GLuint uniformModel, glm::vec3 translation, bool flipped = false)
{
	glm::vec3 pieceColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(pieceColor));

	glm::mat4 piece = model;

	piece = glm::translate(piece, translation);

	if (flipped)
	{
		piece = glm::rotate(piece, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	piece = glm::scale(piece, glm::vec3(pieceSize, pieceSize, pieceSize));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(piece));
	meshList[4]->RenderMesh();
}

void DrawRedFaces(GLuint uniformColor, glm::mat4& model, GLuint uniformModel)
{
	// Red faces
	glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(redColor));

	glm::mat4 redFace1 = model;
	redFace1 = glm::translate(redFace1, glm::vec3(-pieceSize - pieceSpacing, yOffset, 0.0f));
	redFace1 = glm::rotate(redFace1, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace1 = glm::scale(redFace1, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace1));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace2 = model;
	redFace2 = glm::translate(redFace2, glm::vec3(-pieceSize - pieceSpacing, yOffset, -pieceSize));
	redFace2 = glm::rotate(redFace2, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace2 = glm::scale(redFace2, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace2));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace3 = model;
	redFace3 = glm::translate(redFace3, glm::vec3(-pieceSize - pieceSpacing, yOffset, -pieceSize * 2));
	redFace3 = glm::rotate(redFace3, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace3 = glm::scale(redFace3, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace3));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace4 = model;
	redFace4 = glm::translate(redFace4, glm::vec3(-(pieceSize / 2) - pieceSpacing, yOffset + pieceSize, -pieceSize / 2));
	redFace4 = glm::rotate(redFace4, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace4 = glm::scale(redFace4, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace4));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace5 = model;
	redFace5 = glm::translate(redFace5, glm::vec3(-(pieceSize / 2) - pieceSpacing, yOffset + pieceSize, -(pieceSize / 2) * 3));
	redFace5 = glm::rotate(redFace5, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace5 = glm::scale(redFace5, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace5));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace6 = model;
	redFace6 = glm::translate(redFace6, glm::vec3(-pieceSpacing, yOffset + (pieceSize * 2), -pieceSize));
	redFace6 = glm::rotate(redFace6, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace6 = glm::scale(redFace6, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace6));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace7 = model;
	redFace7 = glm::translate(redFace7, glm::vec3(-pieceSize - (pieceSpacing / 2), yOffset * 2 + (pieceSize / 2), -pieceSize / 2));
	redFace7 = glm::rotate(redFace7, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace7 = glm::rotate(redFace7, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace7 = glm::scale(redFace7, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace7));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace8 = model;
	redFace8 = glm::translate(redFace8, glm::vec3(-pieceSize - (pieceSpacing / 2), yOffset * 2 + (pieceSize / 2), -(pieceSize / 2) * 3));
	redFace8 = glm::rotate(redFace8, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace8 = glm::rotate(redFace8, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace8 = glm::scale(redFace8, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace8));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 redFace9 = model;
	redFace9 = glm::translate(redFace9, glm::vec3(-(pieceSize / 2) - (pieceSpacing / 2), yOffset * 2 + (pieceSize / 2) * 3, -pieceSize));
	redFace9 = glm::rotate(redFace9, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace9 = glm::rotate(redFace9, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	redFace9 = glm::scale(redFace9, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(redFace9));
	meshList[2]->RenderMeshGeometry();
}

void DrawGreenFaces(GLuint uniformColor, glm::mat4& model, GLuint uniformModel)
{
	glm::vec3 greenFaceColor = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(greenFaceColor));

	glm::mat4 greenFace1 = model;
	greenFace1 = glm::translate(greenFace1, glm::vec3(-pieceSize, yOffset, pieceSpacing));
	greenFace1 = glm::rotate(greenFace1, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace1 = glm::rotate(greenFace1, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace1 = glm::scale(greenFace1, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace1));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace2 = model;
	greenFace2 = glm::translate(greenFace2, glm::vec3(0.0f, yOffset, pieceSpacing));
	greenFace2 = glm::rotate(greenFace2, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace2 = glm::rotate(greenFace2, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace2 = glm::scale(greenFace2, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace2));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace3 = model;
	greenFace3 = glm::translate(greenFace3, glm::vec3(pieceSize, yOffset, pieceSpacing));
	greenFace3 = glm::rotate(greenFace3, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace3 = glm::rotate(greenFace3, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace3 = glm::scale(greenFace3, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace3));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace4 = model;
	greenFace4 = glm::translate(greenFace4, glm::vec3(-pieceSize / 2, yOffset + pieceSize, -pieceSize / 2 + pieceSpacing));
	greenFace4 = glm::rotate(greenFace4, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace4 = glm::rotate(greenFace4, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace4 = glm::scale(greenFace4, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace4));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace5 = model;
	greenFace5 = glm::translate(greenFace5, glm::vec3(pieceSize / 2, yOffset + pieceSize, -pieceSize / 2 + pieceSpacing));
	greenFace5 = glm::rotate(greenFace5, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace5 = glm::rotate(greenFace5, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace5 = glm::scale(greenFace5, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace5));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace6 = model;
	greenFace6 = glm::translate(greenFace6, glm::vec3(0.0f, yOffset + (pieceSize * 2), -pieceSize + pieceSpacing));
	greenFace6 = glm::rotate(greenFace6, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace6 = glm::rotate(greenFace6, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace6 = glm::scale(greenFace6, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace6));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace7 = model;
	greenFace7 = glm::translate(greenFace7, glm::vec3(-pieceSize / 2, yOffset * 2 + (pieceSize / 2), backFaceOffset));
	greenFace7 = glm::rotate(greenFace7, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace7 = glm::rotate(greenFace7, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace7 = glm::rotate(greenFace7, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace7 = glm::scale(greenFace7, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace7));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace8 = model;
	greenFace8 = glm::translate(greenFace8, glm::vec3(pieceSize / 2, yOffset * 2 + (pieceSize / 2), backFaceOffset));
	greenFace8 = glm::rotate(greenFace8, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace8 = glm::rotate(greenFace8, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace8 = glm::rotate(greenFace8, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace8 = glm::scale(greenFace8, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace8));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 greenFace9 = model;
	greenFace9 = glm::translate(greenFace9, glm::vec3(0.0f, yOffset * 2 + (pieceSize * 1.5f), -backFaceOffset));
	greenFace9 = glm::rotate(greenFace9, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	greenFace9 = glm::rotate(greenFace9, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace9 = glm::rotate(greenFace9, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	greenFace9 = glm::scale(greenFace9, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(greenFace9));
	meshList[2]->RenderMeshGeometry();
}

void DrawBlueFaces(GLuint uniformColor, glm::mat4& model, GLuint uniformModel)
{
	glm::vec3 blueFaceColor = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(blueFaceColor));

	glm::mat4 blueFace1 = model;
	blueFace1 = glm::translate(blueFace1, glm::vec3(pieceSize + pieceSpacing, yOffset, 0.0f));
	blueFace1 = glm::rotate(blueFace1, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace1 = glm::rotate(blueFace1, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace1 = glm::scale(blueFace1, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace1));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace2 = model;
	blueFace2 = glm::translate(blueFace2, glm::vec3(pieceSize + pieceSpacing, yOffset, -pieceSize));
	blueFace2 = glm::rotate(blueFace2, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace2 = glm::rotate(blueFace2, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace2 = glm::scale(blueFace2, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace2));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace3 = model;
	blueFace3 = glm::translate(blueFace3, glm::vec3(pieceSize + pieceSpacing, yOffset, -pieceSize * 2));
	blueFace3 = glm::rotate(blueFace3, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace3 = glm::rotate(blueFace3, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace3 = glm::scale(blueFace3, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace3));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace4 = model;
	blueFace4 = glm::translate(blueFace4, glm::vec3((pieceSize / 2) + pieceSpacing, yOffset + pieceSize, -pieceSize / 2));
	blueFace4 = glm::rotate(blueFace4, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace4 = glm::rotate(blueFace4, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace4 = glm::scale(blueFace4, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace4));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace5 = model;
	blueFace5 = glm::translate(blueFace5, glm::vec3((pieceSize / 2) + pieceSpacing, yOffset + pieceSize, -(pieceSize / 2) * 3));
	blueFace5 = glm::rotate(blueFace5, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace5 = glm::rotate(blueFace5, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace5 = glm::scale(blueFace5, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace5));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace6 = model;
	blueFace6 = glm::translate(blueFace6, glm::vec3(pieceSpacing, yOffset + (pieceSize * 2), -pieceSize));
	blueFace6 = glm::rotate(blueFace6, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace6 = glm::rotate(blueFace6, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace6 = glm::scale(blueFace6, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace6));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace7 = model;
	blueFace7 = glm::translate(blueFace7, glm::vec3((pieceSize / 2) + pieceSpacing * 2, yOffset * 2 + (pieceSize / 2), -(pieceSize * 2) + faceDepthOffset));
	blueFace7 = glm::rotate(blueFace7, glm::radians(flipAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	blueFace7 = glm::rotate(blueFace7, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace7 = glm::rotate(blueFace7, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace7 = glm::scale(blueFace7, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace7));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace8 = model;
	blueFace8 = glm::translate(blueFace8, glm::vec3((pieceSize / 2) + pieceSpacing * 2, yOffset * 2 + (pieceSize / 2), -(pieceSize)+faceDepthOffset));
	blueFace8 = glm::rotate(blueFace8, glm::radians(flipAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	blueFace8 = glm::rotate(blueFace8, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace8 = glm::rotate(blueFace8, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace8 = glm::scale(blueFace8, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace8));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 blueFace9 = model;
	blueFace9 = glm::translate(blueFace9, glm::vec3(pieceSpacing * 2, yOffset * 2 + (pieceSize * 1.5f), -(pieceSize * 2) + pieceSize));
	blueFace9 = glm::rotate(blueFace9, glm::radians(flipAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	blueFace9 = glm::rotate(blueFace9, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace9 = glm::rotate(blueFace9, glm::radians(faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	blueFace9 = glm::scale(blueFace9, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(blueFace9));
	meshList[2]->RenderMeshGeometry();
}

void DrawYellowFaces(GLuint uniformColor, glm::mat4& model, GLuint uniformModel)
{
	glm::vec3 yellowFaceColor = glm::vec3(1.0f, 1.0f, 0.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(yellowFaceColor));

	glm::mat4 yellowFace1 = model;
	yellowFace1 = glm::translate(yellowFace1, glm::vec3(pieceSize, yOffset, -(pieceSize * 2) - pieceSpacing));
	yellowFace1 = glm::rotate(yellowFace1, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace1 = glm::rotate(yellowFace1, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace1 = glm::rotate(yellowFace1, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace1 = glm::scale(yellowFace1, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace1));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace2 = model;
	yellowFace2 = glm::translate(yellowFace2, glm::vec3(0.0f, yOffset, -(pieceSize * 2) - pieceSpacing));
	yellowFace2 = glm::rotate(yellowFace2, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace2 = glm::rotate(yellowFace2, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace2 = glm::rotate(yellowFace2, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace2 = glm::scale(yellowFace2, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace2));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace3 = model;
	yellowFace3 = glm::translate(yellowFace3, glm::vec3(-pieceSize, yOffset, -(pieceSize * 2) - pieceSpacing));
	yellowFace3 = glm::rotate(yellowFace3, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace3 = glm::rotate(yellowFace3, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace3 = glm::rotate(yellowFace3, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace3 = glm::scale(yellowFace3, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace3));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace4 = model;
	yellowFace4 = glm::translate(yellowFace4, glm::vec3(pieceSize / 2, yOffset + pieceSize, -(pieceSize * 1.5f) - pieceSpacing));
	yellowFace4 = glm::rotate(yellowFace4, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace4 = glm::rotate(yellowFace4, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace4 = glm::rotate(yellowFace4, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace4 = glm::scale(yellowFace4, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace4));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace5 = model;
	yellowFace5 = glm::translate(yellowFace5, glm::vec3(-pieceSize / 2, yOffset + pieceSize, -(pieceSize * 1.5f) - pieceSpacing));
	yellowFace5 = glm::rotate(yellowFace5, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace5 = glm::rotate(yellowFace5, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace5 = glm::rotate(yellowFace5, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace5 = glm::scale(yellowFace5, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace5));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace6 = model;
	yellowFace6 = glm::translate(yellowFace6, glm::vec3(0.0f, yOffset + (pieceSize * 2), -(pieceSize)-pieceSpacing));
	yellowFace6 = glm::rotate(yellowFace6, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace6 = glm::rotate(yellowFace6, glm::radians(flipAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace6 = glm::rotate(yellowFace6, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace6 = glm::scale(yellowFace6, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace6));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace7 = model;
	yellowFace7 = glm::translate(yellowFace7, glm::vec3(-pieceSize / 2, yOffset * 2 + (pieceSize / 2), -(pieceSize * 2) - backFaceOffset));
	yellowFace7 = glm::rotate(yellowFace7, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace7 = glm::rotate(yellowFace7, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace7 = glm::scale(yellowFace7, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace7));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace8 = model;
	yellowFace8 = glm::translate(yellowFace8, glm::vec3(pieceSize / 2, yOffset * 2 + (pieceSize / 2), -(pieceSize * 2) - backFaceOffset));
	yellowFace8 = glm::rotate(yellowFace8, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace8 = glm::rotate(yellowFace8, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace8 = glm::scale(yellowFace8, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace8));
	meshList[2]->RenderMeshGeometry();

	glm::mat4 yellowFace9 = model;
	yellowFace9 = glm::translate(yellowFace9, glm::vec3(0.0f, yOffset * 2 + (pieceSize * 1.5f), -(pieceSize * 2) + backFaceOffset));
	yellowFace9 = glm::rotate(yellowFace9, glm::radians(perpendicularAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	yellowFace9 = glm::rotate(yellowFace9, glm::radians(-faceAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	yellowFace9 = glm::scale(yellowFace9, glm::vec3(pieceSize - pieceSpacing, faceThickness, pieceSize - faceDepthOffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(yellowFace9));
	meshList[2]->RenderMeshGeometry();
}

void DrawWhiteFaces(GLuint uniformColor, glm::mat4& model, GLuint uniformModel)
{
	glm::vec3 whiteFaceColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(whiteFaceColor));

	glm::mat4 whiteFace1 = model;
	whiteFace1 = glm::translate(whiteFace1, glm::vec3(0.0f, -pieceSize / 2, 0.0f));
	whiteFace1 = glm::scale(whiteFace1, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace1));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace2 = model;
	whiteFace2 = glm::translate(whiteFace2, glm::vec3(-pieceSize, -pieceSize / 2, 0.0f));
	whiteFace2 = glm::scale(whiteFace2, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace2));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace3 = model;
	whiteFace3 = glm::translate(whiteFace3, glm::vec3(pieceSize, -pieceSize / 2, 0.0f));
	whiteFace3 = glm::scale(whiteFace3, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace3));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace4 = model;
	whiteFace4 = glm::translate(whiteFace4, glm::vec3(0.0f, -pieceSize / 2, -pieceSize));
	whiteFace4 = glm::scale(whiteFace4, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace4));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace5 = model;
	whiteFace5 = glm::translate(whiteFace5, glm::vec3(-pieceSize, -pieceSize / 2, -pieceSize));
	whiteFace5 = glm::scale(whiteFace5, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace5));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace6 = model;
	whiteFace6 = glm::translate(whiteFace6, glm::vec3(pieceSize, -pieceSize / 2, -pieceSize));
	whiteFace6 = glm::scale(whiteFace6, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace6));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace7 = model;
	whiteFace7 = glm::translate(whiteFace7, glm::vec3(0.0f, -pieceSize / 2, -pieceSize * 2));
	whiteFace7 = glm::scale(whiteFace7, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace7));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace8 = model;
	whiteFace8 = glm::translate(whiteFace8, glm::vec3(-pieceSize, -pieceSize / 2, -pieceSize * 2));
	whiteFace8 = glm::scale(whiteFace8, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace8));
	meshList[0]->RenderMesh();

	glm::mat4 whiteFace9 = model;
	whiteFace9 = glm::translate(whiteFace9, glm::vec3(pieceSize, -pieceSize / 2, -pieceSize * 2));
	whiteFace9 = glm::scale(whiteFace9, glm::vec3(pieceSize - 0.04f, faceThickness, pieceSize - 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(whiteFace9));
	meshList[0]->RenderMesh();
}

void DrawPyraminx(glm::mat4& model,  GLuint uniformColor, GLuint uniformModel)
{
	// Front bottom row
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize, 0.0f, 0.0f));
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(0.0f, 0.0f, 0.0f));
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize, 0.0f, 0.0f));

	// Flipped middle pieces
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize / 2, 0.0f, -pieceSize / 2), true);
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize / 2, 0.0f, -pieceSize / 2), true);
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize / 2, 0.0f, -(pieceSize / 2)*3), true);
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize / 2, 0.0f, -(pieceSize / 2)*3), true);

	// Left bottom row
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize, 0.0f, -pieceSize));
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize, 0.0f, -pieceSize * 2));

	// Right bottom row
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize, 0.0f, -pieceSize));
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize, 0.0f, -pieceSize * 2));

	// Back bottom piece
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(0.0f, 0.0f, -pieceSize * 2));

	// Front middle row
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize / 2, pieceSize, -pieceSize / 2));
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize / 2, pieceSize, -pieceSize / 2));

	// Fliped top piece
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(0.0f, pieceSize, -pieceSize), true);

	// Left middle piece
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(-pieceSize / 2, pieceSize, -(pieceSize / 2) * 3));

	// Right middle piece
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(pieceSize / 2, pieceSize, -(pieceSize / 2) * 3));

	// Top piece
	DrawPiece(model, uniformColor, uniformModel, glm::vec3(0.0f, pieceSize * 2, -pieceSize));

	// Faces
	DrawRedFaces(uniformColor, model, uniformModel);
	DrawGreenFaces(uniformColor, model, uniformModel);
	DrawBlueFaces(uniformColor, model, uniformModel);
	DrawYellowFaces(uniformColor, model, uniformModel);
	DrawWhiteFaces(uniformColor, model, uniformModel);
}

int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa
	// Maxima resolucion: 360
	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(3, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		// estas 2 lineas solo se llaman una vez
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[1]->RenderMesh(); //dibuja cubo y pirámide triangular, render mesh non usa TRIANGLE_FAN
		//meshList[2]->RenderMeshGeometry(); // Render mesh geometry usa TRIANGLE_FAN
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera

		//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//DrawHouse(model, color, uniformColor, uniformModel);

		DrawPyraminx(model, uniformColor, uniformModel);

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		