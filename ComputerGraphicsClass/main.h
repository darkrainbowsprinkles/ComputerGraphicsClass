#pragma once

void LoadTextures();

void LoadModels();

void SetSkybox();

void CreateLights();

void RenderCoche(glm::mat4& model, glm::mat4& modelaux, const GLuint& uniformModel, glm::vec3& color, const GLuint& uniformColor);

void RenderPista(glm::mat4& model, const GLuint& uniformModel, const GLuint& uniformSpecularIntensity, const GLuint& uniformShininess);

void RenderNave(glm::mat4& model, const GLuint& uniformModel);

void RenderAelopile(glm::mat4& model, const GLuint& uniformModel);

void RenderAgave(glm::mat4& model, const GLuint& uniformModel, const GLuint& uniformSpecularIntensity, const GLuint& uniformShininess);

void RenderFlecha(glm::vec2& toffset, glm::mat4& model, const GLuint& uniformTextureOffset, const GLuint& uniformModel, glm::vec3& color, const GLuint& uniformColor, const GLuint& uniformSpecularIntensity, const GLuint& uniformShininess);

void RenderNumeros(glm::vec2& toffset, glm::mat4& model, const GLuint& uniformTextureOffset, const GLuint& uniformModel, glm::vec3& color, const GLuint& uniformColor, const GLuint& uniformSpecularIntensity, const GLuint& uniformShininess);

void RenderPiso(glm::mat4& model, const GLuint& uniformModel, const GLuint& uniformColor, glm::vec3& color, const GLuint& uniformTextureOffset, glm::vec2& toffset, const GLuint& uniformSpecularIntensity, const GLuint& uniformShininess);

void SetShaderInfo(GLuint& uniformSpecularIntensity, GLuint& uniformShininess, const GLuint& uniformProjection, glm::mat4& projection, const GLuint& uniformView, const GLuint& uniformEyePosition, glm::vec3& lowerLight);

void ClearWindow(const glm::mat4& projection, GLuint& uniformModel, GLuint& uniformProjection, GLuint& uniformView, GLuint& uniformEyePosition, GLuint& uniformColor, GLuint& uniformTextureOffset);

void RegisterUserEvents();

void ResetVariables(glm::mat4& model, glm::mat4& modelaux, glm::vec3& color, glm::vec2& toffset, const GLuint& uniformTextureOffset);

void UpdateDeltaTime(GLfloat& now);

void UpdateVelocidadAelopile();
