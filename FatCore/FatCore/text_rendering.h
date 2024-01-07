/***************************************************************************************************
FatBoy Engine -- text_rendering

Purpose: all classes necessary for rendering text strings to the screen, while seperating out the
system specific calls to do so.
***************************************************************************************************/

#include "universal_includes.h"
#include "core.h"

namespace fatcore
{
#ifndef _TEXT_OUTPUT_HEADER_
#define _TEXT_OUTPUT_HEADER_

	struct text_box
	{
	public:
		std::string text;
		std::string font;
		loc boxLoc;
		rgb textColor;
		int size;
		bool wrap;
		bool initialized;
		bool center;

		text_box(const std::string TEXT = "", const std::string FONT = "", const int SIZE = 0, const loc BOX_LOCATION = { 0, 0, 0, 0 }, const rgb TEXT_COLOR = { 0, 0, 0 }, const bool WRAP = false);
		~text_box();

#ifdef _WIN32
		LPD3DXFONT font_Win32;
#endif
	};

	class text_render
	{
		friend struct text_box;
	public:
		text_render();
		~text_render();
		void render();
		inline bool cleanUpComplete() { return m_cleanUpComplete; }
		text_box* getTextBoxByContent(const std::string TEXT);

	private:
		static std::vector<text_box*> m_textBoxes;
		static int m_instanceCount;
		static bool m_cleanUpComplete;

		void registerTextBox(text_box* pTextBox);
		void removeTextBox(text_box* pTextBox);
		void initializeFont(text_box* pTextBox);

#ifdef _WIN32

		inline void render_WIN32(text_box* pTextBox);
		void initializeFont_WIN32(text_box* pTextBox);

#endif
	};

#endif
}