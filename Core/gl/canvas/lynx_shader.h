#ifndef LYNX_GL_CANVAS_LYNX_SHADER_H_
#define LYNX_GL_CANVAS_LYNX_SHADER_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace canvas {
  class LxVertexShader {
  public:
    virtual const char* GetShaderString() = 0;
    virtual void Initialize(GLuint program) = 0;
  };

  class LxFragmentShader {
    public:
    virtual const char* GetShaderString() = 0;
    virtual void Initialize(GLuint program) = 0;
  };
  
  class LxVertexShaderPos : public LxVertexShader {
    public:
    virtual const char* GetShaderString() override;
     virtual void Initialize(GLuint program) override;
  };

  class LxFragmentShaderColor : public LxFragmentShader {
    public:
    virtual const char* GetShaderString() override;  
    virtual void Initialize(GLuint program) override;
  };
}

#endif