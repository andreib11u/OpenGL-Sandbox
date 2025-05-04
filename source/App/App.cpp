#include "glad/glad.h"

#include "App.h"
#include "Input/InputListener.h"
#include "Render/RenderObject.h"
#include "Render/Shader.h"
#include "spdlog/spdlog.h"

DEFINE_LOGGER(app)

App::App() = default;

void App::SetupInput()
{
	_input_listener = std::make_unique<InputListener>(_window);
	_input_listener->AddInput(GLFW_KEY_ESCAPE, KeyState::KeyDown, [&] { glfwSetWindowShouldClose(_window, true); });

	Log(log_app, log_level::debug, "Input is setup");
}

void App::SetupScene()
{
	std::vector vertices = {
		core::point{0.5f, 0.5f, 0.0f},	 // top right
		core::point{0.5f, -0.5f, 0.0f},	 // bottom right
		core::point{-0.5f, -0.5f, 0.0f}, // bottom left
		core::point{-0.5f, 0.5f, 0.0f}	 // top left
	};

	std::vector<uint32_t> indices = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	// setup shaders
	auto vertex_shader = Shader(_shaders_dir / "vertex_shader.glsl", ShaderType::Vertex);
	auto fragment_shader = Shader(_shaders_dir / "fragment_shader.glsl", ShaderType::Fragment);

	_shader_program.Init();
	_shader_program.AttachShader(vertex_shader);
	_shader_program.AttachShader(fragment_shader);
	_shader_program.Link();

	_square = RenderObject(vertices, indices);

	Log(log_app, log_level::debug, "Scene is setup");
}

bool App::Init()
{
	Logger::Initialize();

	if (!glfwInit())
		return false;
	Log(log_app, log_level::debug, "glfw is initialized");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(window_width, window_height, _name.c_str(), nullptr, nullptr);
	if (!_window)
	{
		Log(log_app, log_level::err, "glfw couldn't create a window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log(log_app, log_level::err, "Failed to initialize GLAD");
		return false;
	}

	_viewport.height = window_height;
	_viewport.width = window_width;
	_viewport.x = 0;
	_viewport.y = 0;
	glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height);

	glClearColor(_clear_color.red, _clear_color.green, _clear_color.blue, _clear_color.alpha);

	SetupCallbacks();
	SetupInput();
	SetupScene();

	Log(log_app, log_level::info, "App initialized");

	return true;
}

void App::Draw()
{
	_shader_program.Use();
	_square.Render();
}

void App::Run()
{
	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		_input_listener->ListenToInput();

		Draw();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void App::Exit()
{
	glfwTerminate();

	Log(log_app, log_level::info, "Exit called. App is shutdown");
	Logger::Shutdown();
}

App::~App() = default;

void App::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	App& app = Get();
	app._viewport.width = width;
	app._viewport.height = height;
	glViewport(app._viewport.x, app._viewport.y, app._viewport.width, app._viewport.height);
}

void App::SetupCallbacks()
{
	glfwSetFramebufferSizeCallback(_window, FramebufferSizeCallback);

	Log(log_app, log_level::debug, "Callbacks are set");
}

App& App::Get()
{
	static App app;
	return app;
}
