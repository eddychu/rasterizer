#pragma once
#include <glm/glm.hpp>
using namespace glm;
#include <vector>

class Texture {
public:
	enum class Type {
		DIFFUSE,
		SPECULAR,
		NORMAL,
	};
	Texture();
	~Texture();
	void Load(const char* filename, Type type);
	vec3 GetVec3(const vec2& uv);
	float GetFloat(const vec2& uv);

	std::vector<float> data;
	int width;
	int height;
	int channels;
};