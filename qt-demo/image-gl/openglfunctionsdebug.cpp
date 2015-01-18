#include "openglfunctionsdebug.h"

#include <QDebug>

#define GL_DEBUG  { GLenum error = glGetError(); if(error != GL_NO_ERROR){ qDebug() << __FUNCTION__ << error;  } }


OpenGLFunctionsDebug::OpenGLFunctionsDebug():
    QOpenGLFunctions()
{

}

OpenGLFunctionsDebug::~OpenGLFunctionsDebug()
{

}

OpenGLFunctionsDebug::OpenGLFunctionsDebug(QOpenGLContext * context):
    QOpenGLFunctions(context)
{

}

void	OpenGLFunctionsDebug::glActiveTexture(GLenum texture){
    QOpenGLFunctions::glActiveTexture(texture);{ GLenum error = glGetError(); if(error != GL_NO_ERROR){ qDebug() << __FUNCTION__ << error;  } }
}

void	OpenGLFunctionsDebug::glAttachShader(GLuint program, GLuint shader){
    QOpenGLFunctions::glAttachShader(program,shader);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBindAttribLocation(GLuint program, GLuint index, const char * name){
    QOpenGLFunctions::glBindAttribLocation(program,index,name);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBindBuffer(GLenum target, GLuint buffer){
    QOpenGLFunctions::glBindBuffer(target,buffer);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBindFramebuffer(GLenum target, GLuint framebuffer){
    QOpenGLFunctions::glBindFramebuffer(target,framebuffer);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBindRenderbuffer(GLenum target, GLuint renderbuffer){
    QOpenGLFunctions::glBindRenderbuffer(target,renderbuffer);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBindTexture(GLenum target, GLuint texture){
    QOpenGLFunctions::glBindTexture(target,texture);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha){
    QOpenGLFunctions::glBlendColor(red,green,blue,alpha);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBlendEquation(GLenum mode){
    QOpenGLFunctions::glBlendEquation(mode);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha){
    QOpenGLFunctions::glBlendEquationSeparate(modeRGB,modeAlpha);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBlendFunc(GLenum sfactor, GLenum dfactor){
    QOpenGLFunctions::glBlendFunc(sfactor,dfactor);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
    QOpenGLFunctions::glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBufferData(GLenum target, qopengl_GLsizeiptr size, const void * data, GLenum usage){
    QOpenGLFunctions::glBufferData(target,size,data,usage);GL_DEBUG
}

void	OpenGLFunctionsDebug::glBufferSubData(GLenum target, qopengl_GLintptr offset, qopengl_GLsizeiptr size, const void * data){
    QOpenGLFunctions::glBufferSubData(target,offset,size,data);GL_DEBUG
}

GLenum	OpenGLFunctionsDebug::glCheckFramebufferStatus(GLenum target){
    return QOpenGLFunctions::glCheckFramebufferStatus(target);GL_DEBUG
}

void	OpenGLFunctionsDebug::glClear(GLbitfield mask){
    QOpenGLFunctions::glClear(mask);GL_DEBUG
}

void	OpenGLFunctionsDebug::glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha){
    QOpenGLFunctions::glClearColor(red,green,blue,alpha);GL_DEBUG
}

void	OpenGLFunctionsDebug::glClearDepthf(GLclampf depth){
    QOpenGLFunctions::glClearDepthf(depth);GL_DEBUG
}

void	OpenGLFunctionsDebug::glClearStencil(GLint s){
    QOpenGLFunctions::glClearStencil(s);GL_DEBUG
}

void	OpenGLFunctionsDebug::glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha){
    QOpenGLFunctions::glColorMask(red,green,blue,alpha);GL_DEBUG
}

void	OpenGLFunctionsDebug::glCompileShader(GLuint shader){
    QOpenGLFunctions::glCompileShader(shader);GL_DEBUG
}

void	OpenGLFunctionsDebug::glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data){
    glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data);GL_DEBUG
}

void	OpenGLFunctionsDebug::glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data){
    QOpenGLFunctions::glCompressedTexSubImage2D(target,level,xoffset,yoffset,width,height,format,imageSize,data);GL_DEBUG
}

void	OpenGLFunctionsDebug::glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border){
    QOpenGLFunctions::glCopyTexImage2D(target,level,internalformat,x,y,width,height,border);GL_DEBUG
}

void	OpenGLFunctionsDebug::glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
    QOpenGLFunctions::glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height);GL_DEBUG
}

GLuint	OpenGLFunctionsDebug::glCreateProgram(){
    return QOpenGLFunctions::glCreateProgram();GL_DEBUG
}

GLuint	OpenGLFunctionsDebug::glCreateShader(GLenum type){
    return QOpenGLFunctions::glCreateShader(type);GL_DEBUG
}

void	OpenGLFunctionsDebug::glCullFace(GLenum mode){
    QOpenGLFunctions::glCullFace(mode);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDeleteBuffers(GLsizei n, const GLuint * buffers){
    QOpenGLFunctions::glDeleteBuffers(n,buffers);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers){
    QOpenGLFunctions::glDeleteFramebuffers(n,framebuffers);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDeleteProgram(GLuint program){
    QOpenGLFunctions::glDeleteProgram(program);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers){
    QOpenGLFunctions::glDeleteRenderbuffers(n,renderbuffers);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDeleteShader(GLuint shader){
    QOpenGLFunctions::glDeleteShader(shader);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDeleteTextures(GLsizei n, const GLuint * textures){
    QOpenGLFunctions::glDeleteTextures(n,textures);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDepthFunc(GLenum func){
    QOpenGLFunctions::glDepthFunc(func);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDepthMask(GLboolean flag){
    QOpenGLFunctions::glDepthMask(flag);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDepthRangef(GLclampf zNear, GLclampf zFar){
    QOpenGLFunctions::glDepthRangef(zNear,zFar);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDetachShader(GLuint program, GLuint shader){
    QOpenGLFunctions::glDetachShader(program,shader);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDisable(GLenum cap){
    QOpenGLFunctions::glDisable(cap);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDisableVertexAttribArray(GLuint index){
    QOpenGLFunctions::glDisableVertexAttribArray(index);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDrawArrays(GLenum mode, GLint first, GLsizei count){
    QOpenGLFunctions::glDrawArrays(mode,first,count);GL_DEBUG
}

void	OpenGLFunctionsDebug::glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices){
    QOpenGLFunctions::glDrawElements(mode,count,type,indices);GL_DEBUG
}

void	OpenGLFunctionsDebug::glEnable(GLenum cap){
    QOpenGLFunctions::glEnable(cap);GL_DEBUG
}

void	OpenGLFunctionsDebug::glEnableVertexAttribArray(GLuint index){
    QOpenGLFunctions::glEnableVertexAttribArray(index);GL_DEBUG
}

void	OpenGLFunctionsDebug::glFinish(){
    QOpenGLFunctions::glFinish();GL_DEBUG
}

void	OpenGLFunctionsDebug::glFlush(){
    QOpenGLFunctions::glFlush();GL_DEBUG
}

void	OpenGLFunctionsDebug::glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
    QOpenGLFunctions::glFramebufferRenderbuffer(target,attachment,renderbuffertarget,renderbuffer);GL_DEBUG
}

void	OpenGLFunctionsDebug::glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
    QOpenGLFunctions::glFramebufferTexture2D(target,attachment,textarget,texture,level);GL_DEBUG
}

void	OpenGLFunctionsDebug::glFrontFace(GLenum mode){
    QOpenGLFunctions::glFrontFace(mode);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGenBuffers(GLsizei n, GLuint * buffers){
    QOpenGLFunctions::glGenBuffers(n,buffers);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGenFramebuffers(GLsizei n, GLuint * framebuffers){
    QOpenGLFunctions::glGenFramebuffers(n,framebuffers);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGenRenderbuffers(GLsizei n, GLuint * renderbuffers){
    QOpenGLFunctions::glGenRenderbuffers(n,renderbuffers);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGenTextures(GLsizei n, GLuint * textures){
    QOpenGLFunctions::glGenTextures(n,textures);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGenerateMipmap(GLenum target){
    QOpenGLFunctions::glGenerateMipmap(target);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei * length, GLint * size, GLenum * type, char * name){
    QOpenGLFunctions::glGetActiveAttrib(program,index,bufsize,length,size,type,name);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei * length, GLint * size, GLenum * type, char * name){
    QOpenGLFunctions::glGetActiveUniform(program,index,bufsize,length,size,type,name);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei * count, GLuint * shaders){
    QOpenGLFunctions::glGetAttachedShaders(program,maxcount,count,shaders);GL_DEBUG
}

GLint	OpenGLFunctionsDebug::glGetAttribLocation(GLuint program, const char * name){
    return QOpenGLFunctions::glGetAttribLocation(program,name);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetBooleanv(GLenum pname, GLboolean * params){
    QOpenGLFunctions::glGetBooleanv(pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetBufferParameteriv(GLenum target, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetBufferParameteriv(target,pname,params);GL_DEBUG
}

GLenum	OpenGLFunctionsDebug::glGetError(){
    return QOpenGLFunctions::glGetError();GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetFloatv(GLenum pname, GLfloat * params){
    QOpenGLFunctions::glGetFloatv(pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetFramebufferAttachmentParameteriv(target,attachment,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetIntegerv(GLenum pname, GLint * params){
    QOpenGLFunctions::glGetIntegerv(pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei * length, char * infolog){
    QOpenGLFunctions::glGetProgramInfoLog(program,bufsize,length,infolog);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetProgramiv(GLuint program, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetProgramiv(program,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetRenderbufferParameteriv(target,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei * length, char * infolog){
    QOpenGLFunctions::glGetShaderInfoLog(shader,bufsize,length,infolog);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision){
    QOpenGLFunctions::glGetShaderPrecisionFormat(shadertype,precisiontype,range,precision);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei * length, char * source){
    QOpenGLFunctions::glGetShaderSource(shader,bufsize,length,source);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetShaderiv(GLuint shader, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetShaderiv(shader,pname,params);GL_DEBUG
}

const GLubyte *	OpenGLFunctionsDebug::glGetString(GLenum name){
    QOpenGLFunctions::glGetString(name);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params){
    QOpenGLFunctions::glGetTexParameterfv(target,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetTexParameteriv(GLenum target, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetTexParameteriv(target,pname,params);GL_DEBUG
}

GLint	OpenGLFunctionsDebug::glGetUniformLocation(GLuint program, const char * name){
    return QOpenGLFunctions::glGetUniformLocation(program,name);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetUniformfv(GLuint program, GLint location, GLfloat * params){
    QOpenGLFunctions::glGetUniformfv(program,location,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetUniformiv(GLuint program, GLint location, GLint * params){
    QOpenGLFunctions::glGetUniformiv(program,location,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetVertexAttribPointerv(GLuint index, GLenum pname, void ** pointer){
    QOpenGLFunctions::glGetVertexAttribPointerv(index,pname,pointer);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params){
    QOpenGLFunctions::glGetVertexAttribfv(index,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glGetVertexAttribiv(GLuint index, GLenum pname, GLint * params){
    QOpenGLFunctions::glGetVertexAttribiv(index,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glHint(GLenum target, GLenum mode){
    QOpenGLFunctions::glHint(target,mode);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsBuffer(GLuint buffer){
    return QOpenGLFunctions::glIsBuffer(buffer);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsEnabled(GLenum cap){
    return QOpenGLFunctions::glIsEnabled(cap);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsFramebuffer(GLuint framebuffer){
    return QOpenGLFunctions::glIsFramebuffer(framebuffer);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsProgram(GLuint program){
    return QOpenGLFunctions::glIsProgram(program);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsRenderbuffer(GLuint renderbuffer){
    return QOpenGLFunctions::glIsRenderbuffer(renderbuffer);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsShader(GLuint shader){
    return QOpenGLFunctions::glIsShader(shader);GL_DEBUG
}

GLboolean	OpenGLFunctionsDebug::glIsTexture(GLuint texture){
    return QOpenGLFunctions::glIsTexture(texture);GL_DEBUG
}

void	OpenGLFunctionsDebug::glLineWidth(GLfloat width){
    QOpenGLFunctions::glLineWidth(width);GL_DEBUG
}

void	OpenGLFunctionsDebug::glLinkProgram(GLuint program){
    QOpenGLFunctions::glLinkProgram(program);GL_DEBUG
}

void	OpenGLFunctionsDebug::glPixelStorei(GLenum pname, GLint param){
    QOpenGLFunctions::glPixelStorei(pname,param);GL_DEBUG
}

void	OpenGLFunctionsDebug::glPolygonOffset(GLfloat factor, GLfloat units){
    return QOpenGLFunctions::glPolygonOffset(factor,units);GL_DEBUG
}

void	OpenGLFunctionsDebug::glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels){
    QOpenGLFunctions::glReadPixels(x,y,width,height,format,type,pixels);GL_DEBUG
}

void	OpenGLFunctionsDebug::glReleaseShaderCompiler(){
    QOpenGLFunctions::glReleaseShaderCompiler();GL_DEBUG
}

void	OpenGLFunctionsDebug::glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height){
    QOpenGLFunctions::glRenderbufferStorage(target,internalformat,width,height);GL_DEBUG
}

void	OpenGLFunctionsDebug::glSampleCoverage(GLclampf value, GLboolean invert){
    QOpenGLFunctions::glSampleCoverage(value,invert);GL_DEBUG
}

void	OpenGLFunctionsDebug::glScissor(GLint x, GLint y, GLsizei width, GLsizei height){
    QOpenGLFunctions::glScissor(x,y,width,height);GL_DEBUG
}

void	OpenGLFunctionsDebug::glShaderBinary(GLint n, const GLuint * shaders, GLenum binaryformat, const void * binary, GLint length){
    QOpenGLFunctions::glShaderBinary(n,shaders,binaryformat,binary,length);GL_DEBUG
}

void	OpenGLFunctionsDebug::glShaderSource(GLuint shader, GLsizei count, const char ** string, const GLint * length){
    QOpenGLFunctions::glShaderSource(shader,count,string,length);GL_DEBUG
}

void	OpenGLFunctionsDebug::glStencilFunc(GLenum func, GLint ref, GLuint mask){
    QOpenGLFunctions::glStencilFunc(func,ref,mask);GL_DEBUG
}

void	OpenGLFunctionsDebug::glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask){
    QOpenGLFunctions::glStencilFuncSeparate(face,func,ref,mask);GL_DEBUG
}

void	OpenGLFunctionsDebug::glStencilMask(GLuint mask){
    QOpenGLFunctions::glStencilMask(mask);GL_DEBUG
}

void	OpenGLFunctionsDebug::glStencilMaskSeparate(GLenum face, GLuint mask){
    QOpenGLFunctions::glStencilMaskSeparate(face,mask);GL_DEBUG
}

void	OpenGLFunctionsDebug::glStencilOp(GLenum fail, GLenum zfail, GLenum zpass){
    QOpenGLFunctions::glStencilOp(fail,zfail,zpass);GL_DEBUG
}

void	OpenGLFunctionsDebug::glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass){
    QOpenGLFunctions::glStencilOpSeparate(face,fail,zfail,zpass);GL_DEBUG
}

void	OpenGLFunctionsDebug::glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels){
    QOpenGLFunctions::glTexImage2D(target,level,internalformat,width,height,border,format,type,pixels);GL_DEBUG
}

void	OpenGLFunctionsDebug::glTexParameterf(GLenum target, GLenum pname, GLfloat param){
    QOpenGLFunctions::glTexParameterf(target,pname,param);GL_DEBUG
}

void	OpenGLFunctionsDebug::glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params){
    QOpenGLFunctions::glTexParameterfv(target,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glTexParameteri(GLenum target, GLenum pname, GLint param){
    QOpenGLFunctions::glTexParameteri(target,pname,param);GL_DEBUG
}

void	OpenGLFunctionsDebug::glTexParameteriv(GLenum target, GLenum pname, const GLint * params){
    QOpenGLFunctions::glTexParameteriv(target,pname,params);GL_DEBUG
}

void	OpenGLFunctionsDebug::glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels){
    QOpenGLFunctions::glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform1f(GLint location, GLfloat x){
    QOpenGLFunctions::glUniform1f(location,x);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform1fv(GLint location, GLsizei count, const GLfloat * v){
    QOpenGLFunctions::glUniform1fv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform1i(GLint location, GLint x){
    QOpenGLFunctions::glUniform1i(location,x);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform1iv(GLint location, GLsizei count, const GLint * v){
    QOpenGLFunctions::glUniform1iv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform2f(GLint location, GLfloat x, GLfloat y){
    QOpenGLFunctions::glUniform2f(location,x,y);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform2fv(GLint location, GLsizei count, const GLfloat * v){
    QOpenGLFunctions::glUniform2fv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform2i(GLint location, GLint x, GLint y){
    QOpenGLFunctions::glUniform2i(location,x,y);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform2iv(GLint location, GLsizei count, const GLint * v){
    QOpenGLFunctions::glUniform2iv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z){
    QOpenGLFunctions::glUniform3f(location,x,y,z);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform3fv(GLint location, GLsizei count, const GLfloat * v){
    QOpenGLFunctions::glUniform3fv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform3i(GLint location, GLint x, GLint y, GLint z){
    QOpenGLFunctions::glUniform3i(location,x,y,z);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform3iv(GLint location, GLsizei count, const GLint * v){
    QOpenGLFunctions::glUniform3iv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
    QOpenGLFunctions::glUniform4f(location,x,y,z,w);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform4fv(GLint location, GLsizei count, const GLfloat * v){
    QOpenGLFunctions::glUniform4fv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w){
    QOpenGLFunctions::glUniform4i(location,x,y,z,w);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniform4iv(GLint location, GLsizei count, const GLint * v){
    QOpenGLFunctions::glUniform4iv(location,count,v);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value){
    QOpenGLFunctions::glUniformMatrix2fv(location,count,transpose,value);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value){
    QOpenGLFunctions::glUniformMatrix3fv(location,count,transpose,value);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value){
    QOpenGLFunctions::glUniformMatrix4fv(location,count,transpose,value);GL_DEBUG
}

void	OpenGLFunctionsDebug::glUseProgram(GLuint program){
    QOpenGLFunctions::glUseProgram(program);GL_DEBUG
}

void	OpenGLFunctionsDebug::glValidateProgram(GLuint program){
    QOpenGLFunctions::glValidateProgram(program);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib1f(GLuint indx, GLfloat x){
    QOpenGLFunctions::glVertexAttrib1f(indx,x);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib1fv(GLuint indx, const GLfloat * values){
    QOpenGLFunctions::glVertexAttrib1fv(indx,values);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y){
    QOpenGLFunctions::glVertexAttrib2f(indx,x,y);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib2fv(GLuint indx, const GLfloat * values){
    QOpenGLFunctions::glVertexAttrib2fv(indx,values);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z){
    QOpenGLFunctions::glVertexAttrib3f(indx,x,y,z);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib3fv(GLuint indx, const GLfloat * values){
    QOpenGLFunctions::glVertexAttrib3fv(indx,values);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
    QOpenGLFunctions::glVertexAttrib4f(indx,x,y,z,w);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttrib4fv(GLuint indx, const GLfloat * values){
    QOpenGLFunctions::glVertexAttrib4fv(indx,values);GL_DEBUG
}

void	OpenGLFunctionsDebug::glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * ptr){
    QOpenGLFunctions::glVertexAttribPointer(indx,size,type,normalized,stride,ptr);GL_DEBUG
}

void	OpenGLFunctionsDebug::glViewport(GLint x, GLint y, GLsizei width, GLsizei height){
    QOpenGLFunctions::glViewport(x,y,width,height);GL_DEBUG
}

bool	OpenGLFunctionsDebug::hasOpenGLFeature(QOpenGLFunctions::OpenGLFeature feature) const{
    QOpenGLFunctions::hasOpenGLFeature(feature);
}
void	OpenGLFunctionsDebug::initializeOpenGLFunctions(){
    QOpenGLFunctions::initializeOpenGLFunctions();GL_DEBUG
}

QOpenGLFunctions::OpenGLFeatures	OpenGLFunctionsDebug::openGLFeatures() const{
    return QOpenGLFunctions::openGLFeatures();
}
