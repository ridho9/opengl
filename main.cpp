// #include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

GLfloat vertices[] = {
    0.0f, 0.5f, 1.0f, 0.0f, 0.0f,  // Vertex 1: Red
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f // Vertex 3: Blue
};

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
GLuint createShader(const char *name, GLenum shaderType, const GLchar *source);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    // if (!gladLoadGL())
    if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialize\n");
        return -1;
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up VBO
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertexShader = createShader("vertex", GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = createShader("fragment", GL_FRAGMENT_SHADER, fragmentSource);

    // Set up shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Bind fragment shader buffer
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *)(2 * sizeof(float)));

    // Set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
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

GLuint createShader(const char *name, GLenum shaderType, const GLchar *source)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    printShaderDebug(shader, name);

    return shader;
}