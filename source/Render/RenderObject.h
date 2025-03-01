#pragma once

#include "core_types.h"
#include <cstdint>
#include <vector>

enum class DrawType
{
	Static = 0x88E4
};

class RenderObject
{
public:
	RenderObject() = default;
	RenderObject(const std::vector<core::point>& vertices, const std::vector<uint32_t>& indices,
				 DrawType draw_type = DrawType::Static);
	virtual ~RenderObject();
	RenderObject(const RenderObject& other);
	RenderObject& operator=(const RenderObject& other);
	RenderObject(RenderObject&& other) noexcept;
	RenderObject& operator=(RenderObject&& other) noexcept;

	virtual void Render();

private:
	void StoreVerticesOnGPU();

	std::vector<core::point> _vertices;
	std::vector<uint32_t> _indices;
	uint32_t _vao{};
	uint32_t _vbo{};
	uint32_t _ebo{};
	DrawType _draw_type{DrawType::Static};
};
