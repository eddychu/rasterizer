#pragma once
#include <algorithm>
struct DepthBuffer
{

	DepthBuffer(int w, int h)
		:width(w)
		, height(h)
	{
		depths = new float[width * height]();
		Clear();
	}
	~DepthBuffer() {
		delete[] depths;
	}
	float GetDepth(int x, int y) {
		int index = y * width + x;
		return depths[index];
	}
	void SetDepth(int x, int y, float depth) {
		int index = y * width + x;
		depths[index] = depth;
	}
	void Clear() {
		std::fill(depths, depths + width * height, 1.0);
	}

	int width;
	int height;
	float* depths;
};

