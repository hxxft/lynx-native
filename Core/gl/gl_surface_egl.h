#ifndef LYNX_GL_GL_SURFACE_EGL_H_
#define LYNX_GL_GL_SURFACE_EGL_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "gl/gl_surface.h"
#include "gl/gl_context_egl.h"

namespace gl {
class GLSurfaceEGL : public GLSurface {
 public:
  GLSurfaceEGL();
  ~GLSurfaceEGL() override;
  EGLDisplay GetDisplay() override;
  EGLConfig GetConfig() override;

 protected:
  EGLConfig config_;
};

class NativeViewGLSurfaceEGL : public GLSurfaceEGL {
 public:
  explicit NativeViewGLSurfaceEGL(EGLNativeWindowType window);
  ~NativeViewGLSurfaceEGL() override;
  using GLSurfaceEGL::Initialize;
  bool Initialize() override;
  EGLSurface GetHandle() override;
  void SwapBuffers() override;
 protected:
  EGLNativeWindowType window_;

 private:
  EGLSurface surface_;
};
}  // namespace gl

#endif