#ifndef LYNX_GL_GL_SURFACE_H_
#define LYNX_GL_GL_SURFACE_H_

#include "base/ref_counted_ptr.h"
#include "gl/native_widget_types.h"

namespace gl {
  class GLSurface : public base::RefCountPtr<GLSurface>{
    public:
    GLSurface();
    virtual ~GLSurface();

    virtual bool Initialize() = 0;
    //virtual void Destory() = 0;
    virtual void* GetHandle() = 0;

    virtual void SwapBuffers() = 0;
    //virtual bool Resize();

    virtual void* GetDisplay();

    virtual void* GetConfig();

    static base::ScopedRefPtr<GLSurface> CreateViewGLSurface(AcceleratedWidget window);
  };
}

#endif