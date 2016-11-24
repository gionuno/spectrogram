# spectrogram
Real Time Spectrogram in C++

Loads .FLAC files and visualizes a spectrogram in real-time.
Made with OpenGL, GLEW, glfw, glm, and FMOD-Ex (which comes included).

Renders the spectrogram via a simple fragment shader.

2nd edit >>
  - Removed unnecessary libraries. :p
  - Removed commented code.
  - Deleted unnecessary .so

To compile (64 bit linux):

g++ -std=c++14 -o main main.cpp api/lib/libfmodexL64-4.44.61.so -lGLEW -lGL -lglfw


