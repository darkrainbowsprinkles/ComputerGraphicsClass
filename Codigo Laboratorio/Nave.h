#pragma once

#include <stdio.h>
#include <cmath>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Model.h"

#define MAX_FRAMES 100

typedef struct _frame
{
    float movAvion_x;
    float movAvion_y;
    float movAvion_xInc;
    float movAvion_yInc;
    float giroAvion;
    float giroAvionInc;
} FRAME;

class Nave
{
public:
    Nave();
    ~Nave();

    void LoadModels();
    void SetKeyframesIniciales();
    void DisplayMenu();
    void InputControl(bool* keys);
    void Animate();
    void Render(GLuint uniformModel, float deltaTime);

private:
    Model Nave_M;
    Model Ala_M;

    float posXavion;
    float posYavion;
    float posZavion;
    float movAvion_x;
    float movAvion_y;
    float giroAvion;
    float velocidadAlas;
    float amplitudAlas;
    float tiempoVuelo;
    float btn_Xpos, btn_Xneg, btn_Ypos, btn_Yneg;
    float btn_RotPos, btn_RotNeg, btn_HabilitarMov;
    float reproduciranimacion, habilitaranimacion;
    float guardoFrame, reinicioFrame;
    bool btn_GuardarTxt, btn_CargarTxt;

    int i_max_steps;
    int i_curr_steps;
    FRAME KeyFrame[MAX_FRAMES];
    int FrameIndex;
    int playIndex;
    bool play;

    void saveFrame();
    void resetElements();
    void interpolation();
    void GuardarKeyframesArchivo();
    void CargarKeyframesArchivo();

    const float toRadians = 3.14159265f / 180.0f;
};