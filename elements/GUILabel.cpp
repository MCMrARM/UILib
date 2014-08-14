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

#ifndef GUILABEL_CPP_
#define GUILABEL_CPP_

#include "../UILib.cpp"
#include <string>
#include <android/log.h>

class GUILabel: public GUIElement {
	MCPE_Label* real;
	bool shadow;
	
	public:
	
	GUILabel(int x, int y, int width, int color, std::string msg, bool shadow) {
		real = new MCPE_Label();
		real->x = x;
		real->y = y;
		real->width = x+width;
		real->color = color;
		real->msg = msg;
		this->shadow = shadow;
	}
	
	void init() {
		int x = real->x;
		int y = real->y;
		MCPE_Label_Label(real, real->msg, MCPE_Minecraft_instance, real->color, 0, 0, real->width, shadow);
		real->x = x;
		real->y = y;
	}
	
	void render(){
		MCPE_Label_render(real, MCPE_Minecraft_instance, 0, 0); // that x and y doesn't matter
	}
	
	// getters/setters
	int getX() { return real->x; }
	int getY() { return real->y; }
	int getWidth() { return real->width; }
	int getColor() { return real->color; }
	std::string getMsg() { return real->msg; }
	void setX(int newX) { real->x = newX; }
	void setY(int newY) { real->y = newY; }
	void setWidth(int newWidth) { real->width = newWidth; }
	void setColor(int newColor) { real->color = newColor; }
	void setMsg(std::string newMsg) { real->msg = newMsg; }
};

#endif
