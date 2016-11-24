#include <GL/glew.h>
#include <GL/gl.h>

#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <cmath>

#include "api/inc/fmod.h"
#include "api/inc/fmod_codec.h"
#include "api/inc/fmod_dsp.h"
#include "api/inc/fmod_memoryinfo.h"

using namespace std;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int win_x = 1340;
int win_y = 640;

GLFWwindow * window = nullptr;
const int OUTPUT_RATE = 44100;
const int SPEC_SIZE   =  1024;
struct sound_sys
{
    FMOD_SYSTEM  * fmod_sys;
    FMOD_SOUND   * music;
    FMOD_CHANNEL * channel;
    sound_sys(const char * song_name)
    {
        FMOD_System_Create(&fmod_sys);
        FMOD_System_SetSoftwareFormat(fmod_sys, OUTPUT_RATE, FMOD_SOUND_FORMAT_PCM32, 2, 0, FMOD_DSP_RESAMPLER_LINEAR);
        FMOD_System_Init(fmod_sys,32,FMOD_INIT_NORMAL,nullptr);

        FMOD_System_CreateStream(fmod_sys,song_name,FMOD_LOOP_NORMAL | FMOD_2D | FMOD_UNIQUE,nullptr,&music);
    }
    ~sound_sys()
    {
        FMOD_Sound_Release(music);
        FMOD_System_Close(fmod_sys);
        FMOD_System_Release(fmod_sys);
    }
    void play_music()
    {
        FMOD_System_PlaySound(fmod_sys,FMOD_CHANNEL_FREE,music,false,&channel);
    }
    void get_spectrum(float *spectrumL,float *spectrumR)
    {
        FMOD_Channel_GetSpectrum(channel,spectrumL,SPEC_SIZE,0,FMOD_DSP_FFT_WINDOW_HAMMING);
        FMOD_Channel_GetSpectrum(channel,spectrumR,SPEC_SIZE,1,FMOD_DSP_FFT_WINDOW_HAMMING);
    }
    void update()
    {
        FMOD_System_Update(fmod_sys);
    }
};

int init_gl_window()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(win_x,win_y,"Spectrogram",nullptr,nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    return 0;
}


int main(int argc,char ** argv)
{
    srand(time(nullptr));
    if(init_gl_window()<0) return -1;

    screen  screen_;
    shader  show_;

    show_.load_file(GL_VERTEX_SHADER  ,"vert.glsl");
    show_.load_file(GL_FRAGMENT_SHADER,"frag.glsl");
    show_.create();


    double t = 0.;

    tex_<GL_TEXTURE_2D> S;
    float sL[SPEC_SIZE],sR[SPEC_SIZE];

    sound_sys SYS(argc>1?argv[1]:"Comes_A_Time.flac");

    SYS.play_music();

    int idx_t = 0;
    int M = 1000;
    float s[2*M*SPEC_SIZE];
    float s_min = 1e10;
    float s_max =-1e10;
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &win_x, &win_y);

        SYS.update();
        SYS.get_spectrum(sL,sR);
        for(int i=0;i<SPEC_SIZE;i++)
        {
            s[M*i            +idx_t] = std::isnormal(sL[SPEC_SIZE-i-1])?sL[SPEC_SIZE-i-1]:0.0;
            s[M*(SPEC_SIZE+i)+idx_t] = std::isnormal(sR[i])?sR[i]:0.0;
            s_min = min(s_min,min(s[M*i+idx_t],s[M*(SPEC_SIZE+i)+idx_t]));
            s_max = max(s_max,max(s[M*i+idx_t],s[M*(SPEC_SIZE+i)+idx_t]));
        }
        for(int i=0;i<2*SPEC_SIZE;i++)
        {
            s[M*i+idx_t] = (log(fabs(s[M*i+idx_t])+1e-10)+10.0)/40.0;
        }
        
        S.bind_();
            glTexImage2D(GL_TEXTURE_2D,0,GL_RED,M,2*SPEC_SIZE,0,GL_RED,GL_FLOAT,s);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        S.unbind_();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        show_.begin();
        glActiveTexture(GL_TEXTURE0);
        S.bind_();
        glUniform1i(show_("S"),0);

        screen_.draw();

        show_.end();
        S.unbind_();
        
        t += 1.0/60.0;
        idx_t = (idx_t+1)%M;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
