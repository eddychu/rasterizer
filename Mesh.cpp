#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
static void ComputeTangent(vec3* pos, vec2* uv, vec3& tangent, vec3& biTangent)
{
	glm::vec3 edge1 = pos[1] - pos[0];
	glm::vec3 edge2 = pos[2] - pos[0];
	glm::vec2 deltaUV1 = uv[1] - uv[0];
	glm::vec2 deltaUV2 = uv[2] - uv[0];
	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

}


Mesh::Mesh(const char* filepath, const char* name)
{
	std::ifstream in;
	std::string objFile = std::string(filepath) + "/" + std::string(name) + ".obj";
	in.open(objFile, std::ifstream::in);

	if (in.fail()) throw std::runtime_error("couldn't open file.\n");

	std::string line;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			float x, y, z;
			iss >> x >> y >> z;
			m_vertices.push_back(vec3(x, y, z));
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			float x, y, z;
			iss >> x >> y >> z;
			m_normals.push_back(vec3(x, y, z));
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 uv;
			float x, y;
			iss >> x >> y;
			m_texels.push_back(vec2(x, y));
		}
		else if (!line.compare(0, 2, "f ")) {
			int f, t, n;
			iss >> trash;
			int cnt = 0;
			while (iss >> f >> trash >> t >> trash >> n) {
				m_vertexIndices.push_back(--f);
				m_textureIndices.push_back(--t);
				m_normalsIndices.push_back(--n);
				cnt++;
			}

			if (3 != cnt) {
				std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
				in.close();
				return;
			}

		}
	}
	std::cout << "indices " << m_vertexIndices.size() / 3 << std::endl;
	m_numFaces = m_vertexIndices.size() / 3;
	for (int i = 0; i < m_numFaces; i++) {
		vec3 v0 = vertice(i, 0);
		vec3 v1 = vertice(i, 1);
		vec3 v2 = vertice(i, 2);

		m_faceNormals.push_back(normalize(cross(v1 - v0, v2 - v0)));
		
		vec2 uv0 = uv(i, 0);
		vec2 uv1 = uv(i, 1);
		vec2 uv2 = uv(i, 2);

		vec3 pos[3] = { v0, v1, v2 };
		vec2 uvPos[3] = { uv0, uv1, uv2 };

		vec3 tangent, biTangent;
		ComputeTangent(pos, uvPos, tangent, biTangent);
		m_tangents.push_back(tangent);
		m_biTangents.push_back(biTangent);
	}

	std::cout << m_vertices.size() << "vertices " << m_texels.size() << "uvs " << m_normals.size() << "normals" << std::endl;

	std::string diffusefile = std::string(filepath) + "/" + std::string(name) + "_diffuse.tga";
	diffuseTexture.Load(diffusefile.c_str(), Texture::Type::DIFFUSE);

	// std::string specularfile = std::string(filepath) + "/" + std::string(name) + "_spec.tga";
	std::string specularfile = std::string(filepath) + "/" + std::string(name) + "_specular.tga";
	specularTexture.Load(specularfile.c_str(), Texture::Type::SPECULAR);

	// std::string normtfile = std::string(filepath) + "/" + std::string(name) + "_nm_tangent.tga";
	std::string normtfile = std::string(filepath) + "/" + std::string(name) + "_normal.tga";
	normalTexture.Load(normtfile.c_str(), Texture::Type::NORMAL);

	// std::string normfile = std::string(filepath) + "/" + std::string(name) + "_nm.tga";
	// normalTexture.Load(normfile.c_str());
}

Mesh::~Mesh()
{
}