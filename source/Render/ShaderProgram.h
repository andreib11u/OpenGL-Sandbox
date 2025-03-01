#pragma once

#include "core_types.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Shader;
enum class ShaderType : uint32_t;

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	ShaderProgram(const ShaderProgram& other);
	ShaderProgram& operator=(const ShaderProgram& other);
	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	void Init();
	void AttachShader(const Shader& shader);
	void DetachShader(const Shader& shader);
	void Use();
	void Link();

	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetColor(const std::string& name, const core::rgb& color);
	void SetColor(const std::string& name, const core::rgba& color);

private:
	uint32_t _id;
	std::vector<uint32_t> _attached_shaders;
};
