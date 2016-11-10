#version 130

uniform sampler2D S;

const float eps = 1e-8;
const float del = 1e-4;
const float inf = 1e10;
const float pi  = 3.14159265359;

in vec3 f_x;
in vec2 f_t;

out vec4 o_c;

void main()
{
    float a = texture2D(S,f_t).r;
    o_c.rgb = (exp(2.0*a)-1.0)*vec3(0.0,1.0,0.0);
    o_c.a =1.0;
}

