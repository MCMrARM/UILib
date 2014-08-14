/*
 *   _    _ _____ _      _ _
 *  | |  | |_   _| |    |_| |
 *  | |  | | | | | |     _| |__
 *  | |  | | | | | |    | |  _ \
 *  | |__| |_| |_| |____| | |_) |
 *   \____/|_____|______|_|____/  project
 *  http://github.com/MCMrARM/UILib
 *
 *  Made by MrARM
 *  Licensed under GPLv3
 */

#ifndef GUIHEADER_CPP_
#define GUIHEADER_CPP_

#include "../UILib.cpp"
#include <android/log.h>
#include <string>

class GUIHeader: public GUIElement {
	MCPE_Touch_THeader* real;

	public:

	GUIHeader(int id, int x, int y, int width, int height, std::string msg) {
		real = (MCPE_Touch_THeader*) operator new(0x38);
		MCPE_Touch_THeader_THeader(real, id, msg);
		real->x = x;
		real->y = y;
		real->width = width;
		real->height = height;
	}

	void render(){
		MCPE_Touch_THeader_render(real, MCPE_Minecraft_instance, 0, 0); // that x and y doesn't matter
	}

	// touch
	bool pointInside(short x, short y){
		return pointInRectangle(x, y, real->x, real->y, real->width, real->height);
	}

	// getters/setters
	int getId() { return real->id; }
	int getX() { return real->x; }
	int getY() { return real->y; }
	int getWidth() { return real->width; }
	int getHeight() { return real->height; }
	std::string getMsg() { return real->msg; }
	void setId(int newId) { real->id = newId; }
	void setX(int newX) { real->x = newX; }
	void setY(int newY) { real->y = newY; }
	void setWidth(int newWidth) { real->width = newWidth; }
	void setHeight(int newHeight) { real->height = newHeight; }
	void setMsg(std::string newMsg) { real->msg = newMsg; }
};

#endif
