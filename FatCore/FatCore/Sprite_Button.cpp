/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all sprites displayed on the
screen.
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "debug.h"
#include "core.h"
#include "menu-support.h"
#include "render_2.h"

fatcore::button::button(const std::string FILENAME, const loc INITIAL_SOURCE,
	const loc SELECTED_SOURCE, const loc INITIAL_RENDER, const int PRIORITY) :
	sprite(FILENAME, INITIAL_SOURCE, INITIAL_RENDER, PRIORITY),
	m_selected(SELECTED_SOURCE),
	m_unselected(INITIAL_SOURCE)
{
	assignInput(fbc_input::MOUSE_CLICK_LEFT, "BUTTON_CLICK");
	assignInput(fbc_input::XA, "BUTTON_CLICK");
	assignInput(fbc_input::RT_DIGITAL, "BUTTON_CLICK");

	m_CDGroupName = fbc_spriteID::CDGROUP_BUTTON;
	m_spriteClassName = fbc_spriteID::BUTTON;
	m_label = NULL;
}
fatcore::button::~button()
{
	//run the default sprite deconstructor
	//sprite::~sprite();

	//delete the label if one was used
	text_render tMan;
	if (m_label && (!tMan.cleanUpComplete()))
	{
		delete m_label;
		m_label = NULL;
	}
}
void fatcore::button::procFrame()
{
	//reset frame
	boolReturns[BUTTON_CLICKED] = false;

	render2 renderDev;
	renderDev.setSourceLoc(FRGD, m_uniqueVisualID, m_unselected);

	if (boolReturns[BUTTON_SELECTED])
	{
		renderDev.setSourceLoc(FRGD, m_uniqueVisualID, m_selected);
		bool clicked = false;
		
		for (unsigned int i = 0; i < m_inputs.size(); ++i)
		{
			bool temp = false;
			input inputDev;
			inputDev.checkStatus(temp, m_inputs[i].uniqueID);
			if (temp) clicked = true;
		}

		boolReturns[BUTTON_CLICKED] = clicked;
	}
}
void fatcore::button::procCD()
{
	//reset colision detected bool
	boolReturns[BUTTON_SELECTED] = false;

	std::vector<sprite*> cursors = getSpritesInCDGroup(fbc_spriteID::CDGROUP_CURSOR);

	for (unsigned int i = 0; i < cursors.size(); ++i)
	{
		if (locIntersect(cursors[i]->getRenderLoc(), m_physics.Location))
		{
			boolReturns[BUTTON_SELECTED] = true;
		}
	}
}
void fatcore::button::setLabel(const std::string TEXT, const std::string FONT, const int SIZE, const fatcore::rgb COLOR)
{
	//if label is initialized and needs to be reinitialized, delete it
	bool reinitiailize = false;
	if (m_label)
	{
		if (FONT != m_label->font)
			reinitiailize = true;

		else if (SIZE != m_label->size)
			reinitiailize = true;
	}
	else
	{
		m_label = new text_box("", FONT, SIZE);
	}

	if (reinitiailize)
	{
		delete m_label;
		m_label = new text_box("", FONT, SIZE);
	}

	//change other parameters
	m_label->text = TEXT;
	m_label->textColor = COLOR;
	m_label->center = true;

	//determine label location
	loc temp =
	{
		m_physics.Location.x,
		((((m_physics.Location.height - m_physics.Location.y) / 2) + m_physics.Location.y)) - (SIZE / 2),
		m_physics.Location.width,
		(m_physics.Location.height - (((m_physics.Location.height - m_physics.Location.y) / 2))) + (SIZE / 2)
	};

	m_label->boxLoc = temp;
}