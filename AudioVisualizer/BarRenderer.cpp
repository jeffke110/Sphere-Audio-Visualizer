#include "BarRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

BarRenderer::BarRenderer(unsigned int barCount) : barCount(barCount) {
    float cubeVertices[] = {
        // positions        
        -0.05f, 0.0f, -0.05f,
         0.05f, 0.0f, -0.05f,
         0.05f, 1.0f, -0.05f,
         0.05f, 1.0f, -0.05f,
        -0.05f, 1.0f, -0.05f,
        -0.05f, 0.0f, -0.05f,

        -0.05f, 0.0f,  0.05f,
         0.05f, 0.0f,  0.05f,
         0.05f, 1.0f,  0.05f,
         0.05f, 1.0f,  0.05f,
        -0.05f, 1.0f,  0.05f,
        -0.05f, 0.0f,  0.05f,

        -0.05f, 1.0f,  0.05f,
        -0.05f, 1.0f, -0.05f,
        -0.05f, 0.0f, -0.05f,
        -0.05f, 0.0f, -0.05f,
        -0.05f, 0.0f,  0.05f,
        -0.05f, 1.0f,  0.05f,

         0.05f, 1.0f,  0.05f,
         0.05f, 1.0f, -0.05f,
         0.05f, 0.0f, -0.05f,
         0.05f, 0.0f, -0.05f,
         0.05f, 0.0f,  0.05f,
         0.05f, 1.0f,  0.05f,

        -0.05f, 0.0f, -0.05f,
         0.05f, 0.0f, -0.05f,
         0.05f, 0.0f,  0.05f,
         0.05f, 0.0f,  0.05f,
        -0.05f, 0.0f,  0.05f,
        -0.05f, 0.0f, -0.05f,

        -0.05f, 1.0f, -0.05f,
         0.05f, 1.0f, -0.05f,
         0.05f, 1.0f,  0.05f,
         0.05f, 1.0f,  0.05f,
        -0.05f, 1.0f,  0.05f,
        -0.05f, 1.0f, -0.05f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, barCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);
}

void BarRenderer::updateInstanceData(const std::vector<float>& heights) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, barCount * sizeof(float), heights.data());
}

void BarRenderer::draw(const Shader& shader) {
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, barCount);
    glBindVertexArray(0);
}
