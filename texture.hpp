#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED


#include <GL/glew.h>
#include <GL/gl.h>

template<GLuint type_>
struct tex_
{
    GLuint  idx_;
     tex_(){ glGenTextures(1,&idx_);}
    ~tex_(){ if(idx_) glDeleteTextures(1,&idx_);}
    void bind_()
    {
        glBindTexture(type_,idx_);
    }
    void unbind_()
    {
        glBindTexture(type_,   0);
    }
};

#endif // TEXTURE_HPP_INCLUDED
