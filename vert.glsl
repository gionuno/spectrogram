#version 130

in vec3 v_x;
in vec2 v_t;

out vec3 f_x;
out vec2 f_t;

void main()
{
	f_x = v_x;
	f_t = v_t;
	gl_Position = vec4(f_x,1.);
}
