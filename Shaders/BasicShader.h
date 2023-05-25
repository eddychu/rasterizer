#pragma once
#include "../Core/Shader.h"
struct BasicShader : public Shader
{
    BasicShader() : Shader()
    {
    }
    vec4 Vertex(const VertexAttributes &attr, int index)
    {
        vertexAttributes[index] = attr;
        return uniforms.mvp * vec4(attr.vertex, 1.0);
    }
    vec4 Fragment(const vec2 &uv)
    {
    }
};
