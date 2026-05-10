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
#include "Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Nave.h" 

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture; 
Skybox skybox;
Material Material_brillante, Material_opaco;

Nave nave;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

static double limitFPS = 1.0 / 60.0;
unsigned int spotLightCount = 0;
unsigned int pointLightCount = 0;

const float toRadians = 3.14159265f / 180.0f;

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

void UpdateDeltaTime();
void RegisterUserEvents();
void CreateLights();
void RenderPiso(GLuint uniformTextureOffset, glm::vec2& toffset, glm::mat4& model, GLuint uniformModel, GLuint uniformColor, glm::vec3& color, GLuint uniformSpecularIntensity, GLuint uniformShininess);
void ClearWindow(const glm::mat4& projection, GLuint& uniformModel, GLuint& uniformProjection, GLuint& uniformView, GLuint& uniformEyePosition, GLuint& uniformColor, GLuint& uniformTextureOffset);
void SetShaderInfo(GLuint& uniformSpecularIntensity, GLuint& uniformShininess, GLuint uniformProjection, glm::mat4& projection, GLuint uniformView, GLuint uniformEyePosition, glm::vec3& color, glm::vec2& toffset);
void SetFlashLight(glm::vec3& lowerLight);
void LoadTextures();
void SetSkybox();

void CreateObjects()
{
    unsigned int floorIndices[] = { 0, 2, 1, 1, 2, 3 };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f
    };

    // Solo creamos la malla del piso y la guardamos en el índice 0
    Mesh* objFloor = new Mesh();
    objFloor->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(objFloor);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
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

void LoadTextures()
{
    pisoTexture = Texture("Textures/piso.tga");
    pisoTexture.LoadTextureA();
}

void CreateLights()
{
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.3f, 0.3f, 0.0f, 0.0f, -1.0f);

    pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 2.5f, 1.5f, 0.3f, 0.2f, 0.1f);
    pointLightCount++;

    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f);
    spotLightCount++;

    spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 5.0f, 10.0f, 0.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 15.0f);
    spotLightCount++;
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

    // Ahora el piso es el elemento 0 de la lista
    meshList[0]->RenderMesh();
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
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

    shaderList[0].SetDirectionalLight(&mainLight);
    shaderList[0].SetPointLights(pointLights, pointLightCount);
    shaderList[0].SetSpotLights(spotLights, spotLightCount);

    color = glm::vec3(1.0f, 1.0f, 1.0f);
    toffset = glm::vec2(0.0f, 0.0f);
}

void SetFlashLight(glm::vec3& lowerLight)
{
    lowerLight = camera.getCameraPosition();
    lowerLight.y -= 0.3f;
    spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
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

int main()
{
    mainWindow = Window(1366, 768);
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

    LoadTextures();

    nave.LoadModels();
    nave.SetKeyframesIniciales();
    nave.DisplayMenu();

    SetSkybox();

    Material_brillante = Material(4.0f, 256);
    Material_opaco = Material(0.3f, 4);

    CreateLights();

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

    glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);
    glm::mat4 model(1.0);
    glm::vec3 color(1.0f, 1.0f, 1.0f);
    glm::vec2 toffset(0.0f, 0.0f);

    while (!mainWindow.getShouldClose())
    {
        UpdateDeltaTime();
        RegisterUserEvents();

        nave.InputControl(mainWindow.getsKeys());
        nave.Animate();

        ClearWindow(projection, uniformModel, uniformProjection, uniformView, uniformEyePosition, uniformColor, uniformTextureOffset);
        SetShaderInfo(uniformSpecularIntensity, uniformShininess, uniformProjection, projection, uniformView, uniformEyePosition, color, toffset);
        SetFlashLight(lowerLight);

        RenderPiso(uniformTextureOffset, toffset, model, uniformModel, uniformColor, color, uniformSpecularIntensity, uniformShininess);

        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        nave.Render(uniformModel, deltaTime);

        glDisable(GL_BLEND);
        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}