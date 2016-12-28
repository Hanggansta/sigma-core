#version 330

#include <vertex.glsl>

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texcoord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;

void main()
{
    out_vertex.position = vec4(in_position,1);
    out_vertex.normal = in_normal;
    out_vertex.tangent = in_tangent;
    out_vertex.binormal = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;
    gl_Position = vec4(in_position, 1);
}
