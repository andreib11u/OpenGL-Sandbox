#pragma once

namespace core
{
	struct rect
	{
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};

	struct rgb
	{
		float red = 0.f;
		float green = 0.f;
		float blue = 0.f;
	};

	struct rgba
	{
		float red = 0.f;
		float green = 0.f;
		float blue = 0.f;
		float alpha = 1.f;
	};

	struct point
	{
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	};

	
}
