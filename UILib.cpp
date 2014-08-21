/*
 *   _    _ _____ _      _ _
 *  | |  | |_   _| |    |_| |
 *  | |  | | | | | |     _| |__
 *  | |  | | | | | |    | |  _ \
 *  | |__| |_| |_| |____| | |_| |
 *   \____/|_____|______|_|____/  project
 *  http://github.com/MCMrARM/UILib
 *
 *  Made by MrARM
 *  Licensed under GPLv3
 */

#ifndef UILIB_CPP_
#define UILIB_CPP_

void* MCPE_Handle;

#include <dlfcn.h>
#include <android/log.h>
#include <string>
#include <atomic>
#include <vector>
#include "dlsym-weak.cpp"
#include "touchlib.cpp"
#include "mcpelauncher.h"
#include "linker.h"

typedef void* MCPE_Minecraft;
typedef void* MCPE_Gui;
typedef struct {
	void** vtable;
} MCPE_Item;
typedef struct {
	void** vtable;
} MCPE_Tile;
typedef struct {
	int count;
	int damage;
	MCPE_Item* item;
	MCPE_Tile* tile;
	int idk;
} MCPE_ItemInstance;
typedef struct {
	void* vtable; // 4
	int idk1; // 8; always 0? *possibly a pointer*
	void* idk2; // 12
	int x; // 16
	int y; // 20
	int width; // 24
	int height; // 28
	int idk3; // 32; always 0?
	int idk4; // 36; always 0?
	void* idk5; // 40; sometimes null
	std::string msg; // 44
	int id; // 48
} MCPE_Touch_TButton;
typedef struct {
	void* vtable; // 4
	int idk1; // 8; always 0? *possibly a pointer*
	void* idk2; // 12
	int x; // 16
	int y; // 20
	int width; // 24
	int height; // 28
	int idk3; // 32; always 0?
	int idk4; // 36; always 0?
	void* idk5; // 40; sometimes null
	std::string msg; // 44
	int id; // 48
} MCPE_Touch_THeader;
typedef struct {
	void* vtable; // 4
	int idk1; // 8; always 0? *possibly a pointer*
	void* idk2; // 12
	int x; // 16
	int y; // 20
	int width; // 24
	int height; // 28
	int idk3; // 32; always 0?
	int idk4; // 36; always 0?
	void* idk5; // 40; sometimes null
	std::string msg; // 44
	int id; // 48
} MCPE_TextBox;
typedef struct {
	void* vtable; // 4
	int idk1; // 8
	void* idk2; // 12
	int x; // 16
	int y; // 20
	int width; // 24
	int height; // 28
	int idk3; // 32; always 0?
	int idk4; // 36; always 0?
	void* idk5; // 40; sometimes null
	std::string msg; // 44
	void* idk6; // 48
	int color; // 52
	char anything[12]; // 64
} MCPE_Label;

typedef void (*OnClick)(int);
typedef void (*OnTextInput)(void*, int);

// item instance
static void (*MCPE_ItemInstance_setId)(MCPE_ItemInstance*, int);
// screen
void** MCPE_Screen_vtable;
static void (*MCPE_Screen_Screen)(MCPE_Screen*);
static void (*MCPE_Screen_renderGameBehind)(MCPE_Screen*);
static void (*MCPE_Screen_renderBackground)(MCPE_Screen*, int);
static void (*MCPE_Screen_renderDirtBackground)(MCPE_Screen*, int);
static void (*MCPE_Screen_renderMenuBackground)(MCPE_Screen*, float);
static void (*MCPE_Screen_handleBackEvent)(MCPE_Screen*, bool);
static void (*MCPE_Minecraft_setScreen)(MCPE_Minecraft*, MCPE_Screen*);
// button
static void (*MCPE_Touch_TButton_TButton)(MCPE_Touch_TButton*, int, std::string, MCPE_Minecraft*, bool);
static void (*MCPE_Touch_TButton_init)(MCPE_Touch_TButton*, MCPE_Minecraft*);
static void (*MCPE_Button_render)(MCPE_Touch_TButton*, MCPE_Minecraft*, int, int);
static void (*MCPE_Button_setPressed)(MCPE_Touch_TButton*);
// label
static void (*MCPE_Label_Label)(MCPE_Label*, std::string, MCPE_Minecraft*, int, int, int, int, bool);
static void (*MCPE_Label_render)(MCPE_Label*, MCPE_Minecraft*, int, int);
// header
static void (*MCPE_Touch_THeader_THeader)(MCPE_Touch_THeader*, int, std::string);
static void (*MCPE_Touch_THeader_render)(MCPE_Touch_THeader*, MCPE_Minecraft*, int, int);
// item renderer
static void (*MCPE_ItemRenderer_renderGuiItem)(MCPE_Textures*, MCPE_ItemInstance*, int, float, float, float, float, float);

MCPE_Minecraft* MCPE_Minecraft_instance;

class GUIElement {
	public:
	virtual void init() {};
	virtual int getId() { return -1; };
	virtual bool pointInside(short x, short y) { return false; };
	virtual bool touchStarted(int id, short x, short y) { return false; };
	virtual void touchMoved(int id, short x, short y) {};
	virtual void touchEnded(int id, short x, short y) {};
	virtual void render() {};
};

class GUIBaseScreen {
	std::vector<GUIElement*> elements;
	GUIElement** touched;
	bool initialized;

	public:
	GUIBaseScreen() {
		touched = (GUIElement**) malloc(4 * 10);
		for(int i=0;i<10;i++) touched[i] = NULL;
		initialized = false;
	}

	virtual void init() {
		if(initialized == true)
			return;
		initialized = true;
		std::vector<GUIElement*>::iterator iterator = elements.begin();
		std::vector<GUIElement*>::iterator end = elements.end();
		while(iterator < end) {
			GUIElement* el = (GUIElement*) *iterator;
			el->init();
			iterator++;
		}
	}

	void addElement(GUIElement* element) {
		elements.push_back(element);
		if(initialized) {
			element->init();
		}

		std::vector<GUIElement*>::iterator iterator = elements.begin();
		std::vector<GUIElement*>::iterator end = elements.end();
		while(iterator < end) {
			GUIElement* el = (GUIElement*) *iterator;
			iterator++;
		}
	}

	void removeElement(int i) {
		elements.erase(elements.begin()+i);
	}

	void removeElementId(int id) {
		std::vector<GUIElement*>::iterator iterator = elements.begin();
		int i = 0;
		std::vector<GUIElement*>::iterator end = elements.end();
		while(iterator < end) {
			GUIElement* el = (GUIElement*) *iterator;
			if(el->getId() == id) {
				removeElement(i);
				return;
			}
			iterator++;
			i++;
		}
	}

	std::vector<GUIElement*> getElementsArray() {
		return elements;
	}

	virtual void render() {
		if(!initialized) return;
		std::vector<GUIElement*>::iterator iterator = elements.begin();
		std::vector<GUIElement*>::iterator end = elements.end();
		while(iterator < end) {
			GUIElement* el = (GUIElement*) *iterator;
			el->render();
			iterator++;
		}
	}

	virtual bool touchStarted(int id, short x, short y) {
		std::vector<GUIElement*>::iterator iterator = elements.begin();
		std::vector<GUIElement*>::iterator end = elements.end();
		while(iterator < end) {
			GUIElement* el = (GUIElement*) *iterator;
			if(el->pointInside(x, y) && el->touchStarted(id, x, y)) {
				touched[id] = el;
				return true;
			}
			iterator++;
		}
		return false;
	}

	virtual bool touchMoved(int id, short x, short y) {
		if(touched[id] != NULL) {
			touched[id]->touchMoved(id, x, y);
			return true;
		}
		return false;
	}

	virtual bool touchEnded(int id, short x, short y) {
		if(touched[id] != NULL) {
			touched[id]->touchEnded(id, x, y);
			touched[id] = NULL;
			return true;
		}
		return false;
	}
};


GUIBaseScreen* currentScreen = NULL;
bool currentScreenCustom;
GUIBaseScreen* guiScreen = new GUIBaseScreen();
void** customScreenVtable = NULL;

const int BACKGROUND_DIRT = 0;
const int BACKGROUND_GAME = 1;
const int BACKGROUND_GAME_DARK = 2;
const int BACKGROUND_MENU = 3;
class GUIScreen: public GUIBaseScreen {
public:
	MCPE_Screen* screen;
	int bgType;

	void _init() {
		currentScreen->init();
	}

	void _render(int i1, int i2, float f) {
		GUIScreen* vthis = (GUIScreen*) currentScreen;
		if(vthis->bgType == BACKGROUND_DIRT) {
			MCPE_Screen_renderDirtBackground((MCPE_Screen*) this, 0);
		}else if(vthis->bgType == BACKGROUND_MENU) {
			MCPE_Screen_renderMenuBackground((MCPE_Screen*) this, f);
		}else if(vthis->bgType == BACKGROUND_GAME_DARK) {
			MCPE_Screen_renderBackground((MCPE_Screen*) this, 0);
		}

		currentScreen->render();
	}

	/*
	void _renderGameBehind() {
		GUIScreen* vthis = (GUIScreen*) currentScreen;
		if(vthis->bgType == BACKGROUND_GAME || vthis->bgType == BACKGROUND_GAME_DARK) {
			MCPE_Screen_renderGameBehind((MCPE_Screen*) this);
		}
	}
	*/

	void _handleBackEvent(bool b) {
		GUIScreen* vthis = (GUIScreen*) currentScreen;
		if(vthis != NULL && !vthis->handleBackEvent()){
			currentScreen = guiScreen;
			MCPE_Screen_handleBackEvent((MCPE_Screen*) this, b);
		}
	}

	public:
	int i;

	GUIScreen(int bgType):GUIBaseScreen() {
		this->bgType = bgType;
		this->i = 0;

		screen = (MCPE_Screen*) operator new(0x8c);
		MCPE_Screen_Screen(screen);
		screen->vtable = customScreenVtable;

		screen->vtable[2] = (void*) &GUIScreen::_render;
		screen->vtable[3] = (void*) &GUIScreen::_init;
		screen->vtable[10] = (void*) &GUIScreen::_handleBackEvent;
		//screen->vtable[16] = (void*) &GUIScreen::_renderGameBehind; - doesn't work
	}

	void show() {
		MCPE_Minecraft_setScreen(MCPE_Minecraft_instance, screen);
	}

	virtual bool handleBackEvent() {
		return false;
	}
};

static void (*MCPE_Gui_render)(MCPE_Gui*, float, bool, int, int);
void renderGUI(MCPE_Gui* gui, float f, bool b, int i1, int i2) {
	MCPE_Gui_render(gui, f, b, i1, i2);

	if(currentScreen == guiScreen) {
		guiScreen->render();
	}
}

static void (*MCPE_Minecraft_tickInput)(MCPE_Minecraft*);

void minecraft(MCPE_Minecraft* mc) {
	MCPE_Minecraft_tickInput(mc);

	if(MCPE_Minecraft_instance == NULL) {
		//__android_log_print(ANDROID_LOG_INFO, "UILib", "Minecraft instance.\n");
		MCPE_Minecraft_instance = mc;

		guiScreen->init();
		if(currentScreen != NULL) {
			if(currentScreenCustom) {
				currentScreen->init();
				GUIScreen* screen = (GUIScreen*) currentScreen;
				screen->show();
			}
		}
	}
}

void touchStarted(int id, short x, short y) {
	if(currentScreen != NULL) {
		currentScreen->touchStarted(id, x, y);
	}
}
void touchMoved(int id, short x, short y) {
	if(currentScreen != NULL) {
		currentScreen->touchMoved(id, x, y);
	}
}
void touchEnded(int id, short x, short y) {
	if(currentScreen != NULL) {
		currentScreen->touchEnded(id, x, y);
	}
}

bool pointInRectangle(short x, short y, int rectX, int rectY, int rectWidth, int rectHeight) {
	if(rectX <= x && rectY <= y &&
		rectX+rectWidth >= x && rectY+rectHeight >= y)
		return true;
	return false;
}

void setScreenReal(MCPE_Minecraft* mc, MCPE_Screen* screen) {
	if(screen == NULL){
		currentScreen = guiScreen;
		currentScreenCustom = false;
	}else{
		currentScreen = NULL; // we set it directly, not even going though this hook
	}
	MCPE_Minecraft_setScreen(mc, screen);
}

void setupGUI() {
	__android_log_print(ANDROID_LOG_INFO, "UILib", "Starting UILib by MrARM...\n");

	MCPE_Handle = dlopen("libminecraftpe.so", RTLD_LAZY);

	MCPE_ItemInstance_setId = (void (*)(MCPE_ItemInstance*, int)) dlsym(RTLD_DEFAULT, "_ZN12ItemInstance8_setItemEi");

	MCPE_Screen_vtable = (void**) ((int) dlsym(RTLD_DEFAULT, "_ZTV6Screen") + 8);
	customScreenVtable = (void**) malloc(41*4);
	memcpy(customScreenVtable, MCPE_Screen_vtable, 41*4);

	//MCPE_Minecraft_setScreen = (void (*)(MCPE_Minecraft*, MCPE_Screen*)) dlsym(RTLD_DEFAULT, "_ZN9Minecraft9setScreenEP6Screen");
	MCPE_Screen_Screen = (void (*)(MCPE_Screen*)) dlsym_weak(MCPE_Handle, "_ZN6ScreenC1Ev");
	MCPE_Screen_renderGameBehind = (void (*)(MCPE_Screen*)) dlsym(RTLD_DEFAULT, "_ZN6Screen16renderGameBehindEv");
	MCPE_Screen_renderBackground = (void (*)(MCPE_Screen*, int)) dlsym(RTLD_DEFAULT, "_ZN6Screen16renderBackgroundEi");
	MCPE_Screen_renderDirtBackground = (void (*)(MCPE_Screen*, int)) dlsym(RTLD_DEFAULT, "_ZN6Screen20renderDirtBackgroundEi");
	MCPE_Screen_renderMenuBackground = (void (*)(MCPE_Screen*, float)) dlsym(RTLD_DEFAULT, "_ZN6Screen20renderMenuBackgroundEf");
	MCPE_Screen_handleBackEvent = (void (*)(MCPE_Screen*, bool)) dlsym(RTLD_DEFAULT, "_ZN6Screen15handleBackEventEb");

	MCPE_Touch_TButton_TButton = (void (*)(MCPE_Touch_TButton*, int, std::string, MCPE_Minecraft*, bool)) dlsym(RTLD_DEFAULT, "_ZN5Touch7TButtonC1EiRKSsP9Minecraftb");
	MCPE_Touch_TButton_init = (void (*)(MCPE_Touch_TButton*, MCPE_Minecraft*)) dlsym(RTLD_DEFAULT, "_ZN5Touch7TButton4initEP9Minecraft");
	MCPE_Button_render = (void (*)(MCPE_Touch_TButton*, MCPE_Minecraft*, int, int)) dlsym(RTLD_DEFAULT, "_ZN6Button6renderEP9Minecraftii");
	MCPE_Button_setPressed = (void (*)(MCPE_Touch_TButton*)) dlsym(RTLD_DEFAULT, "_ZN6Button10setPressedEv");

	MCPE_Label_Label = (void (*)(MCPE_Label*, std::string, MCPE_Minecraft*, int, int, int, int, bool)) dlsym(RTLD_DEFAULT, "_ZN5LabelC1ESsP9Minecraftiiiib");
	MCPE_Label_render = (void (*)(MCPE_Label*, MCPE_Minecraft*, int, int)) dlsym(RTLD_DEFAULT, "_ZN5Label6renderEP9Minecraftii");

	MCPE_Touch_THeader_THeader = (void (*)(MCPE_Touch_THeader*, int, std::string)) dlsym(RTLD_DEFAULT, "_ZN5Touch7THeaderC2EiRKSs");
	MCPE_Touch_THeader_render = (void (*)(MCPE_Touch_THeader*, MCPE_Minecraft*, int, int)) dlsym(RTLD_DEFAULT, "_ZN5Touch7THeader6renderEP9Minecraftii");

	MCPE_ItemRenderer_renderGuiItem = (void (*)(MCPE_Textures*, MCPE_ItemInstance*, int, float, float, float, float, float)) dlsym(RTLD_DEFAULT, "_ZN12ItemRenderer16renderGuiItemNewEP8TexturesPK12ItemInstanceifffff");

	void* mc = dlsym(RTLD_DEFAULT, "_ZN9Minecraft9tickInputEv");
	mcpelauncher_hook(mc, (void*) &minecraft, (void**) &MCPE_Minecraft_tickInput);

	void* render = dlsym(RTLD_DEFAULT, "_ZN3Gui6renderEfbii");
	mcpelauncher_hook(render, (void*) &renderGUI, (void**) &MCPE_Gui_render);

	void* setScreen = dlsym(RTLD_DEFAULT, "_ZN9Minecraft9setScreenEP6Screen");
	mcpelauncher_hook(setScreen, (void*) &setScreenReal, (void**) &MCPE_Minecraft_setScreen);

	setupTouch();
	setTouchStarted(touchStarted);
	setTouchMoved(touchMoved);
	setTouchEnded(touchEnded);
}

void setScreen(GUIScreen* screen) {
	if(currentScreen == screen)
		return;

	currentScreen = screen;
	currentScreenCustom = true;
	if(screen == NULL) {
		setScreenReal(MCPE_Minecraft_instance, NULL);
		return;
	}

	if(MCPE_Minecraft_instance != NULL) {
		//screen->init();
		screen->show();
	}
}

// GUI is displayed in-game
GUIBaseScreen* getGUIScreen() {
	return guiScreen;
}

#endif
