#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float magnitudes[64];

void main()
{
    // Convert position to spherical coordinates
    float theta = atan(aPos.z, aPos.x); // longitude
    float phi = acos(clamp(aPos.y / length(aPos), -1.0, 1.0)); // latitude

    // Normalize angles to [0, 1]
    float u = (theta + 3.14159) / (2.0 * 3.14159); // longitude ? [0, 1]
    float v = phi / 3.14159;                       // latitude ? [0, 1]

    // Map combination of longitude & latitude to FFT band
    int band = int((u + v) * 0.5 * 64.0);
    band = clamp(band, 0, 63);
    float mag = magnitudes[band];

    // Local ripple (wave) effect
    float waveOffset = sin((u + v) * 40.0 + time * 2.0);
    float displacement = waveOffset * mag * 0.25;

    // Global scale: average magnitude across all bands
    float avg = 0.0;
    for (int i = 0; i < 64; ++i) {
        avg += magnitudes[i];
    }
    avg /= 64.0;
    float globalScale = 1.0 + avg * 0.5;

    // Final vertex displacement
    vec3 displaced = normalize(aPos) * (globalScale + displacement);
    gl_Position = projection * view * model * vec4(displaced, 1.0);
}
