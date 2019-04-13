#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#include "car.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vertexColor;

    uniform mat4 transform;

    void main()
    {
        gl_Position = transform * vec4(aPos, 1.0f);
        vertexColor = aColor;
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 vertexColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(vertexColor, 1.0);
    } 
)glsl";

void processInput(GLFWwindow *window);
void printShaderDebug(GLuint shader, const char *name);

int success;
char buffer[512];

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
auto projection = glm::perspective(glm::radians(45.0f), 1.0f * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "set_float", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // set OpenGL

    // SET SHADERS
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    printShaderDebug(vertexShader, "vertex");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    printShaderDebug(fragmentShader, "fragment");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
        printf("link %s\n", buffer);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0));

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // auto resultTrans = glm::scale(rotate * trans, glm::vec3(0.5, 0.5, 0.5));
        auto resultTrans = projection * view * model;
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(resultTrans));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(0.0, 0.0, 1.0));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(0.0, 1.0, 0.0));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0, 0.0, 0.0));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0));
}

void printShaderDebug(GLuint shader, const char *name)
{
    int status;
    char buffer[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf("%s %x %s\n", name, glGetError(), status == GL_TRUE ? "OK" : "ERR");
    if (status != GL_TRUE)
    {
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        printf("shader %s %s\n", name, buffer);
    }
}