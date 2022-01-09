#include <iostream>
#include "TButton.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include <SDL.h>

TButton::TButton(SDL_Rect src, SDL_Rect dst, SDL_Renderer* r, SDL_Texture* t)
	: Sprite(src, dst, r, t), m_state(STATE_UP) {}

bool TButton::MouseCollision()
{
	const int mx = EVMA::GetMousePos().x;
	const int my = EVMA::GetMousePos().y;
	return (mx < (m_dst.x + m_dst.w) && mx > m_dst.x &&
		    my < (m_dst.y + m_dst.h) && my > m_dst.y);
}

int TButton::Update()
{
	bool col = MouseCollision();
	switch (m_state)
	{
	case STATE_UP:
		if (col)   
			m_state = STATE_OVER;
		break;
	case STATE_OVER:
		if (!col)
			m_state = STATE_UP;
		else if (col && EVMA::MousePressed(1)) // 1 = left, 2 = middle, 3 = right
			m_state = STATE_DOWN;
		break;
	case STATE_DOWN:
		if (EVMA::MouseReleased(1))
		{
			if (col)
			{
				m_state = STATE_OVER;
				// Execute new "callback".
				Execute();
				return 1;
			}
			else 
				m_state = STATE_UP;
		}
		break;
	}
	return 0;
}

void TButton::Render()
{
	m_src.x = m_src.w * (int)m_state;
	SDL_RenderCopy(m_pRend, m_pText, &m_src, &m_dst);
}

// Create button subclasses and their overridden Execute methods below...