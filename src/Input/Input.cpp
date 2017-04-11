#include "../Input/Input.h"

namespace Falltergeist {
	KeyboardInput Kb;
	KeyboardInput::KeyboardInput() {
	}

	KeyboardInput::~KeyboardInput() {
	}

	void KeyboardInput::update(SDL_Event *event) {
		if (SDL_KEYDOWN == event->type) {
			int value = event->key.keysym.sym;
			if(!_currentKey[value])
				_Key[value] = true;
			_currentKey[value] = true;
		}
		if (SDL_KEYUP == event->type) {
			int value = event->key.keysym.sym;
			_Key[value] = false;
			currentKey[value] = false;
		}

	}

	bool KeyboardInput::AnyKey() {
		for(auto v: _Key)
		if (v.second)
		{
			return true;
		}
		return false;
	}

	void KeyboardInput::kbeat() {
		_Key.clear();
	}

	bool KeyboardInput::b(const int key) {
		return _Key[key];
	}

	bool KeyboardInput::bp(const int key) {
		bool ret = _Key[key];
		_Key.erase(key);
		return ret;
	}

	bool KeyboardInput::br(const int key) {
		bool ret = _Key[key];
		_Key.erase(key);
		return ret;
	}
}