#pragma once
#ifndef _TLABEL_H_
#define _TLABEL_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TLabel
{
public: // Public methods.
	TLabel(std::string key, const int x, const int y, const char* str, const SDL_Color col = { 255,255,255,255 });
	~TLabel();
	void Render();
	void SetText(const char* c);
	void SetPos(const int x, const int y);
	void SetColor(const SDL_Color& col);
	void UseFont(std::string key);
	SDL_Rect& GetRect() { return m_rTextRect; }

private: // Private properties.
	TTF_Font* m_Font;
	SDL_Color m_TextColor;
	SDL_Rect m_rTextRect;
	SDL_Texture* m_pTexture;
	char m_String[256];
};

#endif