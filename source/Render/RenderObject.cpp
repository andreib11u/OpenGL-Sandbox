#include "RenderObject.h"
#include "glad/glad.h"

void RenderObject::StoreVerticesOnGPU()
{
	// todo: extract binding to another funciton

	// bind
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// copy data to gpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(core::point) * _vertices.size(), _vertices.data(),
				 static_cast<uint32_t>(_draw_type));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int32_t) * _indices.size(), _indices.data(),
				 static_cast<uint32_t>(_draw_type));

	// instruct how to read that data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

RenderObject::RenderObject(const std::vector<core::point>& vertices, const std::vector<uint32_t>& indices,
						   DrawType draw_type)
	: _vertices(vertices)
	, _indices(indices)
	, _draw_type(draw_type)
{
	StoreVerticesOnGPU();
}

RenderObject::~RenderObject()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

RenderObject::RenderObject(const RenderObject& other)
	: _vertices(other._vertices)
	, _indices(other._indices)
	, _draw_type(other._draw_type)
{
	StoreVerticesOnGPU();
}

RenderObject& RenderObject::operator=(const RenderObject& other)
{
	if (&other == this)
		return *this;

	_vertices = other._vertices;
	_indices = other._indices;
	_draw_type = other._draw_type;

	StoreVerticesOnGPU();

	return *this;
}

RenderObject::RenderObject(RenderObject&& other) noexcept
	: _vertices(std::move(other._vertices))
	, _indices(std::move(other._indices))
	, _vao(other._vao)
	, _vbo(other._vbo)
	, _ebo(other._ebo)
	, _draw_type(other._draw_type)
{
	other._vao = 0;
	other._vbo = 0;
	other._ebo = 0;
}

RenderObject& RenderObject::operator=(RenderObject&& other) noexcept
{
	if (&other == this)
		return *this;

	_vertices = std::move(other._vertices);
	_indices = std::move(other._indices);
	_vao = other._vao;
	_vbo = other._vbo;
	_ebo = other._ebo;
	_draw_type = other._draw_type;

	other._vao = 0;
	other._vbo = 0;
	other._ebo = 0;

	return *this;
}

void RenderObject::Render()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}
