#include "Rasterizer.h"
#include "Shader.h"
#include <omp.h>
Bound2 Rasterizer::GetBound(PixelBuffer& pixelBuffer, vec4* vertices)
{
	float xMax = std::max({ vertices[0].x, vertices[1].x, vertices[2].x });
	float yMax = std::max({ vertices[0].y, vertices[1].y, vertices[2].y });

	float xMin = std::min({ vertices[0].x, vertices[1].x, vertices[2].x });
	float yMin = std::min({ vertices[0].y, vertices[1].y, vertices[2].y });

	xMax = std::min(xMax, (float)pixelBuffer.GetWidth() - 1);
	yMax = std::min(yMax, (float)pixelBuffer.GetHeight() - 1);

	xMin = std::max(xMin, 0.f);
	yMin = std::max(yMin, 0.f);

	return Bound2(xMax, yMax, xMin, yMin);
}


vec3 Rasterizer::Barycentric(vec4* pts, vec2 P)
{
	vec2 v0 = vec2(pts[1] - pts[0]);
	vec2 v1 = vec2(pts[2] - pts[0]);
	vec2 v2 = vec2(P.x - pts[0].x, P.y - pts[0].y);
	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;
	return vec3(u, v, w);
}

void Rasterizer::DrawPoint(PixelBuffer& pixelBuffer, int x, int y, vec3 color)
{
	
	unsigned char r = std::pow(color.x, 1 / 2.0) * 255.0;
	unsigned char g = std::pow(color.y, 1 / 2.0) * 255.0;
	unsigned char b = std::pow(color.z, 1 / 2.0) * 255.0;

	pixelBuffer.SetColor(x, y, r, g, b);
}

bool Rasterizer::Clipping(vec4* coords)
{
	int count = 0;
	for (int i = 0; i < 3; ++i) {
		vec4 vertex = coords[i];
		bool inside = (-vertex.w <= vertex.x <= vertex.w)
			&& (-vertex.w <= vertex.y <= vertex.w)
			&& (0 <= vertex.z <= vertex.w);
		if (!inside) ++count;
	}
	return count == 3;
}

void Rasterizer::PerspectiveDivide(vec4* coords)
{
	for (int j = 0; j < 3; j++) {
		coords[j].x /= coords[j].w;
		coords[j].y /= coords[j].w;
		coords[j].z /= coords[j].w;
	}
}

bool Rasterizer::FaceCulling(vec4* coords)
{
	auto Xa = coords[0].x;
	auto Xb = coords[1].x;
	auto Xc = coords[2].x;

	auto Ya = coords[0].y;
	auto Yb = coords[1].y;
	auto Yc = coords[2].y;

	return Xa * (Yb - Yc) + Xb * (Yc - Ya) + Xc * (Ya - Yb) <= 0;
}

void Rasterizer::Rasterize(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer, Shader* shader, vec4* vertices)
{
	Bound2 bbox = GetBound(pixelBuffer, vertices);
	#pragma omp parallel for
	for (int x = (int)bbox.min.x; x <= (int)bbox.max.x; x++) {
		for (int y = (int)bbox.min.y; y <= (int)bbox.max.y; y++) {
			vec2 P(x + 0.5, y + 0.5);
			vec3 bc = Barycentric(vertices, P);
			vec3 bc_clip = vec3(bc.x / vertices[0].w, bc.y / vertices[1].w, bc.z / vertices[2].w);
			bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
			float z = bc_clip.x * vertices[0].z + bc_clip.y * vertices[1].z + bc_clip.z * vertices[2].z;

			if (depthBuffer.GetDepth(x, y) > z) {
				depthBuffer.SetDepth(x, y, z);
				vec4 color = shader->Fragment(vec2(bc_clip.y, bc_clip.z));
				DrawPoint(pixelBuffer, P.x, pixelBuffer.GetHeight() - P.y, color);
			}

		}
	}
}

void Rasterizer::WriteToPPM(PixelBuffer& pixelBuffer)
{
}

void Rasterizer::ViewPortTransform(PixelBuffer& pixelBuffer, vec4* vertices)
{
	for (int i = 0; i < 3; ++i) {
		vertices[i].x = ((vertices[i].x + 1) * pixelBuffer.GetWidth() * 0.5) + 0.5;
		vertices[i].y = ((vertices[i].y + 1) * pixelBuffer.GetHeight() * 0.5) + 0.5;
	}
}