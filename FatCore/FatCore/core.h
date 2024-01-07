/***************************************************************************************************
FatBoy Engine -- Core

Purpose: The most basic and fundimental building blocks of the engine, simple yet invaluable
items defining coordinates, colors, ect
***************************************************************************************************/
#define CORE
#pragma once

#include <string>

namespace fatcore
{
#ifndef CORE_MAIN_FILE_FATBOY_ENGINE
#define CORE_MAIN_FILE_FATBOY_ENGINE

	//global variables
	extern bool game_running;
	extern std::string game_title;

	//core function declarations
	void Game_Loop();
	void Game_Shutdown();
	int Game_Init(); //returns an error code upon failure, Otherwise returns fbc_err::OK

	//core class declarations/definitions
	class location //basic 4pt coord box, see line 28 for alternative names
	{
	public:
		location(int _x = 0, int _y = 0, int _width = 0, int _height = 0) { x = _x; y = _y; width = _width; height = _height; }
		void set(int _x, int _y, int _width, int _height) { x = _x; y = _y; width = _width; height = _height; }
		location& operator= (const int COORDS[4]) // {x, y, width, height}
		{
			x = COORDS[0];
			y = COORDS[1];
			width = COORDS[2];
			height = COORDS[3];
		}
		location& operator= (const location& ORIGINAL)
		{
			x = ORIGINAL.x;
			y = ORIGINAL.y;
			width = ORIGINAL.width;
			height = ORIGINAL.height;

			return *this;
		}

		int x;
		int y;
		int width;
		int height;
	};

	//alternative names for the location object
	typedef location loc;
	typedef location box;

	class color //defines an rgb color string, see line 45 for alternative names
	{
	public:
		color(int _r = 0, int _g = 0, int _b = 0) { r = _r; g = _g; b = _b; }
		void set(int _r, int _g, int _b) { r = _r; g = _g; b = _b; }
		color& operator=(const int RGB[3]) //FORMAT { r, g, b }
		{
			r = RGB[0];
			g = RGB[1];
			b = RGB[2];
		}
		color& operator= (const color& ORIGINAL)
		{
			r = ORIGINAL.r;
			g = ORIGINAL.g;
			b = ORIGINAL.b;
			
			return *this;
		}
		bool operator==(color& comp)
		{
			if ((this->r == comp.r) &&
				(this->g == comp.g) &&
				(this->b == comp.b))
			{
				return true;
			}

			return false;
		}

		int r;
		int g;
		int b;
	};

	//alternative names for the color object
	typedef color rgb;

	class resolution
	{
	public:
		resolution(int _height = 768, int _width = 1024) { width = _width; height = _height; }
		void set(int _height, int _width) { width = _width; height = _height; }

		int width;
		int height;
	};

	//alternative names for the resolution object
	typedef resolution res;

	//basic functions
	inline location makeLocation(int x, int y, int width, int height) //LEGACY FUNCTION: Please use assignment operator by passing an array in the format { x, y, width, height } 
	{ loc temp(x, y, width, height); return temp; } 
	inline color makeColor(int r, int g, int b)//LEGACY FUNCTION: Please use assignment operator by passing an array in the format { r, g, b } 
	{ rgb temp(r, g, b); return temp; }

	#endif

}