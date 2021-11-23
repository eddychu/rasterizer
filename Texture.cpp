#include "Texture.h"
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
Texture::Texture()
	:width(0)
	,height(0)
{
}

Texture::~Texture() {
	
}

void Texture::Load(const char* file, Type type)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* rawData = stbi_load(file, &width, &height, &channels, 0);
	data.resize(width * height * channels);
	for (int i = 0; i < width * height * channels; i++) {
		data[i] = (float)rawData[i] * (1 / 255.0f);
		if(type == Type::NORMAL) {
			data[i] = (data[i] - 0.5f) * 2.0f;
		} else if(type == Type::DIFFUSE) {
			data[i] = std::pow(data[i], 2.2f);
		} else if (type == Type::SPECULAR) {
			
		}
		else {
			throw std::runtime_error("Unknown texture type");
		}
	}
	stbi_image_free(rawData);
}

float Texture::GetFloat(const vec2& uv) {
	int x = (int)(uv.x * width);
	int y = (int)(uv.y * height);
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return 0.0f;
	}
	return data[(y * width + x) * channels];
}

vec3 Texture::GetVec3(const vec2& uv)
{
	int x = uv.x * width;
	int y = uv.y * height;
	int index = (y * width + x) * channels;
	return vec3(data[index], data[index + 1], data[index + 2]);
}