/*
Animación por Keyframes
*/

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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;
Model Aeolipile_base_M;
Model Aeolipile_M;
Model ala2;

Skybox skybox;

Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


static double limitFPS = 1.0 / 60.0;

unsigned int spotLightCount = 0;
unsigned int pointLightCount = 0;

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
const float toRadians = 3.14159265f / 180.0f;

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

void InputKeyframes(bool* keys);
void UpdateDeltaTime();
void RegisterUserEvents();
void CreateLights();
void SetKeyframesIniciales();
void RenderPiso(GLuint uniformTextureOffset, glm::vec2& toffset, glm::mat4& model, GLuint uniformModel, GLuint uniformColor, glm::vec3& color, GLuint uniformSpecularIntensity, GLuint uniformShininess);
void ClearWindow(const glm::mat4& projection, GLuint& uniformModel, GLuint& uniformProjection, GLuint& uniformView, GLuint& uniformEyePosition, GLuint& uniformColor, GLuint& uniformTextureOffset);
void SetShaderInfo(GLuint& uniformSpecularIntensity, GLuint& uniformShininess, GLuint uniformProjection, glm::mat4& projection, GLuint uniformView, GLuint uniformEyePosition, glm::vec3& color, glm::vec2& toffset);
void SetFlashLight(glm::vec3& lowerLight);
void RenderNave(glm::mat4& model, glm::vec3& posblackhawk, glm::mat4& modelaux, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
void LoadTextures();
void DisplayMenuKeyframes();
void SetSkybox();
void LoadModels();

//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



///////////////////////////////KEYFRAMES/////////////////////

bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0, posYavion = 2.0, posZavion = 0;
float movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;
float btn_Xpos = 0, btn_Xneg = 0;
float btn_Ypos = 0, btn_Yneg = 0;
float btn_RotPos = 0, btn_RotNeg = 0;
float btn_HabilitarMov = 0;

float velocidadAlas = 0.1f;
float amplitudAlas = 25.0f;
float tiempoVuelo = 0.0f;

bool btn_GuardarTxt = false;
bool btn_CargarTxt = false;

#define MAX_FRAMES 100
int i_max_steps = 600;
int i_curr_steps = 6;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto // barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

void GuardarKeyframesArchivo()
{
	FILE* archivo;
	fopen_s(&archivo, "animacion_keyframes.txt", "w");

	if (archivo == NULL)
	{
		printf("Error al abrir el archivo para guardar.\n");
		return;
	}


	fprintf(archivo, "%d\n", FrameIndex);

	for (int i = 0; i < FrameIndex; i++)
	{
		fprintf(archivo, "%f %f %f\n", KeyFrame[i].movAvion_x, KeyFrame[i].movAvion_y, KeyFrame[i].giroAvion);
	}

	fclose(archivo);
	printf("EXITO: Keyframes guardados en 'animacion_keyframes.txt'. (Total de frames: %d)\n", FrameIndex);
}

void CargarKeyframesArchivo()
{
	FILE* archivo;
	fopen_s(&archivo, "animacion_keyframes.txt", "r");

	if (archivo == NULL)
	{
		printf("ERROR: No se encontro el archivo 'animacion_keyframes.txt'.\n");
		return;
	}

	fscanf_s(archivo, "%d", &FrameIndex);

	for (int i = 0; i < FrameIndex; i++)
	{
		fscanf_s(archivo, "%f %f %f", &KeyFrame[i].movAvion_x, &KeyFrame[i].movAvion_y, &KeyFrame[i].giroAvion);
	}

	fclose(archivo);
	printf("EXITO: Keyframes cargados correctamente. (Total de frames: %d)\n", FrameIndex);
	resetElements();
}

///////////////* FIN KEYFRAMES*////////////////////////////

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

void LoadModels()
{
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	Aeolipile_base_M = Model();
	Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();
	Aeolipile_M.LoadModel("Models/Aeolipile.obj");
	ala2 = Model();
	ala2.LoadModel("Models/Ala2.obj");
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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
}

void DisplayMenuKeyframes()
{
	printf("\n--- CONTROLES DE ANIMACION POR KEYFRAMES ---\n");

	printf("MOVIMIENTO MANUAL:\n");
	printf(" [Flechas Izq/Der] Mover nave en el eje X.\n");
	printf(" [Flechas Arr/Aba] Mover nave en el eje Y.\n");
	printf(" [Q / E] Girar la nave hacia la izquierda/derecha.\n");
	printf(" [2] Habilitar movimiento.\n\n");

	printf("GUARDADO DE FRAMES EN MEMORIA:\n");
	printf(" [L] Guardar el Keyframe actual.\n");
	printf(" [P] Habilitar para poder guardar el siguiente Keyframe.\n\n");

	printf("GUARDADO/CARGADO EN ARCHIVO TXT:\n");
	printf(" [G] Guardar todos los Keyframes generados en 'animacion_keyframes.txt'.\n");
	printf(" [C] Cargar Keyframes desde el archivo TXT.\n\n");

	printf("REPRODUCCION:\n");
	printf(" [Barra Espaciadora] Reproducir animacion guardada/cargada.\n");
	printf(" [0] Habilitar reproduccion de nuevo.\n");
	printf("--------------------------------------------\n\n");
}

void CreateLights()
{
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
}

void SetKeyframesIniciales()
{
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;


	KeyFrame[4].movAvion_x = 3.0f;
	KeyFrame[4].movAvion_y = -2.0f;
	KeyFrame[4].giroAvion = 180.0f;

	KeyFrame[5].movAvion_x = 0.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].giroAvion = 180.0f;
}

void RenderPiso(GLuint uniformTextureOffset, glm::vec2& toffset, glm::mat4& model, GLuint uniformModel, GLuint uniformColor, glm::vec3& color, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	pisoTexture.UseTexture();
	Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();
}

void ClearWindow(const glm::mat4& projection, GLuint& uniformModel, GLuint& uniformProjection, GLuint& uniformView, GLuint& uniformEyePosition, GLuint& uniformColor, GLuint& uniformTextureOffset)
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
	uniformTextureOffset = shaderList[0].getOffsetLocation();
}

void SetShaderInfo(GLuint& uniformSpecularIntensity, GLuint& uniformShininess, GLuint uniformProjection, glm::mat4& projection, GLuint uniformView, GLuint uniformEyePosition, glm::vec3& color, glm::vec2& toffset)
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

	//reiniciar variables antes de que sean enviadas al shader
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	toffset = glm::vec2(0.0f, 0.0f);
}

void SetFlashLight(glm::vec3& lowerLight)
{
	lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
}

void RenderNave(glm::mat4& model, glm::vec3& posblackhawk, glm::mat4& modelaux, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
	model = glm::mat4(1.0);
	posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);

	model = glm::translate(model, posblackhawk);
	model = glm::rotate(model, (giroAvion)*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelaux = model;

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Nave_M.RenderModel();

	if (play)
	{
		tiempoVuelo += deltaTime; 
	}
	else
	{
		tiempoVuelo = 0.0f; 
	}

	float anguloAnimacion = sin(tiempoVuelo * velocidadAlas) * amplitudAlas;

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f));
	model = glm::rotate(model, (90.0f + anguloAnimacion) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Ala_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, -1.0f));
	model = glm::rotate(model, (90.0f + anguloAnimacion) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Ala_M.RenderModel();
}

void RegisterUserEvents()
{
	glfwPollEvents();
	camera.keyControl(mainWindow.getsKeys(), deltaTime);
	camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
}

void UpdateDeltaTime()
{
	GLfloat now = glfwGetTime();
	deltaTime = now - lastTime;
	deltaTime += (now - lastTime) / limitFPS;
	lastTime = now;
}

void InputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}

	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			habilitaranimacion = 1;
			printf("Ya puedes reproducir de nuevo la animacion con la tecla de barra espaciadora'\n");
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			//printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}

	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}

	// CONTROLES DE POSICION
	if (keys[GLFW_KEY_RIGHT]) 
	{
		if (btn_Xpos < 1) {
			movAvion_x += 1.0f;
			printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
			btn_Xpos++; btn_HabilitarMov = 0;
		}
	}

	if (keys[GLFW_KEY_LEFT]) 
	{
		if (btn_Xneg < 1) 
		{
			movAvion_x -= 1.0f;
			printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
			btn_Xneg++; btn_HabilitarMov = 0;
		}
	}

	if (keys[GLFW_KEY_UP]) 
	{
		if (btn_Ypos < 1) 
		{
			movAvion_y += 1.0f;
			printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
			btn_Ypos++; btn_HabilitarMov = 0;
		}
	}

	if (keys[GLFW_KEY_DOWN]) 
	{
		if (btn_Yneg < 1) 
		{
			movAvion_y -= 1.0f;
			printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
			btn_Yneg++; btn_HabilitarMov = 0;
		}
	}

	// CONTROLES DE ROTACION
	if (keys[GLFW_KEY_E]) // Girar a la derecha
	{ 
		if (btn_RotPos < 1) {
			giroAvion -= 45.0f;
			printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
			btn_RotPos++; btn_HabilitarMov = 0;
		}
	}

	if (keys[GLFW_KEY_Q]) // Girar a la izquierda
	{ 
		if (btn_RotNeg < 1) {
			giroAvion += 45.0f;
			printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
			btn_RotNeg++; btn_HabilitarMov = 0;
		}
	}

	// RESET DE BOTONES
	if (keys[GLFW_KEY_2]) 
	{
		if (btn_HabilitarMov < 1) 
		{
			btn_Xpos = 0; btn_Xneg = 0;
			btn_Ypos = 0; btn_Yneg = 0;
			btn_RotPos = 0; btn_RotNeg = 0;
			btn_HabilitarMov++;
			printf("Movimientos habilitados de nuevo (Presiona flechas o Q/E)\n");
		}
	}

	// GUARDAR Y CARGAR ARCHIVO
	if (keys[GLFW_KEY_G])
	{
		if (!btn_GuardarTxt)
		{
			GuardarKeyframesArchivo();
			btn_GuardarTxt = true; 
		}
	}
	else
	{
		btn_GuardarTxt = false; 
	}

	if (keys[GLFW_KEY_C])
	{
		if (!btn_CargarTxt)
		{
			CargarKeyframesArchivo();
			btn_CargarTxt = true; 
		}
	}
	else
	{
		btn_CargarTxt = false; 
	}
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	LoadTextures();
	LoadModels();
	SetSkybox();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	CreateLights();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color (1.0f, 1.0f, 1.0f);
	glm::vec2 toffset (0.0f, 0.0f);

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	SetKeyframesIniciales();
	DisplayMenuKeyframes();
	
	while (!mainWindow.getShouldClose())
	{
		angulovaria += 0.5f * deltaTime;

		//¿Cómo haces para que el coche no se salga del piso?
		movCoche -= movOffset * deltaTime;
		rotllanta += rotllantaOffset * deltaTime;

		UpdateDeltaTime();
		RegisterUserEvents();

		//para keyframes
		InputKeyframes(mainWindow.getsKeys());
		animate();

		ClearWindow(projection, uniformModel, uniformProjection, uniformView, uniformEyePosition, uniformColor, uniformTextureOffset);
		SetShaderInfo(uniformSpecularIntensity, uniformShininess, uniformProjection, projection, uniformView, uniformEyePosition, color, toffset);
		SetFlashLight(lowerLight);
		RenderPiso(uniformTextureOffset, toffset, model, uniformModel, uniformColor, color, uniformSpecularIntensity, uniformShininess);
		RenderNave(model, posblackhawk, modelaux, uniformModel, uniformSpecularIntensity, uniformShininess);

		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
