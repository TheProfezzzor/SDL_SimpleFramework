#pragma once
#ifndef _TBUTTON_H_
#define _TBUTTON_H_

#include "Sprite.h"
#include <SDL.h>

// Button is an example of the command design pattern.
class TButton : public Sprite // Must be int-based Rects!
{
public: // Public methods.
	int Update();
	void Render();
protected: // Private but inherited!
	TButton(SDL_Rect src, SDL_Rect dst, SDL_Renderer* r, SDL_Texture* t);
	enum state { STATE_UP, STATE_OVER, STATE_DOWN } m_state;
	bool MouseCollision();
	virtual void Execute() = 0; // Pure virtual method, meaning Button is now an abstract class (interface)!
};

// List button subclasses here...

#endif