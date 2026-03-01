#pragma once

void Draw2DHouse(glm::mat4& model, glm::vec3& houseSize, GLuint uniformModel, GLuint uniformProjection, glm::mat4& projection, glm::vec3& windowSize, glm::vec3& roofSize, glm::vec3& logSize, glm::vec3& treeSize);

void DrawCubeAndPyramid(GLuint& uniformModel, GLuint& uniformProjection, glm::mat4& model, glm::mat4& projection);
