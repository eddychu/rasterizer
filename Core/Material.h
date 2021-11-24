#pragma once
#include "Shader.h"
struct Material
{
    virtual Shader *GetShader() = 0;
};
