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

#ifndef GUIITEM_CPP_
#define GUIITEM_CPP_

#include "../UILib.cpp"
#include <android/log.h>
#include <string>

class GUIItem: public GUIElement {

	int id;
	int x;
	int y;
	int itmId;
	int itmDmg;
	MCPE_ItemInstance* itm;
	OnClick click;

	public:

	GUIItem(int id, int x, int y, int itemId, int itemDamage) {
		this->id = id;
		this->x = x;
		this->y = y;
		this->itmId = itemId;
		this->itmDmg = itemDamage;

		itm = new MCPE_ItemInstance();
		itm->damage = itemDamage;
		itm->count = 1;
		MCPE_ItemInstance_setId(itm, itemId);
	}

	void render(){
		MCPE_ItemRenderer_renderGuiItem(MCPE_Textures_instance, itm, 0, x, y, 1, 1, 1);
	}

	// touch
	bool pointInside(short x, short y){
		return pointInRectangle(x, y, this->x, this->y, 16, 16);
	}

	bool touchStarted(int id, short x, short y){
		if(click != NULL) {
			(*click)(this->id);
		}
		return true;
	}

	// getters/setters
	int getId() { return id; }
	int getX() { return x; }
	int getY() { return y; }
	int getItemId() { return itmId; }
	int getItemDamage() { return itmDmg; }
	void setId(int newId) { id = newId; }
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setClick(OnClick newClick) { click = newClick; }
	void setItemId(int newId) { itmId = newId; MCPE_ItemInstance_setId(itm, newId); }
	void setItemDamage(int newDmg) { itmDmg = newDmg; itm->damage = newDmg; }
};

#endif
