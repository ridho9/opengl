#ifndef SMOKE_PARTICLE
#define SMOKE_PARTICLE

#include <stdlib.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

static const GLfloat particle_vertices[] = {
    -1, -1, 0.0f, //
    1, -1, 0.0f,  //
    -1, 1, 0.0f,  //
    1, 1, 0.0f,   //
};

class ParticleRenderer
{
  public:
    Shader shader;
    GLuint VAO, VBO;
    ParticleRenderer() : shader("particle.vs", "particle.fs")
    {
        // Set Up VBO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Set up VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particle_vertices), particle_vertices, GL_STATIC_DRAW);

        // Set up vertex attrib array
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              3 * sizeof(float), 0);
    }
};

struct SmokeParticle
{
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 speed = glm::vec3(0.0f);
    float rot = 0, life = 0;

    void update(double deltaTime)
    {
        int erratic = 1.5;
        pos += speed;

        life -= deltaTime;
        if (life <= 0)
        {
            speed = glm::vec3(
                (float)(rand() % 10 - 5) * deltaTime * erratic,
                10.0f * deltaTime,
                -(float)(rand() % 10) * deltaTime * erratic);

            rot = (float)(rand() % 360);

            life = (float)((rand() % 50) + 10) / 10;
            pos = glm::vec3(0.0f);
        }
    }

    void draw(ParticleRenderer &renderer, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        renderer.shader.use();

        model = glm::translate(model, glm::vec3(20.0f, 5.0f, -142.0f));
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0, 1.0, 0.0));
        renderer.shader.setMat4("model", model);
        renderer.shader.setMat4("view", view);
        renderer.shader.setMat4("projection", projection);

        renderer.shader.setVec3("particleColor", glm::vec3(0.5f));
        renderer.shader.setFloat("alpha", life / 4.5);

        glBindVertexArray(renderer.VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};

#endif