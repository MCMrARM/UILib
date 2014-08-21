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

#ifndef GUITOGGLEBUTTON_CPP_
#define GUITOGGLEBUTTON_CPP_

#include "../UILib.cpp"
#include <string>

class GUIToggleButton: public GUIElement {
	MCPE_Touch_TButton* real;
	bool pressed;
	OnClick click;
	
	public:
	
	GUIToggleButton(int id, int x, int y, int width, int height, std::string msg) {
		pressed = false;
		real = (MCPE_Touch_TButton*) operator new(0x3c);
		MCPE_Touch_TButton_TButton(real, id, msg, MCPE_Minecraft_instance, true); // true or false... idk
		real->x = x;
		real->y = y;
		real->width = width;
		real->height = height;
		click = NULL;
	}
	
	void init() {
		MCPE_Touch_TButton_init(real, MCPE_Minecraft_instance);
	}
	
	void render(){
		MCPE_Button_render(real, MCPE_Minecraft_instance, 0, 0); // that x and y doesn't matter
	}
	
	void toggle(){
		MCPE_Button_setPressed(real);
		pressed = !pressed;
	}
	
	// touch
	bool pointInside(short x, short y){
		return pointInRectangle(x, y, real->x, real->y, real->width, real->height);
	}
	
	bool touchStarted(int id, short x, short y){
		toggle();
		if(click != NULL) {
			(*click)(real->id);
		}
		return true;
	}
	void touchMoved(int id, short x, short y){
		//
	}
	void touchEnded(int id, short x, short y){
		//
	}
	
	// getters/setters
	int getId() { return real->id; }
	int getX() { return real->x; }
	int getY() { return real->y; }
	int getWidth() { return real->width; }
	int getHeight() { return real->height; }
	std::string getMsg() { return real->msg; }
	bool getChecked() { return pressed; }
	void setId(int newId) { real->id = newId; }
	void setX(int newX) { real->x = newX; }
	void setY(int newY) { real->y = newY; }
	void setWidth(int newWidth) { real->width = newWidth; }
	void setHeight(int newHeight) { real->height = newHeight; }
	void setClick(OnClick newClick) { click = newClick; }
	void setMsg(std::string newMsg) { real->msg = newMsg; }
	void setChecked(bool check) { if((check && !pressed) || (!check && pressed)) toggle(); }
};

#endif
