#include "UIElement.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include <string>

UIElement::UIElement(SDL_Rect d, SDL_Renderer* r, SDL_Color c)
	:m_dst(d), m_pRend(r), m_color(c), m_state(STATE_UP) {}


UIButton::UIButton(SDL_Rect d, SDL_Renderer* r, SDL_Color c, const char* name)
	: UIElement(d, r, c), m_new(d)
{
	m_pName = new TLabel("ui", 0, 0, name, { ClampRGB(m_color.r - 80), ClampRGB(m_color.g - 80), ClampRGB(m_color.b - 80), c.a });
	m_pName->SetPos(d.x + d.w / 2 - m_pName->GetRect().w / 2, d.y + d.h / 2 - m_pName->GetRect().h / 2);
}

bool UIButton::Update()
{
	bool col = COMA::PointAABBCheck(EVMA::GetMousePos(), m_dst);
	switch (m_state)
	{
	case STATE_UP:
		if (col)
			SetState(STATE_OVER, m_dst.y - 2);
		break;
	case STATE_OVER:
		if (!col)
			SetState(STATE_UP, m_dst.y);
		else if (col && EVMA::MousePressed(1)) // 1 = left, 2 = middle, 3 = right
			SetState(STATE_DOWN, m_dst.y + 2);
		break;
	case STATE_DOWN:
		if (EVMA::MouseReleased(1))
		{
			if (col)
			{
				SetState(STATE_OVER, m_dst.y - 2);
				Execute();
				return 1;
			}
			else
				SetState(STATE_UP, m_dst.y);
		}
		break;
	}
	return 0;
}

void UIButton::Render()
{
	if (m_state == STATE_UP)
		SDL_SetRenderDrawColor(m_pRend, m_color.r, m_color.g, m_color.b, m_color.a);
	else if (m_state == STATE_OVER)
		SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r + 32), ClampRGB(m_color.g + 32), ClampRGB(m_color.b + 32), m_color.a);
	else if (m_state == STATE_DOWN)
		SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 16), ClampRGB(m_color.g - 16), ClampRGB(m_color.b - 16), m_color.a);
	SDL_RenderFillRect(m_pRend, &m_new); // Main button rect render.
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 32), ClampRGB(m_color.g - 32), ClampRGB(m_color.b - 32), m_color.a);
	SDL_RenderDrawRect(m_pRend, &m_new); // Right-Bottom border.
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r + 48), ClampRGB(m_color.g + 48), ClampRGB(m_color.b + 48), m_color.a);
	SDL_RenderDrawLine(m_pRend, m_new.x, m_new.y, m_new.x, m_new.y + m_new.h - 1); // Left side highlight.
	SDL_RenderDrawLine(m_pRend, m_new.x, m_new.y, m_new.x + m_new.w - 1, m_new.y); // Top highlight.
	m_pName->Render();
}

void UIButton::SetState(const uiState& s, const int y)
{
	m_state = s;
	m_new.y = y;
	m_pName->SetPos(m_new.x + m_new.w / 2 - m_pName->GetRect().w / 2, m_new.y + m_new.h / 2 - m_pName->GetRect().h / 2);
	if (s == STATE_OVER)
	{
		EVMA::SetCursor(SDL_SYSTEM_CURSOR_HAND);
		m_pName->SetColor({ ClampRGB(m_color.r - 32), ClampRGB(m_color.g - 32), ClampRGB(m_color.b - 32), m_color.a });
	}
	else if (s == STATE_DOWN)
	{
		EVMA::SetCursor(SDL_SYSTEM_CURSOR_HAND);
		m_pName->SetColor({ ClampRGB(m_color.r - 96), ClampRGB(m_color.g - 96), ClampRGB(m_color.b - 96), m_color.a });
	}
	else
	{
		EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
		m_pName->SetColor({ ClampRGB(m_color.r - 80), ClampRGB(m_color.g - 80), ClampRGB(m_color.b - 80), m_color.a });
	}
}

UIButton::~UIButton()
{
	delete m_pName;
}

UISlider::UISlider(SDL_Rect d, SDL_Renderer * r, SDL_Color col, SDL_Color colText, const char* name, int startVal, int minVal, int maxVal)
	: UIElement(d, r, col), m_val(startVal), m_minVal(minVal), m_maxVal(maxVal)
{ // val - min / max - min -> 0 to 1
	m_handle = { (int)(d.x + 2 + ((d.w - d.h) * ((double)(m_val - m_minVal) / (double)(m_maxVal - m_minVal)))) , d.y + 2, d.h - 4, d.h - 4 };
	m_pName = new TLabel("ui", 0, 0, name, colText);
	m_pName->SetPos(d.x - m_pName->GetRect().w - 8, d.y + d.h / 2 - m_pName->GetRect().h / 2);
	m_pVal = new TLabel("ui", 0, 0, std::to_string(m_val).c_str(), colText);
	m_pVal->SetPos(m_handle.x + m_handle.w / 2 - m_pVal->GetRect().w / 2, d.y - m_pVal->GetRect().h - 4);
	m_pMin = new TLabel("ui", 0, 0, std::to_string(m_minVal).c_str(), colText);
	m_pMin->SetPos(d.x + 2 + m_handle.w / 2 - m_pMin->GetRect().w / 2, d.y + d.h + 4);
	m_pMax = new TLabel("ui", 0, 0, std::to_string(m_maxVal).c_str(), colText);
	m_pMax->SetPos(d.x + d.w - m_handle.w / 2 - 2 - m_pMax->GetRect().w / 2, d.y + d.h + 4);
}

UISlider::~UISlider()
{
	delete m_pName;
	delete m_pVal;
	delete m_pMin;
	delete m_pMax;
}

bool UISlider::Update()
{
	SDL_Rect temp = { m_handle.x - 6, m_handle.y - 6, m_handle.w + 12, m_handle.h + 12 };
	bool col = COMA::PointAABBCheck(EVMA::GetMousePos(), temp);
	switch (m_state)
	{
	case STATE_UP:
		if (col)
		{
			EVMA::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
			m_state = STATE_OVER;
		}
		break;
	case STATE_OVER:
		if (!col)
		{
			EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			m_state = STATE_UP;
		}
		else
		{
			if (EVMA::MouseHeld(1))
			{
				m_handle.x = MAMA::ClampI((EVMA::GetMousePos().x - m_handle.w / 2), (m_dst.x + 2), (m_dst.x + 2 + m_dst.w - m_dst.h));
				m_val = m_minVal + (int)((double)(m_maxVal - m_minVal) * ((double)m_handle.x - (m_dst.x + 2.0)) / ((m_dst.x + 2.0 + m_dst.w - m_dst.h) - (m_dst.x + 2.0)));
				m_pVal->SetText(std::to_string(m_val).c_str());
				m_pVal->SetPos(m_handle.x + m_handle.w / 2 - m_pVal->GetRect().w / 2, m_dst.y - m_pVal->GetRect().h - 4);
			}
		}
		break;
	}
	return 0;
}

void UISlider::Render()
{
	// Render labels first!
	m_pName->Render();
	m_pMin->Render();
	m_pMax->Render();
	m_pVal->Render();
	// Then geometry.
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 48), ClampRGB(m_color.g - 48), ClampRGB(m_color.b - 48), m_color.a);
	SDL_RenderFillRect(m_pRend, &m_dst);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 64), ClampRGB(m_color.g - 64), ClampRGB(m_color.b - 64), m_color.a);
	SDL_RenderDrawRect(m_pRend, &m_dst);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 96), ClampRGB(m_color.g - 96), ClampRGB(m_color.b - 96), m_color.a);
	SDL_RenderDrawLine(m_pRend, m_dst.x + m_dst.h / 2, m_dst.y + m_dst.h / 2, m_dst.x + m_dst.w - m_dst.h / 2, m_dst.y + m_dst.h / 2);
	SDL_SetRenderDrawColor(m_pRend, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(m_pRend, &m_handle);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 64), ClampRGB(m_color.g - 64), ClampRGB(m_color.b - 64), m_color.a);
	for (int i = -1; i <= 1; i++)
		SDL_RenderDrawLine(m_pRend, m_handle.x + m_handle.w / 2 - (i*m_handle.h / 6), m_handle.y + m_handle.h / 2 - m_handle.h / 6,
			m_handle.x + m_handle.w / 2 - (i*m_handle.h / 6), m_handle.y + m_handle.h / 2 + m_handle.h / 6);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r + 48), ClampRGB(m_color.g + 48), ClampRGB(m_color.b + 48), m_color.a);
	SDL_RenderDrawLine(m_pRend, m_dst.x, m_dst.y, m_dst.x, m_dst.y + m_dst.h - 1);
	SDL_RenderDrawLine(m_pRend, m_dst.x, m_dst.y, m_dst.x + m_dst.w - 1, m_dst.y);
}

void UISlider::SetValue(int i)
{
	m_val = std::min(std::max(i, m_minVal), m_maxVal);
	m_handle.x = (int)(m_dst.x + 2 + ((double)(m_dst.w - m_dst.h) * ((double)(m_val - m_minVal) / (double)(m_maxVal - m_minVal))));
	m_pVal->SetText(std::to_string(m_val).c_str());
	m_pVal->SetPos(m_handle.x + m_handle.w / 2 - m_pVal->GetRect().w / 2, m_dst.y - m_pVal->GetRect().h - 4);
}