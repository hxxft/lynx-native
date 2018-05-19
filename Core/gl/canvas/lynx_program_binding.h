#ifndef LYNX_GL_CANVAS_LYNX_PROGRAM_BINDING_H_
#define LYNX_GL_CANVAS_LYNX_PROGRAM_BINDING_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "gl/canvas/lynx_geometry_binding.h"

namespace canvas {

class LxProgramBindingBase {
 public:
  LxProgramBindingBase();
  ~LxProgramBindingBase();
  bool Initialize(const char* vertex_shader, const char* fragment_shader);
  bool Link();
  void Cleanup();

  GLuint program() const { return program_; }
  bool initialized() const { return initialized_; }

 protected:
  GLuint LoadShader(GLenum type, const char* shader_src);
  GLuint CreateShaderProgram(GLuint vertex_shader, GLuint fragment_shader);
  void CleanupShaders();

  GLuint program_;
  GLuint vertex_shader_id_;
  GLuint fragment_shader_id_;
  bool initialized_;
};

template <class VertexShader, class FragmentShader>
class LxProgramBinding : public LxProgramBindingBase {
 public:
  LxProgramBinding() {}
  void Initialize() {
    if (!LxProgramBindingBase::Initialize(vertex_shader_.GetShaderString(),
                                          fragment_shader_.GetShaderString())) {
      return;
    }
    vertex_shader_.Initialize(program_); 
    fragment_shader_.Initialize(program_); 
    //LxGeometryBinding::SetupBinding();
    if(!Link()){
      return;
    }

    initialized_ = true;
  }

  const VertexShader& vertex_shader() const { return vertex_shader_; }
  const FragmentShader& fragment_shader() const { return fragment_shader_; }

 private:
  VertexShader vertex_shader_;
  FragmentShader fragment_shader_;
};
}  // namespace canvas

#endif