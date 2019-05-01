#ifndef CONST_H
#define CONST_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float ambientStrength = 0.4;
float scale = 0.035;

float zoom = 45.0f;
glm::vec3 cameraPos = glm::vec3(3.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(-0.5f, -0.1f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos(50.2f, 50.0f, 50.0f);

#endif