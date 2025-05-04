#pragma once
#include <cassert>
#include <functional>
#include <unordered_map>

enum class KeyState
{
	KeyUp,
	KeyDown,
	KeyDownRepeat
};

struct InputAction
{
	bool pressed = false;
	std::function<void()> on_key_down;
	std::function<void()> on_key_up;
	std::function<void()> being_pressed;
};

struct GLFWwindow;

class InputListener
{
public:
	explicit InputListener(GLFWwindow* window);
	~InputListener();

	InputListener(const InputListener&) = delete;
	InputListener(InputListener&&) = delete;
	InputListener& operator=(InputListener&) = delete;
	InputListener& operator=(InputListener&&) = delete;

	template <typename Callback>
	void AddInput(int32_t key, KeyState key_state, Callback callback);

	void ListenToInput();
	void FlushAddedInput();

private:
	std::unordered_map<int32_t, InputAction> _input_mappings;
	std::vector<std::pair<int32_t, std::pair<KeyState, std::function<void()>>>> _input_mappings_to_add;
	GLFWwindow* _window;
};

template <typename Callback>
void InputListener::AddInput(int32_t key, KeyState key_state, Callback callback)
{
	std::function<void()> action = callback;
	std::pair new_pair{key_state, action};
	_input_mappings_to_add.emplace_back(key, new_pair);
}
