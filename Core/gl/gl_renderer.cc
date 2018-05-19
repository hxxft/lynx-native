// Copyright 2017 The Lynx Authors. All rights reserved.

#include "gl/gl_renderer.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "base/log/logging.h"

namespace gl {
GLRenderer::GLRenderer() {}

void GLRenderer::OnAcceleratedWidgetAvailable(AcceleratedWidget widget,
                                              base::Size& size) {
  widget_ = widget;
  size_ = size;
  base::ScopedRefPtr<GLRenderer> ref(this);
  gl_thread_->Looper()->PostTask(base::Bind(&GLRenderer::StartOnGpu, ref));
}

void GLRenderer::Start() {
  gl_thread_.Reset(
      lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_POSIX));
  gl_thread_->Start();
}

void GLRenderer::StartOnGpu() {

  surface_ = GLSurface::CreateViewGLSurface(widget_);
  if (!surface_.Get()) {
    LOG(ERROR) << "egl create glsurface failed ";
    return;
  }
  context_ = GLContext::CreateGLContext(nullptr, surface_.Get());
  if (!context_.Get()) {
    LOG(ERROR) << "egl create glcontext failed ";
    return;
  }
  if (!context_->MakeCurrent(surface_.Get())) {
    LOG(ERROR) << "Failed to make GL context current";
    return;
  }

  canvas_.Reset(new LxGLCanvas(size_.width_, size_.height_));
  RenderFrame();
}


#define VERTEX_ARRAY 0
#define TEX_ARRAY 1
#define COLOR_ARRAY 2
void GLRendererTest()
{
    bool bRet = false;
  
    //单元矩阵，用于投影和模型变换
    float pfIdentity[] =
    {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
  
    // Vertex和Fragment的shader
    char szFragShaderSrc[] = {"\
                               precision mediump float;\
                              varying vec4 v_color; \
                              varying vec2 v_texCoord; \
                              uniform sampler2D sampler; \
                              void main (void)\
                              {\
                              vec4 col = texture2D (sampler, v_texCoord); \ 
                              gl_FragColor = v_color + col;\
                              }"};
    char szVertShaderSrc[] = {"\
                              attribute vec4 myVertex;\
                              attribute vec2 a_texCoord; \
                              attribute vec4 a_color; \
                              uniform mat4 myPMVMatrix;\
                              varying vec4 v_color; \
                              varying vec2 v_texCoord; \
                              void main(void)\
                              {\
                              v_color = a_color;\
                              v_texCoord = a_texCoord; \ 
                              gl_Position = myPMVMatrix * myVertex;\
                              }"};
  
    char * pszFragShader = (char *)szFragShaderSrc;
    char * pszVertShader = (char *)szVertShaderSrc;
  
    GLuint uiFragShader = 0;
    GLuint uiVertShader = 0; /* 用来存放Vertex和Fragment shader的句柄 */ 
    GLuint uiProgramObject = 0; /* 用来存放创建的program的句柄*/ 
  
    GLint bShaderCompiled;
    GLint bLinked;
  
    // 我们要画一个三角形，所以，我们先创建一个顶点缓冲区
    GLuint ui32Vbo = 0; // 顶点缓冲区对象句柄
  
    //顶点数据 这9个数据分别为3个点的X、Y、Z坐标
    GLfloat afVertices[] = { -0.4f,-0.4f,0.0f, // Position
                             1.0f, 0.0f, 0.0f, 1.0f, //color
                                0.4f ,-0.4f,0.0f,
                             0.0f, 1.0f, 0.0f, 1.0f,
                                0.0f ,0.4f ,0.0f,
                             0.0f, 0.0f, 1.0f, 1.0f,};

    GeometryBindingVertex gvertics[3];
    gvertics[0].Set(-0.4f, -0.4f, 0, 0, Lx_ColorRED);
    gvertics[1].Set(0.4f, -0.4f, 0, 0, Lx_ColorRED);
    gvertics[2].Set(0.0f, 0.4f, 0, 0, Lx_ColorRED);
  
    int i32InfoLogLength, i32CharsWritten;
    char* pszInfoLog = NULL;
    int i32Location = 0;
    unsigned int uiSize = 0;
  
    //创建Fragment着色器对象
    uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);
  
    // 将代码加载进来
    glShaderSource(uiFragShader, 1, (const char**)&pszFragShader, NULL);
  
    //编译代码
    glCompileShader(uiFragShader);
  
    //看编译是否成功进行
    glGetShaderiv(uiFragShader, GL_COMPILE_STATUS, &bShaderCompiled);
  
    if (!bShaderCompiled)
    {
        // 错误发生，首先获取错误的长度
        glGetShaderiv(uiFragShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
        //开辟足够的空间来存储错误信息
        pszInfoLog = new char[i32InfoLogLength + 1];
        glGetShaderInfoLog(uiFragShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
        pszInfoLog[i32InfoLogLength] = 0;
        LOGE("lynx", "shader error:%s", pszInfoLog);
        delete[] pszInfoLog;
        goto cleanup;
    }
  
    // 使用同样的方法加载Vertex Shader
    uiVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(uiVertShader, 1, (const char**)&pszVertShader, NULL);
    glCompileShader(uiVertShader);
    glGetShaderiv(uiVertShader, GL_COMPILE_STATUS, &bShaderCompiled);
    if (!bShaderCompiled)
    {
        glGetShaderiv(uiVertShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
        pszInfoLog = new char[i32InfoLogLength + 1];
        glGetShaderInfoLog(uiVertShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
         pszInfoLog[i32InfoLogLength] = 0;
        delete[] pszInfoLog;
        goto cleanup;
    }
  
    // 创建着色器程序
    uiProgramObject = glCreateProgram();
  
    // 将Vertex和Fragment Shader绑定进去。
    glAttachShader(uiProgramObject, uiFragShader);
    glAttachShader(uiProgramObject, uiVertShader);
  
    //将用户自定义的顶点属性myVertex绑定到VERTEX_ARRAY。
    glBindAttribLocation(uiProgramObject, VERTEX_ARRAY, "myVertex");
    glBindAttribLocation(uiProgramObject, COLOR_ARRAY, "a_color");
    glBindAttribLocation(uiProgramObject, TEX_ARRAY, "a_texCoord");
  
    // 链接着色器程序
    glLinkProgram(uiProgramObject);
  
    // 判断链接是否成功的操作
    glGetProgramiv(uiProgramObject, GL_LINK_STATUS, &bLinked);
    if (!bLinked)
    {
        glGetProgramiv(uiProgramObject, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
        pszInfoLog = new char[i32InfoLogLength];
        glGetProgramInfoLog(uiProgramObject, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
        delete[] pszInfoLog;
        goto cleanup;
    }
  
    // 使用着色器程序
    glUseProgram(uiProgramObject);
  
    // 设置清除颜色，以RGBA的模式，每个分量的值从0.0到1.0
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
  
    //生成一个顶点缓冲区对象
    //glGenBuffers(1, &ui32Vbo);
  
    //绑定生成的缓冲区对象到GL_ARRAY_BUFFER
    //glBindBuffer(GL_ARRAY_BUFFER, ui32Vbo);
  
    // 加载顶点数据
    //uiSize = 3 * (sizeof(GLfloat) * 3) + 3 * (sizeof(GLfloat) * 4); // Calc afVertices size (3 vertices * stride (3 GLfloats per vertex))
    //glBufferData(GL_ARRAY_BUFFER, uiSize, afVertices, GL_STATIC_DRAW);
  
    // 画三角形
    {
        //清除颜色缓冲区。glClear同样也能清除深度缓冲区(GL_DEPTH_BUFFER)和模板缓冲区(GL_STENCIL_BUFFER_BIT)
        glClear(GL_COLOR_BUFFER_BIT);
  
  
        //获取myPMVMatrix在shader中的位置
        i32Location = glGetUniformLocation(uiProgramObject, "myPMVMatrix");
  
        //传值给获取到的位置，也就是将pfIdentity传给myPMVMatrix
        glUniformMatrix4fv( i32Location, 1, GL_FALSE, pfIdentity);
  
        //将VERTEX_ARRAY置为有效。
        glEnableVertexAttribArray(VERTEX_ARRAY);
  
        // 将定点数据传到VERTEX_ARRAY
        glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryBindingVertex), gvertics);

//将VERTEX_ARRAY置为有效。
        glEnableVertexAttribArray(TEX_ARRAY);
  
        // 将定点数据传到VERTEX_ARRAY
        glVertexAttribPointer(TEX_ARRAY, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryBindingVertex), (float*)(gvertics) + 2);

        //将VERTEX_ARRAY置为有效。
        glEnableVertexAttribArray(COLOR_ARRAY);

        // 将定点数据传到VERTEX_ARRAY
        glVertexAttribPointer(COLOR_ARRAY, 4, GL_FLOAT, GL_FALSE, sizeof(GeometryBindingVertex), (float*)(gvertics) + 4);
  
        //画一个三角形。
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    bRet = true;
  
cleanup:
    // 释放资源
    if (uiProgramObject)
        glDeleteProgram(uiProgramObject);
    if (uiFragShader)
        glDeleteShader(uiFragShader);
    if (uiVertShader)
        glDeleteShader(uiVertShader);
  
    // Delete the VBO as it is no longer needed
    if (ui32Vbo)
        glDeleteBuffers(1, &ui32Vbo);
  
    //return bRet;
}

void GLRenderer::Flush() {
     canvas::LxPaint paint;
     paint.SetColor(Lx_ColorRED);
     paint.SetStrokeWidth(10);
    //  canvas::LxPath path;
    //  path.MoveTo(100,100);
    //  path.LineTo(500, 100);
    //  path.ArcTo(750, 100, 750, 350, 250);
    //  path.LineTo(750, 600);
    //  path.Stroke();
    // canvas_->DrawPath(path, paint);

    canvas_->DrawArc(400, 400, 50, 0, 360, paint);
    canvas_->Flush();
}

void GLRenderer::RenderFrame() {


    //GLRendererTest();
    Flush();

  surface_->SwapBuffers();
}

}  // namespace gl