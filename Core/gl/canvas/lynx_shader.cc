#include "gl/canvas/lynx_shader.h"

namespace canvas {
  const char* LxVertexShaderPos::GetShaderString() {
    return "\
    attribute vec4 a_position; \
    attribute vec2 a_texCoord;  \
    attribute vec4 a_color; \
    varying vec2  v_texCoord; \
    varying vec4 v_color; \
    void main() { \
      v_color = a_color; \
      v_texCoord = a_texCoord;  \
      gl_Position = a_position; \
    } \
    ";
  }

  void LxVertexShaderPos::Initialize(GLuint program) {

  }

  const char* LxFragmentShaderColor::GetShaderString() {
    return "\
    precision mediump float; \
    varying vec2  v_texCoord; \
    varying vec4 v_color; \
    void main() { \
      gl_FragColor = v_color; \
    } \
    ";
  }

  void LxFragmentShaderColor::Initialize(GLuint program) {

  }
}