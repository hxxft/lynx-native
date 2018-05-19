#include "gl/canvas/lynx_canvas_renderer.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "base/print.h"
#include <string.h>

namespace canvas {

LxCanvasRenderer::LxCanvasRenderer(int width, int height, int format)
    : current_program_(-1) {
  Initialize(width, height, format);
  binding_vertex_.SetupBinding();
}

LxCanvasRenderer::~LxCanvasRenderer() {}

void LxCanvasRenderer::DrawVertices(const LxPaint* paint,
                                    GeometryBindingVertex* vertices,
                                    int size) {
  UseProgram(soild_color_program()->program());
  binding_vertex_.PushVertices(vertices, size);
}

LxCanvasRenderer::SoildColorProgram* LxCanvasRenderer::soild_color_program() {
  if (!soild_color_program_.initialized()) {
    soild_color_program_.Initialize();
  }
  return &soild_color_program_;
}

void LxCanvasRenderer::UseProgram(int program) {
  if (current_program_ != program) {
    binding_vertex_.Flush();
    glUseProgram(program);
    current_program_ = program; 
  } 
}

void LxCanvasRenderer::Initialize(int width, int height, int format) {
  // glGenFramebuffers(1, &off_screen_fbo_);
  // BindOffSceenFBO();
  // texture_ = LxTexture::Create(width, height, format);
  // if (texture_ == nullptr)
  //   return;
  // texture_->Bind();
  // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
  //                        texture_->id(), 0);

  // const char* externs = (const char*)glGetString(GL_EXTENSIONS);
  // bool OES_packed_depth_stencil =
  //     (strstr(externs, "GL_OES_packed_depth_stencil") != 0);

  // if (OES_packed_depth_stencil) {
  //   glGenRenderbuffers(1, &rbo_);
  //   glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  //   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES,
  //                         texture_->width(), texture_->height());
  //   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
  //                             GL_RENDERBUFFER, rbo_);
  //   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
  //                             GL_RENDERBUFFER, rbo_);
  // } else {
  //   glGenRenderbuffers(1, &rbo_);
  //   glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  //   glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, texture_->width(),
  //                         texture_->height());
  //   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
  //                             GL_RENDERBUFFER, rbo_);
  // }
  // glClearColor(1, 1, 1, 1);
  // glClear(GL_COLOR_BUFFER_BIT);
  // GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  // if (status != GL_FRAMEBUFFER_COMPLETE) {
  //   LOGE("lynx", "failed to make complete framebuffer object %x", (int)status);
  // }
  // UnBindOffSceenFBO();


  // glEnable(GL_BLEND);
  // glDepthFunc(GL_ALWAYS);

  // glViewport(0, 0, width, height);
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  //glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void LxCanvasRenderer::BindOffSceenFBO() {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &saved_fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, off_screen_fbo_);
}

void LxCanvasRenderer::UnBindOffSceenFBO() {
  glBindFramebuffer(GL_FRAMEBUFFER, saved_fbo_);
}

void LxCanvasRenderer::Flush() {
  binding_vertex_.Flush();
}
}  // namespace canvas