#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <GL/glew.h>
#include <GL/gl.h>

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct shader
{
    GLuint program;
    vector<GLuint> shaders;

    map<string,GLuint> attribute_list;
    map<string,GLuint> uniform_list;

    shader()
    {

    }
    ~shader()
    {
        glDeleteProgram(program);
    }

    void load_string(GLenum type,const string & source)
    {
        GLuint shader = glCreateShader(type);

        const char * p_tmp = source.c_str();
        glShaderSource(shader,1,&p_tmp,nullptr);

        GLint status;
        glCompileShader(shader);
        glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
        if(status == GL_FALSE)
        {
            GLint il_len;
            glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&il_len);

            GLchar * il = new GLchar[il_len];
            glGetShaderInfoLog(shader,il_len,NULL,il);
            cerr<<"Compile Log: "<<il<<endl;

            delete [] il;
        }
        shaders.push_back(shader);
    }
    void load_file(GLenum type,const string & filename)
    {
        ifstream fp;
        fp.open(filename.c_str());
        if(fp)
        {
            string buffer(std::istreambuf_iterator<char>(fp),(std::istreambuf_iterator<char>()));
            load_string(type,buffer);
        }
        else
            cerr<<"Error: "<<filename<<endl;
    }

    void create()
    {
        program = glCreateProgram();

        for(auto & s : shaders)
            if(s != 0) glAttachShader(program,s);

        GLint status;
        glLinkProgram(program);
        glGetProgramiv(program,GL_LINK_STATUS,&status);
        if(status == GL_FALSE)
        {
            GLint il_len;
            glGetProgramiv(program,GL_INFO_LOG_LENGTH,&il_len);

            GLchar * il = new GLchar[il_len];
            glGetProgramInfoLog(program,il_len,NULL,il);
            cerr<<"Link Log: "<<il<<endl;

            delete [] il;
        }

        for(auto & s : shaders)
            glDeleteShader(s);
        shaders.clear();
    }

    GLuint operator [] (const string & attrib)
    {
        if(attribute_list.count(attrib)==0)
            attribute_list[attrib] = glGetAttribLocation(program,attrib.c_str());
        return attribute_list[attrib];
    }

    GLuint operator () (const string & unif)
    {
        if(uniform_list.count(unif)==0)
            uniform_list[unif] = glGetUniformLocation(program,unif.c_str());
        return uniform_list[unif];
    }

    void begin(){ glUseProgram(program);}
    void end(){ glUseProgram(0);}
};



#endif // SHADER_HPP_INCLUDED
