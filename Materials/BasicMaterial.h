#pragma once
#include "../Core/Material.h"
#include "../Core/Texture.h"
#include <memory>
struct BasicMaterial : public Material
{
    BasicMaterial() : color(1.0f, 1.0f, 1.0f) {

    }

    BasicMaterial(std::shared_ptr<Texture> map) : colorMap(map) {

    }
    vec3 color;
    std::shared_ptr<Texture> colorMap;
};



