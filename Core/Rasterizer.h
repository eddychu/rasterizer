#pragma once
#include "Bound.h"
#include "PixelBuffer.h"
#include "DepthBuffer.h"
class Shader;
class Rasterizer
{
public:
	static Bound2 GetBound(PixelBuffer& pixelBuffer, vec4* vertices);

	static vec3 Barycentric(vec4* pts, vec2 P);
	static void DrawPoint(PixelBuffer& pixelBuffer, int x, int y, vec3 color);
	static bool Clipping(vec4* coords);
	static void PerspectiveDivide(vec4* coords);
	static bool FaceCulling(vec4* coords);
	static void Rasterize(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer, Shader* shader, vec4* vertices);
	static void WriteToPPM(PixelBuffer& pixelBuffer);
	static void ViewPortTransform(PixelBuffer& pixelBuffer, vec4* vertices);
};
