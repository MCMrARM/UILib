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

#ifndef GUIBUTTON_CPP_
#define GUIBUTTON_CPP_

#include "../UILib.cpp"
#include <android/log.h>
#include <string>

class GUIButton: public GUIElement {
	MCPE_Touch_TButton* real;
	bool pressed;
	int touchId;
	OnClick click;
	
	public:
	
	GUIButton(int id, int x, int y, int width, int height, std::string msg) {
		pressed = false;
		touchId = -1;
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
	
	void press(){
		if(!pressed){
			toggle();
		}
	}
	
	void release(){
		if(pressed){
			toggle();
		}
	}
	
	// touch
	bool pointInside(short x, short y){
		return pointInRectangle(x, y, real->x, real->y, real->width, real->height);
	}
	
	bool touchStarted(int id, short x, short y){
		if(touchId != -1)
			return false;
		
		touchId = id;
		if(id == 0){
			press();
		}
		return true;
	}
	void touchMoved(int id, short x, short y){
		if(id == 0){
			if(pointInside(x, y)){
				press();
			}else{
				release();
			}
		}
	}
	void touchEnded(int id, short x, short y){
		if(id == 0)
			release();
			
		touchId = -1;
		
		if(pointInside(x, y)){
			// clicked
			if(click != NULL) {
				(*click)(real->id);
			}
		}
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
	void setClick(OnClick newClick) { click = newClick; }
	void setMsg(std::string newMsg) { real->msg = newMsg; }
};

#endif
