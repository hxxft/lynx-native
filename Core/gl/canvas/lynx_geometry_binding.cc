#include <base/print.h>
#include "gl/canvas/lynx_geometry_binding.h"

namespace canvas {
LxGeometryBinding::LxGeometryBinding(): buffer_cur_(0) {
} 

LxGeometryBinding::~LxGeometryBinding(){
}

void LxGeometryBinding::SetupBinding() {
  glVertexAttribPointer(LxGeometryBinding::PositionAttribLocation(), 2,
                        GL_FLOAT, GL_FALSE, sizeof(GeometryBindingVertex),
                        vertex_buffer_);
  glVertexAttribPointer(LxGeometryBinding::TexCoordAttribLocation(), 2,
                        GL_FLOAT, GL_FALSE, sizeof(GeometryBindingVertex),
                        (GLfloat*)(vertex_buffer_)  + 2);
  glVertexAttribPointer(LxGeometryBinding::ColorAttribLocation(), 4, GL_FLOAT,
                        GL_FALSE, sizeof(GeometryBindingVertex),
                        (GLfloat*)(vertex_buffer_) + 4);
  glEnableVertexAttribArray(LxGeometryBinding::PositionAttribLocation());
  glEnableVertexAttribArray(LxGeometryBinding::TexCoordAttribLocation());
  glEnableVertexAttribArray(LxGeometryBinding::ColorAttribLocation());
}

void LxGeometryBinding::PushVertices(GeometryBindingVertex* vectices,
                                     int size) {
  if (buffer_cur_ + size >= VERTEX_BUFFER_SIZE) {
    Flush();
  }
  GeometryBindingVertex* binding_vertex = &vertex_buffer_[buffer_cur_];
  for(int i = 0; i < size; ++i) {
    binding_vertex[i] = vectices[i];
  }
  buffer_cur_ += size; 
}

void LxGeometryBinding::Flush() {
  if (buffer_cur_ > 0) {
    glDrawArrays(GL_TRIANGLES, 0, buffer_cur_);
    buffer_cur_ = 0;
  }
}
};  // namespace canvas