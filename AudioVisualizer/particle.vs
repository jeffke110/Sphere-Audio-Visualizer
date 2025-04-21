#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float magnitudes[64];

void main()
{
    // Calculate the average magnitude
    float avg = 0.0;
    for (int i = 0; i < 64; ++i) {
        avg += magnitudes[i];
    }
    avg /= 64.0;

    // Scale the entire sphere based on audio intensity
    float scale = 1.0 + avg * 0.5;

    // Expand vertex position outward
    vec3 displaced = normalize(aPos) * scale;
    gl_Position = projection * view * model * vec4(displaced, 1.0);
}
