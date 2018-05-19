#include "gl/canvas/lynx_program_binding.h"
#include "gl/canvas/lynx_geometry_binding.h"

#include "base/print.h"

#include <memory>

namespace canvas {
LxProgramBindingBase::LxProgramBindingBase()
    : program_(0),
      vertex_shader_id_(0),
      fragment_shader_id_(0),
      initialized_(false) {}

LxProgramBindingBase::~LxProgramBindingBase() {}

bool LxProgramBindingBase::Initialize(const char* vertex_shader,
                                      const char* fragment_shader) {
  vertex_shader_id_ = LoadShader(GL_VERTEX_SHADER, vertex_shader);
  if (vertex_shader_id_ == 0)
    return false;
  fragment_shader_id_ = LoadShader(GL_FRAGMENT_SHADER, fragment_shader);
  if (!fragment_shader_id_) {
    glDeleteShader(vertex_shader_id_);
    fragment_shader_id_ = 0;
    return false;
  }
  program_ = CreateShaderProgram(vertex_shader_id_, fragment_shader_id_);

  return !!program_;
}

bool LxProgramBindingBase::Link() {
  glLinkProgram(program_);
  CleanupShaders();

  if (!program_)
    return false;
}

void LxProgramBindingBase::Cleanup() {
  initialized_ = false;
  if (!program_)
    return;

  glDeleteProgram(program_);
}

GLuint LxProgramBindingBase::LoadShader(GLenum type, const char* shader_src) {
  GLuint shader;
  GLint compiled;

  shader = glCreateShader(type);

  if (shader == 0) {
    return 0;
  }

  glShaderSource(shader, 1, &shader_src, NULL);

  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLint info_len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len > 1) {
      char* info_log = (char*)malloc(sizeof(char) * info_len);
      glGetShaderInfoLog(shader, info_len, NULL, info_log);
      LOGE("lynx-shader", "Eoor compiling shader:%s", info_log);
      free(info_log);
    }
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

void LxProgramBindingBase::CleanupShaders() {
  if (vertex_shader_id_) {
    glDeleteShader(vertex_shader_id_);
    vertex_shader_id_ = 0;
  }

  if (fragment_shader_id_) {
    glDeleteShader(fragment_shader_id_);
    fragment_shader_id_ = 0;
  }
}

GLuint LxProgramBindingBase::CreateShaderProgram(GLuint vertex_shader,
                                                 GLuint fragment_shader) {
  GLuint program_object = glCreateProgram();
  if (program_object == 0) {
    return 0;
  }

  glAttachShader(program_object, vertex_shader);
  glAttachShader(program_object, fragment_shader);

  glBindAttribLocation(program_object,
                       LxGeometryBinding::PositionAttribLocation(),
                       "a_position");
  glBindAttribLocation(program_object,
                       LxGeometryBinding::TexCoordAttribLocation(),
                       "a_texCoord");
  glBindAttribLocation(program_object,
                       LxGeometryBinding::ColorAttribLocation(),
                       "a_color");

  return program_object;
}

}  // namespace canvas