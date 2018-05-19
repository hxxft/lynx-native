// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_GL_GL_RENDERER_H_
#define LYNX_GL_GL_RENDERER_H_

#include "base/size.h"
#include "base/ref_counted_ptr.h"
#include "base/threading/thread.h"

#include "gl/gl_context.h"
#include "gl/gl_surface.h"

#include "gl/native_widget_types.h"
#include "gl/canvas/lynx_gl_canvas.h"

using namespace canvas;
namespace gl {
class GLRenderer : public base::RefCountPtr<GLRenderer> {
 public:
  GLRenderer();

  void Start();

  void OnAcceleratedWidgetAvailable(AcceleratedWidget widget, base::Size& size);
 private:
  void StartOnGpu();
  void RenderFrame();
  void Flush();
  base::ScopedRefPtr<GLSurface> surface_;
  base::ScopedRefPtr<GLContext> context_;

  base::ScopedPtr<base::Thread> gl_thread_;

  AcceleratedWidget widget_;
  base::Size size_;
  base::ScopedPtr<LxGLCanvas> canvas_;
  //canvas::LxProgramBinding<> render_sreen_program_;
};
}  // namespace gl

#endif