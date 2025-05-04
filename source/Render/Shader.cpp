#include "Shader.h"
#include "App/Log/Log.h"
#include "fmt/xchar.h"
#include "glad/glad.h"
#include <fstream>
#include <sstream>

DEFINE_LOGGER(shader)

void Shader::Compile()
{
	glCompileShader(_id);

	int success;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info_log[512];
		// todo: add a type of shader to log
		glGetShaderInfoLog(_id, 512, nullptr, info_log);
		printf("Error: shader compilation failed. Reason: %s", info_log);
		glDeleteShader(_id);
		_id = 0;
	}
}

bool Shader::IsValid() const
{
	return _id != 0;
}

Shader::~Shader()
{
	glDeleteShader(_id);
}

Shader::Shader(const std::string& source, ShaderType shader_type)
	: _shader_type(shader_type)
{
	_id = glCreateShader(static_cast<uint32_t>(_shader_type));
	if (_id == 0)
	{
		printf("Error: couldn't create shader\n");
		return;
	}
	_code = source;

	const char* c_str = _code.c_str();
	glShaderSource(_id, 1, &c_str, nullptr);

	Compile();

	_code.resize(0);
}

Shader::Shader(const Shader& other)
	: _code(other._code)
	, _shader_type(other._shader_type)
{
	_id = glCreateShader(static_cast<uint32_t>(_shader_type));
	const char* c_str = _code.c_str();
	glShaderSource(_id, 1, &c_str, nullptr);

	Compile();
}

Shader& Shader::operator=(const Shader& other)
{
	if (this == &other)
		return *this;

	_code = other._code;
	_shader_type = other._shader_type;

	_id = glCreateShader(static_cast<uint32_t>(_shader_type));
	const char* c_str = _code.c_str();
	glShaderSource(_id, 1, &c_str, nullptr);

	Compile();

	return *this;
}

Shader::Shader(Shader&& other) noexcept
	: _code(std::move(other._code))
	, _shader_type(other._shader_type)
	, _id(other._id)
{
	other._id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this == &other)
		return *this;

	_code = other._code;
	_shader_type = other._shader_type;
	_id = other._id;

	other._id = 0;

	return *this;
}

Shader::Shader(const std::filesystem::path& path, ShaderType shader_type)
	: _shader_type(shader_type)
{
	_id = glCreateShader(static_cast<uint32_t>(_shader_type));
	if (_id == 0)
	{
		printf("Error: couldn't create shader\n");
		return;
	}

	auto path_to_shader = std::filesystem::current_path() / path;
	std::string VertexType = (_shader_type == ShaderType::Fragment ? "Fragment" : "Vertex");
	Log(log_shader, log_level::trace, fmt::format("{} shader path: {}", VertexType, path.string()));
	std::ifstream source_file(path_to_shader);
	if (source_file.is_open())
	{
		std::stringstream buffer;
		buffer << source_file.rdbuf();
		_code = std::move(buffer.str());
	}
	else
	{
		printf("Error: source file '%ls' can't be found\n", path.c_str());
		glDeleteShader(_id);
		_id = 0;
		return;
	}

	const char* c_str = _code.c_str();
	glShaderSource(_id, 1, &c_str, nullptr);

	Compile();

	_code.resize(0);
}
