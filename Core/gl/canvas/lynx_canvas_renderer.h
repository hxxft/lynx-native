#ifndef GL_LYNX_CANVAS_LYNX_CANVAS_RENDERER_H_
#define GL_LYNX_CANVAS_LYNX_CANVAS_RENDERER_H_

#include "gl/canvas/lynx_geometry_binding.h"
#include "gl/canvas/lynx_paint.h"
#include "gl/canvas/lynx_program_binding.h"
#include "gl/canvas/lynx_shader.h"
#include "gl/canvas/lynx_texture.h"

namespace canvas {
class LxCanvasRenderer {
 public:
 LxCanvasRenderer(int width, int height, int format);
 ~LxCanvasRenderer();
  void DrawVertices(const LxPaint* paint, GeometryBindingVertex* vertices, int size);

  void BindOffSceenFBO();
  void UnBindOffSceenFBO();
  void Flush();

 private:
  void UseProgram(int program);
  void Initialize(int width, int height, int format);
  typedef LxProgramBinding<LxVertexShaderPos, LxFragmentShaderColor>
      SoildColorProgram;

  SoildColorProgram* soild_color_program();

  SoildColorProgram soild_color_program_;

  LxGeometryBinding binding_vertex_;

  GLuint off_screen_fbo_;
  GLint saved_fbo_;
  GLuint rbo_;

  LxTexture* texture_;

  int current_program_;
};
}  // namespace canvas

#endif