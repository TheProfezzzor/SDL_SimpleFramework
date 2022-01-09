#include "Engine.h"
#include "TLabel.h"
#include "FontManager.h"
#include <cstring>

TLabel::TLabel(std::string key, const int x, const int y, const char* str,
	const SDL_Color col) :m_TextColor(col)
{
	m_Font = FontManager::GetFont(key);
	SetPos(x, y);
	SetText(str);
}

TLabel::~TLabel()
{
	SDL_DestroyTexture(m_pTexture);
}

void TLabel::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pTexture, 0, &m_rTextRect);
}

void TLabel::SetText(const char* str)
{
	strcpy_s(m_String, 256, str);
	SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, m_String, m_TextColor);
	SDL_DestroyTexture(m_pTexture);
	m_pTexture = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), fontSurf);
	m_rTextRect = { m_rTextRect.x, m_rTextRect.y, fontSurf->w, fontSurf->h };
	SDL_FreeSurface(fontSurf);
}

void TLabel::SetPos(const int x, const int y)
{
	m_rTextRect = { x, y, m_rTextRect.w, m_rTextRect.h };
}

void TLabel::SetColor(const SDL_Color& col)
{
	m_TextColor = col;
	SetText(m_String);
}

void TLabel::UseFont(std::string key)
{
	SetText(m_String);
}