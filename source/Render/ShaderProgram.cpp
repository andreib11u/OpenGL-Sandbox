#include "ShaderProgram.h"
#include "Shader.h"
#include "glad/glad.h"
#include <cassert>

void ShaderProgram::Link()
{
	glLinkProgram(_id);

	int success;
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		constexpr int max_log_size = 512;
		char info_log[max_log_size];
		glGetProgramInfoLog(_id, max_log_size, nullptr, info_log);
		printf("Shader program's linking failed. Reason: %s\n", info_log);
	}
}

void ShaderProgram::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::SetColor(const std::string& name, const core::rgb& color)
{
	glUniform3f(glGetUniformLocation(_id, name.c_str()), color.red, color.green, color.blue);
}

void ShaderProgram::SetColor(const std::string& name, const core::rgba& color)
{
	glUniform4f(glGetUniformLocation(_id, name.c_str()), color.red, color.green, color.blue, color.alpha);
}

ShaderProgram::ShaderProgram() = default;

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

ShaderProgram::ShaderProgram(const ShaderProgram& other)
	: _attached_shaders(other._attached_shaders)
{
	_id = glCreateProgram();

	for (const uint32_t shader : _attached_shaders)
	{
		glAttachShader(_id, shader);
	}
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other)
{
	if (this == &other)
		return *this;

	_id = glCreateProgram();
	_attached_shaders = other._attached_shaders;
	for (const uint32_t shader : _attached_shaders)
	{
		glAttachShader(_id, shader);
	}

	return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: _id(other._id)
	, _attached_shaders(std::move(other._attached_shaders))
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this == &other)
		return *this;

	_id = other._id;
	_attached_shaders = std::move(other._attached_shaders);

	return *this;
}

void ShaderProgram::Init()
{
	_id = glCreateProgram();
}

void ShaderProgram::AttachShader(const Shader& shader)
{
	assert(shader.IsValid());
	glAttachShader(_id, shader.GetId());
	_attached_shaders.push_back(shader.GetId());
}

void ShaderProgram::DetachShader(const Shader& shader)
{
	glDetachShader(_id, shader.GetId());
	std::erase_if(_attached_shaders, [&shader](uint32_t shader_id) { return shader_id == shader.GetId(); });
}

void ShaderProgram::Use()
{
	glUseProgram(_id);
}
