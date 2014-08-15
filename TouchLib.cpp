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

#ifndef TOUCHLIB_CPP_
#define TOUCHLIB_CPP_

#include <dlfcn.h>
#include <android/log.h>
#include "mcpelauncher.h"

typedef void* MCPE_MouseAction;
typedef void* MCPE_Textures;
typedef void* MCPE_Options;
typedef void* MCPE_AppPlatform;
typedef struct {
	void** vtable;
} MCPE_Screen;

static int (*MCPE_AppPlatform_getScreenWidth)(MCPE_AppPlatform*);

typedef void (*OnTouchStarted)(int, short, short);
typedef void (*OnTouchMoved)(int, short, short);
typedef void (*OnTouchEnded)(int, short, short);

MCPE_Textures* MCPE_Textures_instance;
MCPE_AppPlatform* MCPE_AppPlatform_instance;

OnTouchStarted touchStartedFunc;
OnTouchMoved touchMovedFunc;
OnTouchEnded touchEndedFunc;

void setTouchStarted(OnTouchStarted f){
	touchStartedFunc = f;
}
void setTouchMoved(OnTouchMoved f){
	touchMovedFunc = f;
}
void setTouchEnded(OnTouchEnded f){
	touchEndedFunc = f;
}

int pixelSize = 0;
int screenWidthReal = 0;
int screenWidth = 0;
int screenHeight = 0;

bool ignoreThis = false;
static void (*MCPE_MouseAction_MouseAction)(MCPE_MouseAction*, char, char, short, short, short, short, char);
void mouseActionHook(MCPE_MouseAction* action, char c1, char c2, short x, short y, short s3, short s4, char c3) {
	if(ignoreThis) {
		MCPE_MouseAction_MouseAction(action, c1, c2, x, y, s3, s4, c3);
		ignoreThis = false;
		return;
	}
	ignoreThis = true;

	if((int)c1 == 1 && (int)c2 == 1) {
		// start
		(*touchStartedFunc)(0, x/pixelSize, y/pixelSize);
	}else if((int)c1 == 1 && (int)c2 == 0) {
		// end
		(*touchEndedFunc)(0, x/pixelSize, y/pixelSize);
	}else{
		// move
		(*touchMovedFunc)(0, x/pixelSize, y/pixelSize);
	}

    MCPE_MouseAction_MouseAction(action, c1, c2, x, y, s3, s4, c3);
}

static void (*MCPE_Textures_Textures)(MCPE_Textures*, MCPE_Options*, MCPE_AppPlatform*);
void texturesHook(MCPE_Textures* textures, MCPE_Options* options, MCPE_AppPlatform* platform) {
	MCPE_Textures_instance = textures;
	MCPE_AppPlatform_instance = platform;
	MCPE_Textures_Textures(textures, options, platform);

	screenWidthReal = MCPE_AppPlatform_getScreenWidth(platform);
	if(screenWidth != 0) {
		pixelSize = screenWidthReal/screenWidth;
	}
}

static void (*MCPE_Screen_setSize)(MCPE_Screen*, int, int);
void setSizeHook(MCPE_Screen* s, int width, int height) {
	MCPE_Screen_setSize(s, width, height);
	screenWidth = width;
	screenHeight = height;
	if(screenWidthReal != 0) {
		pixelSize = screenWidthReal/screenWidth;
	}
}

int getScreenWidth() {
	return screenWidth;
}
int getScreenHeight() {
	return screenHeight;
}

void setupTouch() {
	__android_log_print(ANDROID_LOG_INFO, "UILib", "Starting TouchLib by MrARM...\n");

	MCPE_AppPlatform_getScreenWidth = (int (*)(MCPE_AppPlatform*)) dlsym_weak(MCPE_Handle, "_ZN19AppPlatform_android14getScreenWidthEv");

	void* setSize = dlsym(RTLD_DEFAULT, "_ZN6Screen7setSizeEii");
	mcpelauncher_hook(setSize, (void*) &setSizeHook, (void**) &MCPE_Screen_setSize);

	void* mouseaction = dlsym(RTLD_DEFAULT, "_ZN11MouseActionC2Eccssssc");
	mcpelauncher_hook(mouseaction, (void*) &mouseActionHook, (void**) &MCPE_MouseAction_MouseAction);

	void* textures = dlsym(RTLD_DEFAULT, "_ZN8TexturesC1EP7OptionsP11AppPlatform");
	mcpelauncher_hook(textures, (void*) &texturesHook, (void**) &MCPE_Textures_Textures);
}

#endif
