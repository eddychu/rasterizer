#pragma once

#include <glm/glm.hpp>
using namespace glm;
#include <algorithm>
#include <iostream>
#include "Texture.h"
#include <glm/gtx/string_cast.hpp>

struct VertexAttributes
{
	VertexAttributes() {

	}
	VertexAttributes(vec3 position, vec3 normal, vec2 texCoord, vec3 tangent, vec3 bitangent)
	{
		this->vertex = position;
		this->normal = normal;
		this->texCoord = texCoord;
		this->tangent = tangent;
		this->bitangent = bitangent;
	}

	vec3 vertex;
	vec3 normal;
	vec2 texCoord;
	vec3 tangent;
	vec3 bitangent;
};

struct VertexOut {
	vec3 lightDir;
	vec3 viewDir;
};

struct ShaderUniforms
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 mv;
	mat4 mvp;
	mat3 normalMatrix;
	vec3 lightPos;
};

struct ShaderSamplers
{
	Texture* diffuse;
	Texture* specular;
	Texture* normal;
};

static vec3 interp(vec3 *vs, vec2 uv) {
	vec3 p = vs[0] * (1 - uv.x - uv.y) + vs[1] * uv.x + vs[2] * uv.y;
	return p;
}

static vec2 interp(vec2 *vs, vec2 uv) {
	vec2 p = vs[0] * (1 - uv.x - uv.y) + vs[1] * uv.x + vs[2] * uv.y;
	return p;
}


struct Shader {

	Shader() {

	}
	VertexAttributes vertexAttributes[3];
	ShaderUniforms uniforms;
	ShaderSamplers samplers;
	virtual vec4 Vertex(const VertexAttributes& attr, int index) {
		return vec4(0, 0, 0, 0);
	};
	virtual vec4 Fragment(const vec2& uv) {
		return vec4(0, 0, 0, 1);
	}
};

struct BlinnPhongShader : public Shader {

	VertexOut vsOut[3];
	BlinnPhongShader() : Shader() {

	}

	vec4 Vertex(const VertexAttributes& attr, int index) override {
		vertexAttributes[index] = attr;
		vec3 norm = normalize(uniforms.normalMatrix * attr.normal);
		vec3 tang = normalize(uniforms.normalMatrix * attr.tangent);
		vec3 bitan = normalize(uniforms.normalMatrix * attr.bitangent);
		tang = normalize(tang - norm * dot(tang, norm));


		mat3 TBN = mat3(tang, bitan, norm);
		vec3 pos = vec3(uniforms.mv * vec4(attr.vertex, 1.0f));
		vsOut[index].lightDir = normalize(TBN * (uniforms.lightPos - pos));
		vsOut[index].viewDir = normalize(TBN * (-pos));
		return uniforms.mvp * vec4(attr.vertex, 1.0f);
	}

	vec4 Fragment(const vec2& uv) override {
		vec2 texCoords[3];
		texCoords[0] = vertexAttributes[0].texCoord;
		texCoords[1] = vertexAttributes[1].texCoord;
		texCoords[2] = vertexAttributes[2].texCoord;

		vec3 viewDirs[3];
		viewDirs[0] = vsOut[0].viewDir;
		viewDirs[1] = vsOut[1].viewDir;
		viewDirs[2] = vsOut[2].viewDir;

		vec3 lightDirs[3];
		lightDirs[0] = vsOut[0].lightDir;
		lightDirs[1] = vsOut[1].lightDir;
		lightDirs[2] = vsOut[2].lightDir;

		vec3 viewDir = normalize(interp(viewDirs, uv));
		vec3 lightDir = normalize(interp(lightDirs, uv));



		vec2 texCoord = interp(texCoords, uv);
		vec3 norm = normalize(samplers.normal->GetVec3(texCoord));
		vec3 diffuse = samplers.diffuse->GetVec3(texCoord);
		float specular = 0;
		float sDotN = max(dot(lightDir, norm), 0.0f);
		if (sDotN > 0.0f) {
			vec3 v = viewDir;
			vec3 h = normalize(v + lightDir);
			float spec = samplers.specular->GetFloat(texCoord);
			float specular = std::pow(max(dot(h,norm), 0.0f), specular);
		}
		return vec4(diffuse * ( sDotN + specular), 1.0f);
	}


	
};



// struct Shader 
// {
// 	mat4 MVP;
// 	mat4 MV;
// 	mat3 N;
// 	Texture* diffuseTexture;
// 	Texture* specTexture;
// 	Texture* normalTexture;
// 	Texture* tangentTexture;
// 	float varIntensity = 0;
// 	vec3 lightPos;
// 	vec2 varyingUVs[3];
// 	vec3 varyingPos[3];
// 	vec3 varyingNormal[3];

// 	vec4 Vertex(const vec3& vertex, const vec3& normal, const vec2& uv, int index)
// 	{
// 		varyingUVs[index] = uv;
// 		varyingPos[index] = vec3(MV * vec4(vertex, 1.0));
// 		// varyingNormal[index] = normalize(N * normal);
// 		return MVP * vec4(vertex, 1.0);
// 	}

// 	vec4 Fragment(float a, float b, float c)
// 	{
// 		vec2 interUV = a * varyingUVs[0] + b * varyingUVs[1] + c * varyingUVs[2];
// 		vec3 interPos = a * varyingPos[0] + b * varyingPos[1] + c * varyingPos[2];
// 		// vec3 interNorm = a * varyingNormal[0] + b * varyingNormal[1] + c * varyingNormal[2];	
// 		vec3 interNorm = normalize(N * normalTexture->GetColor(interUV));
// 		vec3 interTangent = normalize(N * tangentTexture->GetColor(interUV));
// 		vec3 binormal = normalize(cross(interNorm, interTangent)) * tangentTexture->GetColor(interUV).w;
// 		mat3 toObject = mat3(
// 			interTangent.x, binormal.x, interNorm.x,
// 			interTangent.y, binormal.y, interNorm.y,
// 			interTangent.z, binormal.z, interNorm.z);
// 		vec3 s = toObject * normalize(lightPos - interPos);
// 		float sDotN = max(dot(s, interNorm), 0.0f);
// 		vec4 diffuse = diffuseTexture->GetColor(interUV) * sDotN;
// 		vec4 spec = specTexture->GetColor(interUV);
// 		if (sDotN > 0.0f) {
// 			vec3 v = toObject * normalize(-interPos);
// 			vec3 r = glm::reflect(-s, interNorm);
// 			float temp = std::pow(max(dot(r, v), 0.0f), 64);
// 			spec = spec * temp;
// 		}
// 		return diffuse + spec;
// 	}
// };