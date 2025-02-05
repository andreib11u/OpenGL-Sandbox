#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

enum class ShaderType : uint32_t
{
	Vertex = 0x8B31,
	Fragment = 0x8B30
};

class Shader
{
public:
	~Shader();
	Shader(const std::string& source, ShaderType shader_type);
	Shader(const std::filesystem::path& path, ShaderType shader_type);
	Shader(const Shader& other);
	Shader& operator=(const Shader& other);
	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void Compile();
	unsigned GetId() const { return _id; }
	ShaderType GetShaderType() const { return _shader_type; }
	bool IsValid() const;

private:
	std::string _code;
	ShaderType _shader_type;
	uint32_t _id = 0;
};
