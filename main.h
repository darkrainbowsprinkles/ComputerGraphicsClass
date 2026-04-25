#pragma once

void LoadTextures();

void LoadModels();

void SetSkybox();

void RenderCoche(glm::mat4& model, glm::vec3& poscoche, GLuint uniformModel, glm::mat4& modelaux, GLuint uniformSpecularIntensity, GLuint uniformShininess);

void RenderAgave(glm::mat4& model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);

void RenderHelicoptero(glm::mat4& model, GLuint uniformModel);

void HandleFaroCoche(glm::mat4& modelaux);

void UpdateDeltaTime();

void ClearWindow(const glm::mat4& projection, GLuint& uniformModel, GLuint& uniformProjection, GLuint& uniformView, GLuint& uniformEyePosition, GLuint& uniformColor);

void SetShaderInfo(GLuint& uniformSpecularIntensity, GLuint& uniformShininess, GLuint uniformProjection, glm::mat4& projection, GLuint uniformView, GLuint uniformEyePosition);

void RenderPiso(glm::mat4& model, GLuint uniformModel, GLuint uniformColor, glm::vec3& color, GLuint uniformSpecularIntensity, GLuint uniformShininess);

void SetLinterna();

void RegisterUserEvents();
