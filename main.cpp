#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "set_float", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Set viewport
    glViewport(0, 0, 800, 600);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        //clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0f, 1.0f, 0.0f);

        glBegin(GL_POLYGON);           //starts drawing of points
        glVertex3f(0.0, 0.6f, 0.0f);   //upper-right corner
        glVertex3f(0.5, 0.1f, 0.0f);   //upper-right corner
        glVertex3f(0.3, -0.5f, 0.0f);  //upper-right corner
        glVertex3f(-0.3, -0.5f, 0.0f); //upper-right corner
        glVertex3f(-0.5, 0.1f, 0.0f);  //upper-right corner
        glEnd();                       //end drawing of points

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}