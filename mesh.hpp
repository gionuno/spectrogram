#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <GL/glew.h>
#include <GL/gl.h>

#include <armadillo>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <string>

using namespace arma;
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
/*
struct mesh
{
    GLuint i_vbo;
    GLuint x_vbo;
    GLuint n_vbo;

    mesh(const string & filename)
    {
        vector<int> e;

        vector<vec3> x;
        vector<vec3> n;

        vector<vector<int> > f_x;
        vector<vector<int> > f_n;

        ifstream file(filename.c_str());
        string s;
        while(getline(file,s))
        {
            if(s.size()<3)
                continue;
            stringstream ss(s.substr(2));
            if(s.substr(0,2) == "v ")
            {
                vec3 aux;
                ss >> aux.x >> aux.y >> aux.z;
                x.push_back(aux);
            }
            else if(s.substr(0,2) == "vn")
            {
                vec3 aux;
                ss >> aux.x >> aux.y >> aux.z;
                n.push_back(aux);
            }
            else if(s.substr(0,2) == "f ")
            {
                int a_x,b_x,c_x;
                int a_n,b_n,c_n;

                char aux;

                ss >> a_x >> aux >> aux >> a_n;
                ss >> b_x >> aux >> aux >> b_n;
                ss >> c_x >> aux >> aux >> c_n;

                f_x.push_back(vector<int>(3));
                f_x[f_x.size()-1][0] = a_x-1;
                f_x[f_x.size()-1][1] = b_x-1;
                f_x[f_x.size()-1][2] = c_x-1;

                f_n.push_back(vector<int>(3));
                f_n[f_n.size()-1][0] = a_n-1;
                f_n[f_n.size()-1][1] = b_n-1;
                f_n[f_n.size()-1][2] = c_n-1;
            }
        }

        vector<GLuint> e;
        for(auto & )
    }

    ~mesh()
    {
        glDeleteBuffers(1,&i_vbo);
        glDeleteBuffers(1,&x_vbo);
        glDeleteBuffers(1,&n_vbo);
    }
};
*/
#endif // MESH_HPP_INCLUDED
