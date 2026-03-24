#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "main.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shaderpersonaje.vert";
static const char* fShaderColor = "shaders/shaderpersonaje.frag";

void CreateShaders()
{
	Shader *shader1 = new Shader(); 
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}

void CrearFiguras()
{
	// 0. Cuadrado verde para la tunica
	GLfloat cuadrado_verde[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.6f, 0.2f
	};
	MeshColor* m0 = new MeshColor();
	m0->CreateMeshColor(cuadrado_verde, 36);
	meshColorList.push_back(m0);

	// 1. Triangulo verde para el gorro
	GLfloat triangulo_verde[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.6f, 0.2f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.6f, 0.2f
	};
	MeshColor* m1 = new MeshColor();
	m1->CreateMeshColor(triangulo_verde, 18);
	meshColorList.push_back(m1);

	// 2. Cuadrado piel
	GLfloat cuadrado_piel[] = {
		-0.5f, -0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		 0.5f, -0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		 0.5f,  0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		 0.5f,  0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		-0.5f,  0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		-0.5f, -0.5f, 0.0f,  0.9f, 0.7f, 0.5f
	};
	MeshColor* m2 = new MeshColor();
	m2->CreateMeshColor(cuadrado_piel, 36);
	meshColorList.push_back(m2);

	// 3. Cuadrado cafe
	GLfloat cuadrado_cafe[] = {
		-0.5f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.5f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		-0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		-0.5f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f
	};
	MeshColor* m3 = new MeshColor();
	m3->CreateMeshColor(cuadrado_cafe, 36);
	meshColorList.push_back(m3);

	// 4. Triangulo cafe invertido
	GLfloat triangulo_cafe[] = {
		-0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.0f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f
	};
	MeshColor* m4 = new MeshColor();
	m4->CreateMeshColor(triangulo_cafe, 18);
	meshColorList.push_back(m4);

	// 5. Triangulo gris
	GLfloat triangulo_gris[] = {
		-0.5f, -0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		 0.5f, -0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		 0.0f,  0.5f, 0.0f,  0.7f, 0.8f, 0.9f
	};
	MeshColor* m5 = new MeshColor();
	m5->CreateMeshColor(triangulo_gris, 18);
	meshColorList.push_back(m5);

	// 6. Cuadrado amarillo
	GLfloat cuadrado_amarillo[] = {
		-0.5f, -0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		 0.5f, -0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		 0.5f,  0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		 0.5f,  0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		-0.5f,  0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		-0.5f, -0.5f, 0.0f,  0.9f, 0.8f, 0.2f
	};
	MeshColor* m6 = new MeshColor();
	m6->CreateMeshColor(cuadrado_amarillo, 36);
	meshColorList.push_back(m6);

	// 7. Triangulo piel
	GLfloat triangulo_piel[] = {
		-0.5f, -0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		 0.5f, -0.5f, 0.0f,  0.9f, 0.7f, 0.5f,
		 0.0f,  0.5f, 0.0f,  0.9f, 0.7f, 0.5f
	};
	MeshColor* m7 = new MeshColor();
	m7->CreateMeshColor(triangulo_piel, 18);
	meshColorList.push_back(m7);

	// 8. Cuadrado blanco
	GLfloat cuadrado_blanco[] = {
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f
	};
	MeshColor* m8 = new MeshColor();
	m8->CreateMeshColor(cuadrado_blanco, 36);
	meshColorList.push_back(m8);

	// 9. Cuadrado azul
	GLfloat cuadrado_azul[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.8f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.8f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.8f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.8f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.8f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.8f
	};
	MeshColor* m9 = new MeshColor();
	m9->CreateMeshColor(cuadrado_azul, 36);
	meshColorList.push_back(m9);

	// 10. Triangulo amarillo
	GLfloat triangulo_amarillo[] = {
		-0.5f, -0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		 0.5f, -0.5f, 0.0f,  0.9f, 0.8f, 0.2f,
		 0.0f,  0.5f, 0.0f,  0.9f, 0.8f, 0.2f
	};
	MeshColor* m10 = new MeshColor();
	m10->CreateMeshColor(triangulo_amarillo, 18);
	meshColorList.push_back(m10);

	// 11. Cuadrado cafe oscuro
	GLfloat cuadrado_cafe_oscuro[] = {
		-0.5f, -0.5f, 0.0f,  0.3f, 0.15f, 0.05f,
		 0.5f, -0.5f, 0.0f,  0.3f, 0.15f, 0.05f,
		 0.5f,  0.5f, 0.0f,  0.3f, 0.15f, 0.05f,
		 0.5f,  0.5f, 0.0f,  0.3f, 0.15f, 0.05f,
		-0.5f,  0.5f, 0.0f,  0.3f, 0.15f, 0.05f,
		-0.5f, -0.5f, 0.0f,  0.3f, 0.15f, 0.05f
	};
	MeshColor* m11 = new MeshColor();
	m11->CreateMeshColor(cuadrado_cafe_oscuro, 36);
	meshColorList.push_back(m11);

	// 12. Cuadrado rojo
	GLfloat cuadrado_rojo[] = {
		-0.5f, -0.5f, 0.0f,  0.8f, 0.1f, 0.1f,
		 0.5f, -0.5f, 0.0f,  0.8f, 0.1f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.8f, 0.1f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.8f, 0.1f, 0.1f,
		-0.5f,  0.5f, 0.0f,  0.8f, 0.1f, 0.1f,
		-0.5f, -0.5f, 0.0f,  0.8f, 0.1f, 0.1f
	};
	MeshColor* m12 = new MeshColor();
	m12->CreateMeshColor(cuadrado_rojo, 36);
	meshColorList.push_back(m12);

	// 13. Cuadrado gris
	GLfloat cuadrado_gris[] = {
		-0.5f, -0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		 0.5f, -0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		 0.5f,  0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		 0.5f,  0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		-0.5f,  0.5f, 0.0f,  0.7f, 0.8f, 0.9f,
		-0.5f, -0.5f, 0.0f,  0.7f, 0.8f, 0.9f
	};
	MeshColor* m13 = new MeshColor();
	m13->CreateMeshColor(cuadrado_gris, 36);
	meshColorList.push_back(m13);

	// 14. Cuadrado negro
	GLfloat cuadrado_negro[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f
	};
	MeshColor* m14 = new MeshColor();
	m14->CreateMeshColor(cuadrado_negro, 36);
	meshColorList.push_back(m14);

	// 15. Cuadrado dorado
	GLfloat cuadrado_dorado[] = {
		-0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.0f
	};
	MeshColor* m15 = new MeshColor();
	m15->CreateMeshColor(cuadrado_dorado, 36);
	meshColorList.push_back(m15);

	// 16. Cuadrado azul oscuro
	GLfloat cuadrado_azul_oscuro[] = {
		-0.5f, -0.5f, 0.0f,  0.1f, 0.2f, 0.5f,
		 0.5f, -0.5f, 0.0f,  0.1f, 0.2f, 0.5f,
		 0.5f,  0.5f, 0.0f,  0.1f, 0.2f, 0.5f,
		 0.5f,  0.5f, 0.0f,  0.1f, 0.2f, 0.5f,
		-0.5f,  0.5f, 0.0f,  0.1f, 0.2f, 0.5f,
		-0.5f, -0.5f, 0.0f,  0.1f, 0.2f, 0.5f
	};
	MeshColor* m16 = new MeshColor();
	m16->CreateMeshColor(cuadrado_azul_oscuro, 36);
	meshColorList.push_back(m16);

	// 17. Triangulo piel sombreada
	GLfloat triangulo_piel_sombreada[] = {
		-0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.0f,  0.5f, 0.0f,  0.8f, 0.6f, 0.4f
	};
	MeshColor* m17 = new MeshColor();
	m17->CreateMeshColor(triangulo_piel_sombreada, 18);
	meshColorList.push_back(m17);

	// 18. Cuadrado gris oscuro
	GLfloat cuadrado_gris_oscuro[] = {
		-0.5f, -0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		 0.5f, -0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		 0.5f,  0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		 0.5f,  0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		-0.5f,  0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		-0.5f, -0.5f, 0.0f,  0.5f, 0.6f, 0.7f
	};
	MeshColor* m18 = new MeshColor();
	m18->CreateMeshColor(cuadrado_gris_oscuro, 36);
	meshColorList.push_back(m18);

	// 19. Triangulo rectangulo gris oscuro
	GLfloat triangulo_mitad_gris_oscuro[] = {
		-0.5f, -0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		 0.0f, -0.5f, 0.0f,  0.5f, 0.6f, 0.7f,
		 0.0f,  0.5f, 0.0f,  0.5f, 0.6f, 0.7f
	};
	MeshColor* m19 = new MeshColor();
	m19->CreateMeshColor(triangulo_mitad_gris_oscuro, 18);
	meshColorList.push_back(m19);

	// 20. Cuadrado piel sombreada
	GLfloat cuadrado_piel_sombreada[] = {
		-0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		-0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		-0.5f, -0.5f, 0.0f,  0.8f, 0.6f, 0.4f
	};
	MeshColor* m20 = new MeshColor();
	m20->CreateMeshColor(cuadrado_piel_sombreada, 36);
	meshColorList.push_back(m20);

	// 21. Triangulo rectangulo piel sombreada
	GLfloat mandibula_izquierda_sombreada[] = {
		-0.5f,  0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.0f, -0.5f, 0.0f,  0.8f, 0.6f, 0.4f,
		 0.0f,  0.5f, 0.0f,  0.8f, 0.6f, 0.4f
	};
	MeshColor* m21 = new MeshColor();
	m21->CreateMeshColor(mandibula_izquierda_sombreada, 18);
	meshColorList.push_back(m21);

	// 22. Cuadrado verde oscuro
	GLfloat cuadrado_verde_oscuro[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.4f, 0.1f
	};
	MeshColor* m22 = new MeshColor();
	m22->CreateMeshColor(cuadrado_verde_oscuro, 36);
	meshColorList.push_back(m22);

	// 23. Medio triangulo verde oscuro
	GLfloat medio_triangulo_verde_oscuro[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		 0.0f, -0.5f, 0.0f,  0.0f, 0.4f, 0.1f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.4f, 0.1f
	};
	MeshColor* m23 = new MeshColor();
	m23->CreateMeshColor(medio_triangulo_verde_oscuro, 18);
	meshColorList.push_back(m23);

	// 24. Cuadrado cafe oscuro
	GLfloat cuadrado_cafe_sombreado[] = {
		-0.5f, -0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.5f, -0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.5f,  0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.5f,  0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		-0.5f,  0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		-0.5f, -0.5f, 0.0f,  0.25f, 0.15f, 0.05f
	};
	MeshColor* m24 = new MeshColor();
	m24->CreateMeshColor(cuadrado_cafe_sombreado, 36);
	meshColorList.push_back(m24);

	// 25. Cuadrado amarillo oscuro
	GLfloat cuadrado_amarillo_sombreado[] = {
		-0.5f, -0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		 0.5f, -0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		 0.5f,  0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		-0.5f,  0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		-0.5f, -0.5f, 0.0f,  0.6f, 0.5f, 0.1f
	};
	MeshColor* m25 = new MeshColor();
	m25->CreateMeshColor(cuadrado_amarillo_sombreado, 36);
	meshColorList.push_back(m25);

	// 26. Medio triangulo amarillo oscuro
	GLfloat triangulo_amarillo_sombreado[] = {
		-0.5f, -0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		 0.0f, -0.5f, 0.0f,  0.6f, 0.5f, 0.1f,
		 0.0f,  0.5f, 0.0f,  0.6f, 0.5f, 0.1f
	};
	MeshColor* m26 = new MeshColor();
	m26->CreateMeshColor(triangulo_amarillo_sombreado, 18);
	meshColorList.push_back(m26);

	// 27. Medio triangulo cafe oscuro
	GLfloat medio_triangulo_cafe_sombreado[] = {
		-0.5f, -0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.0f, -0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.0f,  0.5f, 0.0f,  0.25f, 0.15f, 0.05f
	};
	MeshColor* m27 = new MeshColor();
	m27->CreateMeshColor(medio_triangulo_cafe_sombreado, 18);
	meshColorList.push_back(m27);

	// 28. Medio triangulo invertido cafe oscuro
	GLfloat medio_triangulo_inv_cafe_sombreado[] = {
		-0.5f,  0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.0f, -0.5f, 0.0f,  0.25f, 0.15f, 0.05f,
		 0.0f,  0.5f, 0.0f,  0.25f, 0.15f, 0.05f
	};
	MeshColor* m28 = new MeshColor();
	m28->CreateMeshColor(medio_triangulo_inv_cafe_sombreado, 18);
	meshColorList.push_back(m28);

	// 29. Ceja detallada
	GLfloat ceja_cafe[] = {
		-0.3f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.5f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.3f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		 0.3f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		-0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.1f,
		-0.3f, -0.5f, 0.0f,  0.5f, 0.3f, 0.1f
	};
	MeshColor* m29 = new MeshColor();
	m29->CreateMeshColor(ceja_cafe, 36);
	meshColorList.push_back(m29);

	// 30. Triangulo piel muy oscura
	GLfloat triangulo_piel_muy_oscura[] = {
		-0.5f, -0.5f, 0.0f,  0.7f, 0.5f, 0.3f,
		 0.5f, -0.5f, 0.0f,  0.7f, 0.5f, 0.3f,
		 0.0f,  0.5f, 0.0f,  0.7f, 0.5f, 0.3f
	};
	MeshColor* m30 = new MeshColor();
	m30->CreateMeshColor(triangulo_piel_muy_oscura, 18);
	meshColorList.push_back(m30);
}

// Funcion auxiliar para dibujar
void DibujarParte(glm::mat4 modelBase, GLuint uniformModel, glm::vec3 pos, glm::vec3 scale, float angleZ, int meshIndex) {
	glm::mat4 model = modelBase;
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshColorList[meshIndex]->RenderMeshColor();
}

void DibujarPersonaje(glm::mat4 model, GLuint uniformModel)
{
	// Falda
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.37f, 0.005f), glm::vec3(0.42f, 0.25f, 1.0f), 0.0f, 1);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.37f, 0.006f), glm::vec3(0.42f, 0.25f, 1.0f), 0.0f, 23);

	// Piernas
	DibujarParte(model, uniformModel, glm::vec3(-0.10f, -0.58f, -0.01f), glm::vec3(0.11f, 0.3f, 1.0f), 0.0f, 2);
	DibujarParte(model, uniformModel, glm::vec3(0.10f, -0.58f, -0.01f), glm::vec3(0.11f, 0.3f, 1.0f), 0.0f, 2);
	// Sombras piernas
	DibujarParte(model, uniformModel, glm::vec3(-0.1275f, -0.58f, -0.009f), glm::vec3(0.055f, 0.3f, 1.0f), 0.0f, 20);
	DibujarParte(model, uniformModel, glm::vec3(0.0725f, -0.58f, -0.009f), glm::vec3(0.055f, 0.3f, 1.0f), 0.0f, 20);

	// Botas arriba
	DibujarParte(model, uniformModel, glm::vec3(-0.10f, -0.68f, 0.015f), glm::vec3(0.19f, 0.06f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(-0.1475f, -0.68f, 0.016f), glm::vec3(0.095f, 0.06f, 1.0f), 0.0f, 24); 
	DibujarParte(model, uniformModel, glm::vec3(0.10f, -0.68f, 0.015f), glm::vec3(0.19f, 0.06f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.0525f, -0.68f, 0.016f), glm::vec3(0.095f, 0.06f, 1.0f), 0.0f, 24); 

	// Botas medio
	DibujarParte(model, uniformModel, glm::vec3(-0.10f, -0.79f, 0.01f), glm::vec3(0.15f, 0.20f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(-0.1375f, -0.79f, 0.011f), glm::vec3(0.075f, 0.20f, 1.0f), 0.0f, 24); 
	DibujarParte(model, uniformModel, glm::vec3(0.10f, -0.79f, 0.01f), glm::vec3(0.15f, 0.20f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.0625f, -0.79f, 0.011f), glm::vec3(0.075f, 0.20f, 1.0f), 0.0f, 24); 

	// Botas base
	DibujarParte(model, uniformModel, glm::vec3(-0.10f, -0.91f, 0.01f), glm::vec3(0.15f, 0.04f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(-0.1375f, -0.91f, 0.011f), glm::vec3(0.075f, 0.04f, 1.0f), 0.0f, 24); 
	DibujarParte(model, uniformModel, glm::vec3(0.10f, -0.91f, 0.01f), glm::vec3(0.15f, 0.04f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.0625f, -0.91f, 0.011f), glm::vec3(0.075f, 0.04f, 1.0f), 0.0f, 24); 

	// Puntas botas
	DibujarParte(model, uniformModel, glm::vec3(-0.205f, -0.91f, 0.01f), glm::vec3(0.06f, 0.04f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(-0.22f, -0.91f, 0.011f), glm::vec3(0.03f, 0.04f, 1.0f), 0.0f, 24); 
	DibujarParte(model, uniformModel, glm::vec3(0.205f, -0.91f, 0.01f), glm::vec3(0.06f, 0.04f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.19f, -0.91f, 0.011f), glm::vec3(0.03f, 0.04f, 1.0f), 0.0f, 24); 

	// Suelas
	DibujarParte(model, uniformModel, glm::vec3(-0.13f, -0.94f, 0.011f), glm::vec3(0.21f, 0.02f, 1.0f), 0.0f, 11);
	DibujarParte(model, uniformModel, glm::vec3(0.13f, -0.94f, 0.011f), glm::vec3(0.21f, 0.02f, 1.0f), 0.0f, 11);

	// Espada
	glm::mat4 swordBase = glm::translate(model, glm::vec3(0.30f, -0.31f, -0.02f));
	swordBase = glm::rotate(swordBase, glm::radians(-110.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Pomo espada
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, -0.15f, 0.0f), glm::vec3(0.07f, 0.06f, 1.0f), 0.0f, 3);

	// Mango espada
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, -0.06f, -0.005f), glm::vec3(0.04f, 0.14f, 1.0f), 0.0f, 11);

	// Mano derecha
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, -0.02f, 0.05f), glm::vec3(0.11f, 0.10f, 1.0f), 0.0f, 2);
	DibujarParte(swordBase, uniformModel, glm::vec3(-0.0275f, -0.02f, 0.051f), glm::vec3(0.055f, 0.10f, 1.0f), 0.0f, 20);

	// Hoja espada
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, 0.25f, -0.01f), glm::vec3(0.09f, 0.36f, 1.0f), 0.0f, 13);
	DibujarParte(swordBase, uniformModel, glm::vec3(-0.0225f, 0.25f, -0.009f), glm::vec3(0.045f, 0.36f, 1.0f), 0.0f, 18);

	// Punta espada
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, 0.515f, -0.01f), glm::vec3(0.09f, 0.17f, 1.0f), 0.0f, 5);
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, 0.515f, -0.009f), glm::vec3(0.09f, 0.17f, 1.0f), 0.0f, 19);

	// Guarda espada
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.24f, 0.08f, 1.0f), 0.0f, 3);
	DibujarParte(swordBase, uniformModel, glm::vec3(-0.12f, 0.04f, 0.0f), glm::vec3(0.06f, 0.10f, 1.0f), 0.0f, 4);
	DibujarParte(swordBase, uniformModel, glm::vec3(0.12f, 0.04f, 0.0f), glm::vec3(0.06f, 0.10f, 1.0f), 0.0f, 4);

	// Gema roja
	DibujarParte(swordBase, uniformModel, glm::vec3(0.0f, 0.05f, 0.01f), glm::vec3(0.035f, 0.035f, 1.0f), 45.0f, 12);

	// Gorro
	DibujarParte(model, uniformModel, glm::vec3(0.18f, 0.25f, -0.05f), glm::vec3(0.26f, 0.75f, 1.0f), -140.0f, 1);
	DibujarParte(model, uniformModel, glm::vec3(0.18f, 0.25f, -0.049f), glm::vec3(0.26f, 0.75f, 1.0f), -140.0f, 23);

	// Mangas
	DibujarParte(model, uniformModel, glm::vec3(-0.24f, 0.04f, 0.0f), glm::vec3(0.12f, 0.20f, 1.0f), -30.0f, 0);
	DibujarParte(model, uniformModel, glm::vec3(-0.27f, 0.04f, 0.001f), glm::vec3(0.06f, 0.20f, 1.0f), -30.0f, 22);
	DibujarParte(model, uniformModel, glm::vec3(0.23f, 0.03f, 0.0f), glm::vec3(0.12f, 0.20f, 1.0f), 20.0f, 0);
	DibujarParte(model, uniformModel, glm::vec3(0.20f, 0.03f, 0.001f), glm::vec3(0.06f, 0.20f, 1.0f), 20.0f, 22);

	// Brazo derecho
	DibujarParte(model, uniformModel, glm::vec3(0.27f, -0.14f, 0.0f), glm::vec3(0.08f, 0.35f, 1.0f), 10.0f, 2);
	DibujarParte(model, uniformModel, glm::vec3(0.2503f, -0.1435f, 0.001f), glm::vec3(0.04f, 0.35f, 1.0f), 10.0f, 20);

	// Funda espada
	DibujarParte(model, uniformModel, glm::vec3(0.10f, 0.05f, -0.02f), glm::vec3(0.09f, 0.45f, 1.0f), -40.0f, 16);
	DibujarParte(model, uniformModel, glm::vec3(0.244f, 0.222f, -0.019f), glm::vec3(0.10f, 0.05f, 1.0f), -40.0f, 15);
	DibujarParte(model, uniformModel, glm::vec3(-0.044f, -0.122f, -0.019f), glm::vec3(0.10f, 0.05f, 1.0f), -40.0f, 15);

	// Tunica
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.12f, 0.0f), glm::vec3(0.36f, 0.62f, 1.0f), 0.0f, 0);
	DibujarParte(model, uniformModel, glm::vec3(-0.09f, -0.12f, 0.001f), glm::vec3(0.18f, 0.62f, 1.0f), 0.0f, 22);

	// Cinturones
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.05f, 0.015f), glm::vec3(0.07f, 0.44f, 1.0f), -40.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.01f, -0.04f, 0.016f), glm::vec3(0.09f, 0.07f, 1.0f), -40.0f, 6);
	DibujarParte(model, uniformModel, glm::vec3(0.01f, -0.04f, 0.017f), glm::vec3(0.05f, 0.035f, 1.0f), -40.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.25f, 0.02f), glm::vec3(0.38f, 0.06f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(-0.095f, -0.25f, 0.0205f), glm::vec3(0.19f, 0.06f, 1.0f), 0.0f, 24);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.25f, 0.021f), glm::vec3(0.09f, 0.09f, 1.0f), 0.0f, 6);
	DibujarParte(model, uniformModel, glm::vec3(-0.0225f, -0.25f, 0.0215f), glm::vec3(0.045f, 0.09f, 1.0f), 0.0f, 25);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, -0.25f, 0.022f), glm::vec3(0.05f, 0.05f, 1.0f), 0.0f, 3);

	// Brazo izquierdo
	DibujarParte(model, uniformModel, glm::vec3(-0.30f, -0.01f, 0.01f), glm::vec3(0.08f, 0.3f, 1.0f), -30.0f, 2);
	DibujarParte(model, uniformModel, glm::vec3(-0.3173f, 0.0f, 0.011f), glm::vec3(0.04f, 0.3f, 1.0f), -30.0f, 20);

	// Escudo
	glm::mat4 shieldBase = glm::translate(model, glm::vec3(-0.35f, -0.15f, 0.09f));
	shieldBase = glm::rotate(shieldBase, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shieldBase = glm::scale(shieldBase, glm::vec3(1.25f, 1.25f, 1.0f));

	// Frente escudo
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.35f, 0.20f, 1.0f), 0.0f, 3);
	DibujarParte(shieldBase, uniformModel, glm::vec3(-0.0875f, 0.05f, 0.0005f), glm::vec3(0.175f, 0.20f, 1.0f), 0.0f, 24);

	// Cuernos escudo
	DibujarParte(shieldBase, uniformModel, glm::vec3(-0.0875f, 0.19f, 0.0f), glm::vec3(0.175f, 0.08f, 1.0f), 180.0f, 4);
	DibujarParte(shieldBase, uniformModel, glm::vec3(-0.0875f, 0.19f, 0.0006f), glm::vec3(0.175f, 0.08f, 1.0f), 0.0f, 27);
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.0875f, 0.19f, 0.0f), glm::vec3(0.175f, 0.08f, 1.0f), 180.0f, 4);
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.0875f, 0.19f, 0.0006f), glm::vec3(0.175f, 0.08f, 1.0f), 0.0f, 27);

	// Punta escudo
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.0f, -0.15f, 0.0f), glm::vec3(0.35f, 0.20f, 1.0f), 0.0f, 4);
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.0f, -0.15f, 0.0005f), glm::vec3(0.35f, 0.20f, 1.0f), 0.0f, 28);

	// Espiral escudo
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.0f, 0.02f, 0.001f), glm::vec3(0.04f, 0.02f, 1.0f), 0.0f, 12);
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.02f, -0.01f, 0.001f), glm::vec3(0.02f, 0.08f, 1.0f), 0.0f, 12);
	DibujarParte(shieldBase, uniformModel, glm::vec3(-0.01f, -0.04f, 0.001f), glm::vec3(0.08f, 0.02f, 1.0f), 0.0f, 12);
	DibujarParte(shieldBase, uniformModel, glm::vec3(-0.04f, 0.02f, 0.001f), glm::vec3(0.02f, 0.12f, 1.0f), 0.0f, 12);
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.02f, 0.07f, 0.001f), glm::vec3(0.14f, 0.02f, 1.0f), 0.0f, 12);
	DibujarParte(shieldBase, uniformModel, glm::vec3(0.08f, -0.02f, 0.001f), glm::vec3(0.02f, 0.18f, 1.0f), 0.0f, 12);
	DibujarParte(shieldBase, uniformModel, glm::vec3(-0.01f, -0.10f, 0.001f), glm::vec3(0.18f, 0.02f, 1.0f), 0.0f, 12);

	// Cuello y pecho
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.17f, 0.01f), glm::vec3(0.12f, 0.10f, 1.0f), 0.0f, 2);
	DibujarParte(model, uniformModel, glm::vec3(-0.03f, 0.17f, 0.011f), glm::vec3(0.06f, 0.10f, 1.0f), 0.0f, 20);

	// Cuello en V
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.12f, 0.02f), glm::vec3(0.18f, 0.12f, 1.0f), 180.0f, 7);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.12f, 0.021f), glm::vec3(0.18f, 0.12f, 1.0f), 0.0f, 21);

	// Orejas
	DibujarParte(model, uniformModel, glm::vec3(-0.22f, 0.43f, 0.01f), glm::vec3(0.12f, 0.35f, 1.0f), 65.0f, 17); 
	DibujarParte(model, uniformModel, glm::vec3(0.22f, 0.43f, 0.01f), glm::vec3(0.12f, 0.35f, 1.0f), -65.0f, 7);

	// Profundidad orejas
	DibujarParte(model, uniformModel, glm::vec3(-0.23f, 0.435f, 0.015f), glm::vec3(0.05f, 0.20f, 1.0f), 65.0f, 30);
	DibujarParte(model, uniformModel, glm::vec3(0.23f, 0.435f, 0.015f), glm::vec3(0.05f, 0.20f, 1.0f), -65.0f, 17);

	// Rostro
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.45f, 0.03f), glm::vec3(0.36f, 0.30f, 1.0f), 0.0f, 2);
	DibujarParte(model, uniformModel, glm::vec3(-0.09f, 0.45f, 0.031f), glm::vec3(0.18f, 0.30f, 1.0f), 0.0f, 20);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.24f, 0.03f), glm::vec3(0.36f, 0.12f, 1.0f), 180.0f, 7);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.24f, 0.031f), glm::vec3(0.36f, 0.12f, 1.0f), 0.0f, 21);

	// Ojos
	DibujarParte(model, uniformModel, glm::vec3(-0.085f, 0.38f, 0.04f), glm::vec3(0.075f, 0.09f, 1.0f), -5.0f, 8);
	DibujarParte(model, uniformModel, glm::vec3(0.085f, 0.38f, 0.04f), glm::vec3(0.075f, 0.09f, 1.0f), 5.0f, 8);
	DibujarParte(model, uniformModel, glm::vec3(-0.075f, 0.38f, 0.05f), glm::vec3(0.055f, 0.055f, 1.0f), 0.0f, 9);
	DibujarParte(model, uniformModel, glm::vec3(0.075f, 0.38f, 0.05f), glm::vec3(0.055f, 0.055f, 1.0f), 0.0f, 9);
	DibujarParte(model, uniformModel, glm::vec3(-0.075f, 0.38f, 0.051f), glm::vec3(0.035f, 0.035f, 1.0f), 0.0f, 14);
	DibujarParte(model, uniformModel, glm::vec3(0.075f, 0.38f, 0.051f), glm::vec3(0.035f, 0.035f, 1.0f), 0.0f, 14);

	// Brillo ojos
	DibujarParte(model, uniformModel, glm::vec3(-0.062f, 0.392f, 0.052f), glm::vec3(0.015f, 0.015f, 1.0f), 0.0f, 8);
	DibujarParte(model, uniformModel, glm::vec3(0.088f, 0.392f, 0.052f), glm::vec3(0.015f, 0.015f, 1.0f), 0.0f, 8);

	// Pestanas
	DibujarParte(model, uniformModel, glm::vec3(-0.085f, 0.425f, 0.055f), glm::vec3(0.08f, 0.012f, 1.0f), -5.0f, 11);
	DibujarParte(model, uniformModel, glm::vec3(0.085f, 0.425f, 0.055f), glm::vec3(0.08f, 0.012f, 1.0f), 5.0f, 11);

	// Nariz
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.32f, 0.04f), glm::vec3(0.04f, 0.08f, 1.0f), 180.0f, 7);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.285f, 0.041f), glm::vec3(0.03f, 0.025f, 1.0f), 180.0f, 17);

	// Boca
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.26f, 0.04f), glm::vec3(0.05f, 0.01f, 1.0f), 0.0f, 3);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.245f, 0.041f), glm::vec3(0.035f, 0.015f, 1.0f), 180.0f, 17);

	// Cabello
	DibujarParte(model, uniformModel, glm::vec3(-0.16f, 0.53f, 0.06f), glm::vec3(0.18f, 0.15f, 1.0f), 35.0f, 25);
	DibujarParte(model, uniformModel, glm::vec3(0.16f, 0.53f, 0.06f), glm::vec3(0.18f, 0.15f, 1.0f), -35.0f, 6);
	DibujarParte(model, uniformModel, glm::vec3(-0.17f, 0.38f, 0.07f), glm::vec3(0.12f, 0.30f, 1.0f), 195.0f, 10);
	DibujarParte(model, uniformModel, glm::vec3(0.17f, 0.39f, 0.07f), glm::vec3(0.11f, 0.28f, 1.0f), 165.0f, 10);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.59f, 0.065f), glm::vec3(0.18f, 0.07f, 1.0f), 180.0f, 10);
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.59f, 0.066f), glm::vec3(0.18f, 0.07f, 1.0f), 180.0f, 26);

	// Cejas
	DibujarParte(model, uniformModel, glm::vec3(-0.08f, 0.44f, 0.08f), glm::vec3(-0.10f, 0.025f, 1.0f), -30.0f, 29);
	DibujarParte(model, uniformModel, glm::vec3(0.08f, 0.44f, 0.08f), glm::vec3(0.10f, 0.025f, 1.0f), 30.0f, 29);

	// Gorro base
	DibujarParte(model, uniformModel, glm::vec3(0.0f, 0.62f, 0.02f), glm::vec3(0.38f, 0.12f, 1.0f), 0.0f, 0);
	DibujarParte(model, uniformModel, glm::vec3(-0.095f, 0.62f, 0.021f), glm::vec3(0.19f, 0.12f, 1.0f), 0.0f, 22);
}

int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();

	CrearFiguras(); 
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 model(1.0); 

	while (!mainWindow.getShouldClose())
	{
		glfwPollEvents();
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		shaderList[0].useShader();
		GLint currentProgram = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		GLuint uModelOutline = glGetUniformLocation(currentProgram, "model");
		GLuint uProjOutline = glGetUniformLocation(currentProgram, "projection");
		GLuint uColorOutline = glGetUniformLocation(currentProgram, "color");

		glUniformMatrix4fv(uProjOutline, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(uColorOutline, 0.0f, 0.0f, 0.0f); // Outline negro

		float offset = 0.005f;
		float offsets[8][2] = {
			{-offset, 0.0f}, {offset, 0.0f}, {0.0f, -offset}, {0.0f, offset},
			{-offset, -offset}, {-offset, offset}, {offset, -offset}, {offset, offset}
		};

		for (int i = 0; i < 8; i++)
		{
			glm::mat4 modelOutline = glm::translate(glm::mat4(1.0f), glm::vec3(offsets[i][0], offsets[i][1], -0.2f));
			DibujarPersonaje(modelOutline, uModelOutline);
		}

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		DibujarPersonaje(model, uniformModel);

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}