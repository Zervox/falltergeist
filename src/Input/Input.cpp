#include "../Input/Input.h"

namespace Falltergeist {
	KeyboardInput Kb;
	KeyboardInput::KeyboardInput() {
	}

	KeyboardInput::~KeyboardInput() {
	}

	void KeyboardInput::clear() {}

	void KeyboardInput::update(SDL_Event *event) {
		
		_event = event;
		
		if (SDL_KEYDOWN == _event->type) {
			int value = _event->key.keysym.sym;
			if(!currentKey[value])
				Key[value] = true;
			currentKey[value] = true;
		}
		if (SDL_KEYUP == _event->type) {
			int value = _event->key.keysym.sym;
			Key[value] = false;
			currentKey[value] = false;
		}

	}

	bool KeyboardInput::AnyKey() {
		for(auto v: Key)
		if (v.second)
		{
			return true;
		}
		return false;
	}

	void KeyboardInput::kbeat() {
		Key.clear();
	}

	bool KeyboardInput::b(const int key) {
		return Key[key];
	}

	bool KeyboardInput::bp(const int key) {
		bool ret = Key[key];
		Key.erase(key);
		return ret;
	}

	bool KeyboardInput::br(const int key) {
		bool ret = Key[key];
		Key.erase(key);
		return ret;
	}

	SDL_Event *KeyboardInput::getEvent() {
		return _event;
	}
}