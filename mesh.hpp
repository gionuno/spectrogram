#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct screen
{
    GLuint i_vbo;
    GLuint x_vbo;
    GLuint t_vbo;
    screen()
    {
        vector<glm::vec3> screen_x =
        {
            glm::vec3(-1., 1., 0.),
            glm::vec3(-1.,-1., 0.),
            glm::vec3( 1.,-1., 0.),
            glm::vec3( 1., 1., 0.)
        };

        vector<glm::vec2> screen_t =
        {
            glm::vec2( 0., 1.),
            glm::vec2( 0., 0.),
            glm::vec2( 1., 0.),
            glm::vec2( 1., 1.)
        };

        vector<int> screen_i = { 0 , 1 , 2 , 0 , 2 , 3 };

        glGenBuffers(1,&x_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,x_vbo);
        glEnableVertexAttribArray(0);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*screen_x.size(),&screen_x[0],GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

        glGenBuffers(1,&t_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,t_vbo);
        glEnableVertexAttribArray(1);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)*screen_t.size(),&screen_t[0],GL_STATIC_DRAW);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

        glGenBuffers(1,&i_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,i_vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*screen_i.size(),&screen_i[0],GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }
    ~screen()
    {
        glDeleteBuffers(1,&i_vbo);
        glDeleteBuffers(1,&x_vbo);
        glDeleteBuffers(1,&t_vbo);
    }
    void draw()
    {
        glBindBuffer(GL_ARRAY_BUFFER,x_vbo);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER,t_vbo);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,i_vbo);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }
};

#endif // MESH_HPP_INCLUDED
