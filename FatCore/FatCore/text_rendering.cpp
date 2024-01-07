/***************************************************************************************************
FatBoy Engine -- text_rendering

Purpose: all classes necessary for rendering text strings to the screen, while seperating out the
system specific calls to do so.
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "render.h"
#include "render_2.h"
#include "text_rendering.h" //class header file, struct text_box, class text_render

//text_box constructor/deconstructor
fatcore::text_box::text_box(const std::string TEXT, const std::string FONT, const int SIZE, const loc BOX_LOCATION, const rgb TEXT_COLOR, const bool WRAP) :
text(TEXT), font(FONT), size(SIZE),
boxLoc(BOX_LOCATION), wrap(WRAP), initialized(true), textColor(TEXT_COLOR), center(false),
#ifdef _WIN32
	font_Win32(NULL)
#endif
{
	//register the text_box
	text_render textMan;
	textMan.registerTextBox(this);
}
fatcore::text_box::~text_box()
{
	//remove this box from the registry
	text_render textMan;
	textMan.removeTextBox(this);
}

//text_render static variable declairations
int fatcore::text_render::m_instanceCount = 0;
bool fatcore::text_render::m_cleanUpComplete = false;
std::vector<fatcore::text_box*> fatcore::text_render::m_textBoxes;

//text_render member function deffinitions
fatcore::text_render::text_render()
{
	//incriment the instance count
	m_instanceCount++;
}
fatcore::text_render::~text_render()
{
	//deincriment the instance count
	m_instanceCount--;

	//last one performs cleanup
	if (m_instanceCount < 1)
	{
		//flag cleanup as completed
		m_cleanUpComplete = true;

		for (unsigned int i = 0; i < m_textBoxes.size(); ++i)
		{
			if (m_textBoxes[i]) delete m_textBoxes[i];
		}
	}
}
void fatcore::text_render::registerTextBox(fatcore::text_box* pTextBox)
{
	//reassign any unused text_box pointers, rather than adding more;
	for (unsigned int i = 0; i < m_textBoxes.size(); ++i)
	{
		if (!m_textBoxes[i])
		{
			m_textBoxes[i] = pTextBox;
			initializeFont(pTextBox);
			return;
		}
	}

	//if a reassignment was not available, add new pointer at the end
	m_textBoxes.push_back(pTextBox);

	//initialize the font for rendering
	initializeFont(pTextBox);
}
void fatcore::text_render::removeTextBox(fatcore::text_box* pTextBox)
{
	for (unsigned int i = 0; i < m_textBoxes.size(); ++i)
	{
		if (m_textBoxes[i] == pTextBox)
		{
			if (m_textBoxes[i]->font_Win32)
			{
				m_textBoxes[i]->font_Win32->Release();
				m_textBoxes[i]->font_Win32 = NULL;
				m_textBoxes[i]->initialized = false;
			}
			m_textBoxes[i] = NULL;
		}
	}
}
fatcore::text_box* fatcore::text_render::getTextBoxByContent(const std::string TEXT)
{
	for (unsigned int i = 0; i < m_textBoxes.size(); ++i)
	{
		//skip any text boxes that were deleted
		if (!m_textBoxes[i]) continue;

		//check if the current text matches the search
		if (TEXT == m_textBoxes[i]->text)
		{
			return m_textBoxes[i];
		}
	}

	//return NULL if no match was found
	return NULL;
}
void fatcore::text_render::render()
{
	for (unsigned int i = 0; i < m_textBoxes.size(); ++i)
	{
		if (!m_textBoxes[i]) continue;

#ifdef _WIN32 //windows with directX rendering
		render_WIN32(m_textBoxes[i]);
#endif
	}
}
void fatcore::text_render::initializeFont(fatcore::text_box* pTextBox)
{
#ifdef _WIN32
	initializeFont_WIN32(pTextBox);
#endif
}
#ifdef _WIN32
void fatcore::text_render::render_WIN32(fatcore::text_box* pTextBox)
{
	//get local sprite object and verify
	fatcore::render rDev;
	LPD3DXSPRITE localSprite = rDev.getSprite();

	//determine text boundry
	RECT rect;
	if (pTextBox->wrap)
	{
		rect = {
			pTextBox->boxLoc.x,
			pTextBox->boxLoc.y,
			pTextBox->boxLoc.width,
			pTextBox->boxLoc.height
		};
	}
	else
	{
		rect = {
			pTextBox->boxLoc.x,
			pTextBox->boxLoc.y,
			0,
			0
		};
	}
	
	pTextBox->font_Win32->DrawTextA(NULL, pTextBox->text.c_str(), pTextBox->text.length(), &rect, DT_CALCRECT,
		D3DCOLOR_XRGB(pTextBox->textColor.r, pTextBox->textColor.g, pTextBox->textColor.b));

	D3DXVECTOR2 scale(1.0, 1.0);
	D3DXVECTOR2 trans(0, 0);
	D3DXVECTOR2 center(0, 0);

	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, 0, &trans);
	localSprite->SetTransform(&mat);

	//print the text
	if (pTextBox->center)
	{
		rect = {
			pTextBox->boxLoc.x,
			pTextBox->boxLoc.y,
			pTextBox->boxLoc.width,
			pTextBox->boxLoc.height
		};

		pTextBox->font_Win32->DrawTextA(localSprite, pTextBox->text.c_str(), pTextBox->text.length(), &rect, DT_CENTER,
			D3DCOLOR_XRGB(pTextBox->textColor.r, pTextBox->textColor.g, pTextBox->textColor.b));
	}
	else
	{
		pTextBox->font_Win32->DrawTextA(localSprite, pTextBox->text.c_str(), pTextBox->text.length(), &rect, DT_LEFT,
			D3DCOLOR_XRGB(pTextBox->textColor.r, pTextBox->textColor.g, pTextBox->textColor.b));
	}
}
void fatcore::text_render::initializeFont_WIN32(fatcore::text_box* pTextBox)
{
	D3DXFONT_DESC desc = {
		pTextBox->size,
		0,
		0,
		0,
		false,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		""
	};

	strcpy(desc.FaceName, pTextBox->font.c_str());
	fatcore::render rendMan;
	D3DXCreateFontIndirect(rendMan.getDevice(), &desc, &pTextBox->font_Win32);
}
#endif

fatcore::text_render __PLACEHOLDER_TEXT_RENDER__;