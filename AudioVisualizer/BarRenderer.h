#ifndef BARRENDERER_H
#define BARRENDERER_H
#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

class BarRenderer {
public:
	BarRenderer(unsigned int barCount);
	void updateInstanceData(const std::vector<float>& heights);
	void draw(const Shader& shader);

private:
	unsigned int VAO, VBO, instanceVBO;
	unsigned int barCount;
};


#endif // !BARRENDERER_H