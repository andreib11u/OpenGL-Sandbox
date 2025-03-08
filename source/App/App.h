#pragma once

#include "App/Log/Log.h"
#include "GLFW/glfw3.h"
#include "Render/RenderObject.h"
#include "Render/ShaderProgram.h"
#include "core_types.h"
#include <filesystem>
#include <memory>
#include <string>

class InputListener;
struct GLFWwindow;

class App
{
public:
	App();

	static App& Get();

	constexpr static int window_height = 600;
	constexpr static int window_width = 800;
	constexpr static int major_version = 3;
	constexpr static int minor_version = 3;

	bool Init();
	void Draw();
	void Run();
	void Exit();

	~App();

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void SetupCallbacks();
	void SetupInput();
	void SetupScene();

	// configuration
	core::rgba _clear_color = {0.2f, 0.3f, 0.3f, 1.0f};
	std::string _name = "Learn OpenGL";
	std::filesystem::path _shaders_dir = {"Shaders"};

	GLFWwindow* _window = nullptr;
	core::rect _viewport = {};
	std::unique_ptr<InputListener> _input_listener;
	ShaderProgram _shader_program;
	RenderObject _square;
};
