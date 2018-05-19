#ifndef LYNX_GL_CANVAS_LYNX_TEXTURE_H_
#define LYNX_GL_CANVAS_LYNX_TEXTURE_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace canvas {
class LxTexture {
 public:
  void Bind() { glBindTexture(GL_TEXTURE_2D, id_); }
  void UnBind() { glBindTexture(GL_TEXTURE_2D, 0); }

  GLuint id() { return id_; }

  void set_id(GLuint texture_id) { id_ = texture_id; }

  unsigned int width() { return width_; }

  void set_width(unsigned int width) { width_ = width; }

  unsigned int height() { return height_; }

  void set_height(unsigned int height) { height_ = height; }

  GLenum format() { return format_; }

  void set_format(GLenum format) { format_ = format; }

  static LxTexture* Create(unsigned int width,
                           unsigned int height,
                           GLenum format,
                           void* data = nullptr) {
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    if (width > max_texture_size || height > max_texture_size) {
      return nullptr;
    }

    GLint binding_texture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &binding_texture);

    LxTexture* texture = new LxTexture;
    GLuint id = 0;
    glGenTextures(1, &id);
    
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, binding_texture);

    texture->set_id(id);
    texture->set_width(width);
    texture->set_height(height);
    texture->set_format(format);

    return texture;
  }

 private:
  GLuint id_;
  unsigned int width_;
  unsigned int height_;
  GLenum format_;
};
}  // namespace canvas

#endif