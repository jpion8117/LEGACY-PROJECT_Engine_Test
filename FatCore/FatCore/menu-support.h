/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all sprites displayed on the
screen.
***************************************************************************************************/

#pragma once

#include "sprites.h"
#include "text_rendering.h"
#include "render_2.h"

namespace fatcore
{
#ifndef _MENU_SPRITES_HEADER_
#define _MENU_SPRITES_HEADER_

	enum BUTTON_RETURNS
	{
		BUTTON_SELECTED = 0,
		BUTTON_CLICKED
	};

	class cursor : public sprite
	{
	public:
		cursor(const std::string FILENAME = "", const loc INITIAL_SOURCE = makeLocation(0, 0, 0, 0), 
			const loc INITIAL_RENDER = makeLocation(0, 0, 0, 0), const bool ACTIVE = true);
		~cursor() { sprite::~sprite(); }
		virtual void procFrame();

	private:
		virtual void procCD();

		const std::string DIN_X = "DINPUT_X_AXIS";
		const std::string DIN_Y = "DINPUT_Y_AXIS";
		const std::string XIN_X = "XINPUT_X_AXIS";
		const std::string XIN_Y = "XINPUT_Y_AXIS";
	};
	class button : public sprite
	{
	public:
		button(const std::string FILENAME = "", const loc INITIAL_SOURCE = { 0, 0, 0, 0 }, 
			const loc SELECTED_SOURCE = { 0, 0, 0, 0 }, const loc INITIAL_RENDER = { 0, 0, 0, 0 }, 
			const int PRIORITY = 100);
		~button();
		virtual void procFrame();
		inline void setScaleX(float scale){ render2 rDev; rDev.setScaleX(m_uniqueVisualID, scale); }
		void setLabel(const std::string TEXT, const std::string FONT, const int SIZE, const rgb COLOR = { 0, 0, 0 });

	private:
		virtual void procCD();

		const loc m_selected;
		const loc m_unselected;

		text_box* m_label;
	};

#endif
}