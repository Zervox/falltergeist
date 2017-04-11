#ifndef FALLTERGEIST_INPUT_H
#define FALLTERGEIST_INPUT_H
#define SDL_MAIN_HANDLED
#include <map>
#include <SDL_image.h>
namespace Falltergeist
{
	// -- basic input-handling class 
	class KeyboardInput {
	public:
		KeyboardInput();
		~KeyboardInput();
		void    update(SDL_Event *event);
		bool  AnyKey();
		void kbeat();
		bool   b(const int);
		bool   bp(const int);
		bool   br(const int);
	private:
		std::map<int, bool> _currentKey;
		std::map<int, bool> _Key;
	};
	extern KeyboardInput Kb;
	// -- implementation 
}
#endif