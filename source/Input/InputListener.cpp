#include "InputListener.h"
#include "GLFW/glfw3.h"

InputListener::InputListener(GLFWwindow* window)
	: _window{window}
{
}

InputListener::~InputListener() = default;

void InputListener::ListenToInput()
{
	for (auto& [key, action] : _input_mappings)
	{
		if (glfwGetKey(_window, key) == static_cast<int>(KeyState::KeyDown))
		{
			if (action.on_key_down && !action.pressed)
			{
				action.pressed = true;
				action.on_key_down();
			}
			if (action.being_pressed)
			{
				action.being_pressed();
			}
		}
		if (glfwGetKey(_window, key) == static_cast<int>(KeyState::KeyUp))
		{
			if (action.on_key_up && action.pressed)
			{
				action.pressed = false;
				action.on_key_up();
			}
		}
	}

	FlushAddedInput();
}

void InputListener::FlushAddedInput()
{
	for (const auto& [key, binding] : _input_mappings_to_add)
	{
		auto found_mapping = _input_mappings.find(key);
		InputAction input_action;
		auto key_state = binding.first;
		if (found_mapping != _input_mappings.end())
		{
			input_action = found_mapping->second;
		}

		switch (key_state)
		{
		case KeyState::KeyUp:
			input_action.on_key_up = binding.second;
			break;
		case KeyState::KeyDown:
			input_action.on_key_down = binding.second;
			break;
		case KeyState::KeyDownRepeat:
			input_action.being_pressed = binding.second;
			break;
		default:
			assert(false);
			break;
		}

		_input_mappings.insert_or_assign(key, input_action);
	}
}
