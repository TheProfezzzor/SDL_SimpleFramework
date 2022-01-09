#pragma once
#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include "TLabel.h"
#include "SoundManager.h"
#include <algorithm>
#include <SDL.h>

enum uiState { STATE_UP, STATE_OVER, STATE_DOWN };

class UIElement // Abstract base class.
{
public: // Whatever we need to access through the base class pointer has to be public.
	virtual ~UIElement() {} // Invoked for delete calls if subclasses are stored through base class pointers.
	virtual bool Update() { return 0; }; // Pure virtual.
	virtual void Render() = 0; // Pure virtual.
	
protected:
	UIElement(SDL_Rect d, SDL_Renderer* r, SDL_Color c);
	Uint8 ClampRGB(int c) { return (Uint8)(std::min(std::max(c,0), 255)); }

protected:
	SDL_Renderer* m_pRend;
	SDL_Rect m_dst;
	SDL_Color m_color; 
	uiState m_state;
};

class UIButton : public UIElement // Also abstract. Still a command pattern.
{
public:
	bool Update();
	void Render();

protected:
	UIButton(SDL_Rect d, SDL_Renderer* r, SDL_Color c, const char* name);
	~UIButton(); // Make sure UIButtons are stored through pointers to UIElement base class or make this public.
	virtual void Execute() = 0; // Makes it abstract.
	void SetState(const uiState& s, const int y);

private:
	SDL_Rect m_new;
	TLabel *m_pName;
};

class UISlider : public UIElement
{
public:
	UISlider(SDL_Rect d, SDL_Renderer* r, SDL_Color col, SDL_Color colText, const char* name, int startVal = 100, int minVal = 0, int maxVal = 100);
	bool Update();
	void Render();
	int Value() { return m_val; }
	void SetValue(int i);

private:
	~UISlider(); // Make sure UISliders are stored through pointers to UIElement base class or make this public.

private:
	int m_val, m_minVal, m_maxVal;
	SDL_Rect m_handle;
	TLabel *m_pName, *m_pVal, *m_pMin, *m_pMax;
};

// Unique button subclasses here. Inline is okay.


#endif