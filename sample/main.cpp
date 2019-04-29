#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// Function to listen and handle keypress
void processInput(GLFWwindow *window)
{
    // Escape key for closing
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "set_float", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // Set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        //clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // set draw color
        glColor3f(0.0f, 1.0f, 0.0f);

        glBegin(GL_TRIANGLES); //starts drawing of points
        glVertex2f(0.0, 0.6f); // x, y, z
        glVertex2f(0.5, 0.1f);
        glVertex2f(0.3, -0.5f);
        glEnd(); //end drawing of points

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES); //starts drawing of points
        glVertex2f(-0.3, -0.5f);
        glVertex2f(-0.5, 0.1f);
        glVertex2f(-0.8, 0.1f);
        glEnd(); //end drawing of points

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}