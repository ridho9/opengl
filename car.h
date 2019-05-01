#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "const.h"
#include "shader.h"
#include "vertices.h"

class Car
{
  public:
    GLuint VBO, VAO, texture;
    Shader shader;

    Car() : shader("vertex.vs", "fragment.fs")
    {
        // Set Up VBO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Set up VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Set up vertex attrib array
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float), 0);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float), (void *)(3 * sizeof(float)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float), (void *)(5 * sizeof(float)));

        // Set up textures
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load("b8ojml.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("Failed to load texture\n");
        }
        stbi_image_free(data);

        // set transform
        shader.use();
        shader.setVec3("lightPos", lightPos);
    }

    void draw()
    {
        shader.use();

        // model, set object scale pos rot here
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::translate(model, glm::vec3(-40.0f, -40.0f, 0.0f));
        shader.setMat4("model", model);

        // view, set camera pos here
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        // projection, set camera projection here
        glm::mat4 projection = glm::perspective(glm::radians(zoom), 1.0f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);
        shader.setMat4("projection", projection);

        shader.setFloat("ambientStrength", ambientStrength);

        // bind texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render
        glBindVertexArray(VAO);

        // draw body
        for (int i = 0; i < 11; i++)
        {
            glDrawArrays(GL_TRIANGLE_STRIP, 4 * i, 4);
        }

        model = glm::translate(model, glm::vec3(40.0f, -20.0f, -50.0f));
        drawWheel(model);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -70.0f));
        drawWheel(model);
        model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
        drawWheel(model);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 70.0f));
        drawWheel(model);
    }

  private:
    void drawWheel(glm::mat4 wheelOffset)
    {
        // draw wheel
        glm::mat4 model;
        model = wheelOffset;
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 4 * 11, 4);
        model = glm::translate(wheelOffset, glm::vec3(10.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 4 * 11, 4);
        //tb
        model = glm::translate(wheelOffset, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 4 * 12, 4);
        model = glm::translate(wheelOffset, glm::vec3(0.0f, -40.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 4 * 12, 4);
        //fb
        model = glm::translate(wheelOffset, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 4 * 13, 4);
        model = glm::translate(wheelOffset, glm::vec3(0.0f, 0.0f, -40.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 4 * 13, 4);
    }
};

#endif