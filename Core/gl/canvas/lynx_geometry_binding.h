#ifndef LYNX_GL_CANVAS_LYNX_GEOMETRY_BINDING_H_
#define LYNX_GL_CANVAS_LYNX_GEOMETRY_BINDING_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "gl/canvas/lynx_color.h"

namespace canvas {
struct GeometryBindingVertex {
  GLfloat position_[2];
  GLfloat tex_coord_[2];
  GLfloat color_[4];
 
  GeometryBindingVertex(){
  }

  GeometryBindingVertex(float x, float y, float tx, float ty, LxColor color){
    Set(x, y, tx, ty, color);
  } 

  void Set(float x, float y, float tx, float ty, LxColor color) {
    position_[0] = x;
    position_[1] = y;
    tex_coord_[0] = tx;
    tex_coord_[1] = ty;
    color_[0] = LxColorGetRf(color);
    color_[1] = LxColorGetGf(color);
    color_[2] = LxColorGetBf(color);
    color_[3] = LxColorGetAf(color);
  }
};

#define VERTEX_BUFFER_SIZE 1024
class LxGeometryBinding {
 public:
  static GLuint PositionAttribLocation () {
    return 0;
  }

  static GLuint TexCoordAttribLocation () {
    return 1;
  }

  static GLuint ColorAttribLocation () {
    return 2;
  }
  LxGeometryBinding();
  ~LxGeometryBinding();
  void SetupBinding();

  void PushVertices(GeometryBindingVertex* vectices, int size); 
  void Flush();
 private:
  GeometryBindingVertex vertex_buffer_[VERTEX_BUFFER_SIZE];
  int buffer_cur_;
};
}  // namespace cavnas

#endif