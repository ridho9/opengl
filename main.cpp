#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "const.h"
#include "vertices.h"
#include "car.h"

#include "smoke_particle.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
float yaw = -90, pitch = 0;

// Function to listen and handle keypress
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

double prevTime, curTime, deltaTime;

int main(void)
{
    srand(time(NULL));
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "set_float", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGL())
    // if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialize\n");
        return -1;
    }

    // Set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Car car;
    ParticleRenderer particleRenderer;
    SmokeParticle particles[100];
    SmokeParticle rainParticles[5000];

    for(int i = 0; i < 5000; i++) {
        float pos_x = (float) (rand() % 500);
        float pos_y = (float) (rand() % 500);
        float pos_z = (float) (rand() % 500);
        rainParticles[i].pos = glm::vec3(pos_x, pos_y, pos_z);
        rainParticles[i].speed = glm::vec3(0.0f, -5.0f, 0.0f);
    }

    curTime = glfwGetTime();
    glm::vec3 black = glm::vec3(0.0f);
    glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        prevTime = curTime;
        curTime = glfwGetTime();
        deltaTime = curTime - prevTime;

        for (int i = 0; i < 100; i++)
        {
            particles[i].update(deltaTime);
        }

        for(int i = 0; i < 5000; i++) {
            rainParticles[i].rain(deltaTime);
        }

        // Background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // model, set object scale pos rot here
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::translate(model, glm::vec3(-40.0f, -40.0f, 0.0f));

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // projection, set camera projection here
        glm::mat4 projection = glm::perspective(glm::radians(zoom), 1.0f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);

        car.draw(model, view, projection);
        for (int i = 0; i < 100; i++)
        {
            particles[i].draw(particleRenderer, model, view, projection, black);
        }

        for(int i = 0; i < 5000; i++) {
            rainParticles[i].draw(particleRenderer, model, view, projection, blue);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    // Escape key for closing
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    float cameraSpeed = 0.1; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        ambientStrength -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        ambientStrength += 0.05;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}