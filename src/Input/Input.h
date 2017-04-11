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
		void clear();
		void    update(SDL_Event *event);
		bool  AnyKey();
		void kbeat();
		bool   b(const int);
		bool   bp(const int);
		bool   br(const int);
		SDL_Event *getEvent();
	private:
		std::map<int, bool> currentKey;
		std::map<int, bool> Key;
		SDL_Event    *_event;
	};
	extern KeyboardInput Kb;
	// -- implementation 
}
#endif