
# 🔊 Sphere Audio Visualizer

A real-time OpenGL audio visualizer built in C++ that renders a glowing wireframe sphere that pulses and distorts based on the frequency spectrum of an MP3 file.

<p align="center">
  <img src="preview.gif" alt="Visualizer Screenshot" width="500"/>
</p>

---

## 🎧 Features

- 🎵 Real-time audio analysis using [FFTW3](http://www.fftw.org/) and [minimp3](https://github.com/lieff/minimp3)
- 🔊 Visual amplitude spectrum mapped to a 3D wireframe sphere
- 💠 Smooth pulsing wave effect using GLSL vertex/fragment shaders
- 🎨 Dynamic color bloom based on frequency magnitudes
- 🖱️ Interactive camera controls (WASD + mouse)
- 🎛️ ImGui interface for toggling settings

---


## 🛠️ Build Instructions (Visual Studio 2022 + CMake)

### ✅ Prerequisites:
- Visual Studio 2022
- CMake (>= 3.10)
- Git (for cloning dependencies, optional)

### 🚀 Steps:

```bash
git clone https://github.com/yourusername/SphereAudioVisualizer.git
cd Sphere-Audio-Visualizer
```

1. **Open in Visual Studio 2022**:  
Open up solution file

2. **Build** (F7) and **Run** (Ctrl+F5)

---

## 🎹 How It Works

- `minimp3` decodes MP3 audio frames
- `miniaudio` streams them to the OS for playback
- `FFTW3` performs Fast Fourier Transform
- The FFT magnitudes are passed to the vertex shader
- The sphere's geometry and color shift in real-time to reflect the sound spectrum

---

## 🎮 Controls

| Key     | Action              |
|---------|---------------------|
| W/A/S/D | Move camera         |
| Mouse   | Look around         |
| ESC     | Exit                |

---

## 📦 Dependencies (included in `/Dependencies`)

- [GLFW](https://github.com/glfw/glfw)
- [GLAD](https://glad.dav1d.de/)
- [GLM](https://github.com/g-truc/glm)
- [FFTW](http://fftw.org)
- [minimp3](https://github.com/lieff/minimp3)
- [miniaudio](https://github.com/mackron/miniaudio)
- [Dear ImGui](https://github.com/ocornut/imgui)

---

## 🧠 License

This project is open-source and available under the **MIT License**.

---

## 🙌 Acknowledgments

Special thanks to all the libraries above and to [GLSL Sandbox](https://glslsandbox.com/) for inspiration in real-time fragment shader rendering.

---
