#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "vertices.hpp"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

// GLfloat vertices[] = {
//     0.0f, 0.5f, 1.0f, 0.0f, 0.0f,  // Vertex 1: Red
//     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
//     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f // Vertex 3: Blue
// };

GLint status;
char buffer[512];

const GLchar *vertexSource = R"glsl(
    #version 130
    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar *fragmentSource = R"glsl(
    #version 130
    in vec3 Color;

    out vec4 outColor;

    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

// Function to listen and handle keypress
void processInput(GLFWwindow *window);
void printShaderDebug(GLuint shader, const char *name);

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "set_float", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("glew init failed");
        return -1;
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    printf("VAO %x\n", glGetError());

    // Set up VBO
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    printf("VBO %x\n", glGetError());

    // Set up vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    printShaderDebug(vertexShader, "vertex");

    // Set up fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    printShaderDebug(vertexShader, "fragment");

    // Set up shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    printf("sp %x\n", glGetError());

    // Bind fragment shader buffer
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    printf("bind f %x\n", glGetError());
    glLinkProgram(shaderProgram);
    printf("link %x\n", glGetError());
    glUseProgram(shaderProgram);
    printf("use %x\n", glGetError());

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    printf("getal %x\n", glGetError());
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *)(2 * sizeof(float)));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, verticesCount);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    // Escape key for closing
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void printShaderDebug(GLuint shader, const char *name)
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf("%s %x %s\n", name, glGetError(), status == GL_TRUE ? "OK" : "ERR");
    if (status != GL_TRUE)
    {
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        printf("%s %s\n", name, buffer);
    }
}