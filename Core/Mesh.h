#pragma once
#include <glm/glm.hpp>
using namespace glm;
#include <vector>
#include "Texture.h"
class Mesh
{
public:
	Mesh(const char* filepath, const char* name);

	~Mesh();

	vec3 vertice(const int iface, const int nthvert) const {
		return m_vertices[m_vertexIndices[iface * 3 + nthvert]];
	}

	vec2 uv(const int iface, const int nthvert) const {
		return m_texels[m_textureIndices[iface * 3 + nthvert]];
	}

	vec3 normal(const int iface, const int nthvert) const {
		return m_normals[m_normalsIndices[iface * 3 + nthvert]];
	}

	vec3 faceNormal(const int iface) {
		return m_faceNormals[iface];
	}

	vec3 tangent(const int iface) {
		return m_tangents[iface];
	}

	vec3 bitangent(const int iface) {
		return m_biTangents[iface];
	}

	int GetFaceNumbers() {
		return m_numFaces;
	}

	Texture* GetDiffuse() {
		return &diffuseTexture;
	}

	Texture* GetSpecular() {
		return &specularTexture;
	}

	Texture* GetNormal() {
		return &normalTexture;
	}

	Texture* GetTangent() {
		return &tangentTexture;
	}

private:
	std::vector<vec3> m_vertices;
	std::vector<vec3> m_normals;
	std::vector<vec2> m_texels;
	std::vector<vec3> m_tangents;
	std::vector<vec3> m_biTangents;


	// per face values;
	int m_numFaces = 0;
	// std::vector<Vec3> fNormals; //Normals for the whole face
	std::vector<int> m_vertexIndices;
	std::vector<int> m_textureIndices;
	std::vector<int> m_normalsIndices;
	std::vector<vec3> m_faceNormals;

	Texture diffuseTexture;
	Texture specularTexture;
	Texture tangentTexture;
	Texture normalTexture;
};

