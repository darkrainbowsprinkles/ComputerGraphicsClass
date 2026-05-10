#define _CRT_SECURE_NO_WARNINGS
#include "Nave.h"

Nave::Nave()
{
    posXavion = 2.0f;
    posYavion = 2.0f;
    posZavion = 0.0f;
    movAvion_x = 0.0f;
    movAvion_y = 0.0f;
    giroAvion = 0.0f;

    velocidadAlas = 0.2f;
    amplitudAlas = 25.0f;
    tiempoVuelo = 0.0f;

    btn_Xpos = 0; btn_Xneg = 0;
    btn_Ypos = 0; btn_Yneg = 0;
    btn_RotPos = 0; btn_RotNeg = 0;
    btn_HabilitarMov = 0;
    reproduciranimacion = 0;
    habilitaranimacion = 0;
    guardoFrame = 0;
    reinicioFrame = 0;
    btn_GuardarTxt = false;
    btn_CargarTxt = false;

    i_max_steps = 600;
    i_curr_steps = 6;
    FrameIndex = 6;
    playIndex = 0;
    play = false;
}

Nave::~Nave() {}

void Nave::LoadModels()
{
    Nave_M = Model();
    Nave_M.LoadModel("Models/nave.obj");
    Ala_M = Model();
    Ala_M.LoadModel("Models/ala.obj");
}

void Nave::SetKeyframesIniciales()
{
    KeyFrame[0].movAvion_x = 0.0f;
    KeyFrame[0].movAvion_y = 0.0f;
    KeyFrame[0].giroAvion = 0.0f;

    KeyFrame[1].movAvion_x = 1.0f;
    KeyFrame[1].movAvion_y = 2.0f;
    KeyFrame[1].giroAvion = 0.0f;

    KeyFrame[2].movAvion_x = 2.0f;
    KeyFrame[2].movAvion_y = 0.0f;
    KeyFrame[2].giroAvion = 0.0f;

    KeyFrame[3].movAvion_x = 3.0f;
    KeyFrame[3].movAvion_y = -2.0f;
    KeyFrame[3].giroAvion = 0.0f;

    KeyFrame[4].movAvion_x = 3.0f;
    KeyFrame[4].movAvion_y = -2.0f;
    KeyFrame[4].giroAvion = 180.0f;

    KeyFrame[5].movAvion_x = 0.0f;
    KeyFrame[5].movAvion_y = 0.0f;
    KeyFrame[5].giroAvion = 180.0f;
}

void Nave::DisplayMenu()
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

void Nave::saveFrame()
{
    printf("frameindex %d\n", FrameIndex);
    KeyFrame[FrameIndex].movAvion_x = movAvion_x;
    KeyFrame[FrameIndex].movAvion_y = movAvion_y;
    KeyFrame[FrameIndex].giroAvion = giroAvion;
    FrameIndex++;
}

void Nave::resetElements()
{
    movAvion_x = KeyFrame[0].movAvion_x;
    movAvion_y = KeyFrame[0].movAvion_y;
    giroAvion = KeyFrame[0].giroAvion;
}

void Nave::interpolation()
{
    KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
    KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
    KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
}

void Nave::GuardarKeyframesArchivo()
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

void Nave::CargarKeyframesArchivo()
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

void Nave::InputControl(bool* keys)
{
    if (keys[GLFW_KEY_SPACE])
    {
        if (reproduciranimacion < 1)
        {
            if (play == false && (FrameIndex > 1))
            {
                resetElements();
                interpolation();
                play = true;
                playIndex = 0;
                i_curr_steps = 0;
                reproduciranimacion++;
                printf("presiona 0 para habilitar reproducir de nuevo la animación\n");
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
            printf("Ya puedes reproducir de nuevo la animacion con la tecla de barra espaciadora\n");
        }
    }

    if (keys[GLFW_KEY_L])
    {
        if (guardoFrame < 1)
        {
            saveFrame();
            printf("presiona P para habilitar guardar otro frame\n");
            guardoFrame++;
            reinicioFrame = 0;
        }
    }

    if (keys[GLFW_KEY_P])
    {
        if (reinicioFrame < 1)
        {
            guardoFrame = 0;
            printf("Ya puedes guardar otro frame presionando la tecla L\n");
        }
    }

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

    if (keys[GLFW_KEY_E])
    {
        if (btn_RotPos < 1) {
            giroAvion -= 45.0f;
            printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
            btn_RotPos++; btn_HabilitarMov = 0;
        }
    }

    if (keys[GLFW_KEY_Q])
    {
        if (btn_RotNeg < 1) {
            giroAvion += 45.0f;
            printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
            btn_RotNeg++; btn_HabilitarMov = 0;
        }
    }

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

void Nave::Animate()
{
    if (play)
    {
        if (i_curr_steps >= i_max_steps)
        {
            playIndex++;
            printf("playindex : %d\n", playIndex);
            if (playIndex > FrameIndex - 2)
            {
                printf("Frame index= %d\n", FrameIndex);
                printf("termina anim\n");
                playIndex = 0;
                play = false;
            }
            else
            {
                i_curr_steps = 0;
                interpolation();
            }
        }
        else
        {
            movAvion_x += KeyFrame[playIndex].movAvion_xInc;
            movAvion_y += KeyFrame[playIndex].movAvion_yInc;
            giroAvion += KeyFrame[playIndex].giroAvionInc;
            i_curr_steps++;
        }
    }
}

void Nave::Render(GLuint uniformModel, float deltaTime)
{
    glm::mat4 model = glm::mat4(1.0);
    glm::vec3 posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);

    model = glm::translate(model, posblackhawk);
    model = glm::rotate(model, (giroAvion)*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelaux = model;

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
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