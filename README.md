UILib
=====

Minecraft: PE User Interface Library

### Usage
1. Include UILib.cpp:
```
  #include "UILib.cpp"
```
2. In your JNI_OnLoad add:
```
  setupGUI();
```

### Example code
1. Custom screen:
```
  GUIScreen* screen = new GUIScreen(BACKGROUND_DIRT);
  GUIButton* testButton = new GUIButton(0, 10, 10, 100, 20, "Test");
  screen->addElement(testButton);
  setScreen(screen);
```
2. In-game button:
```
  GUIButton* testButton = new GUIButton(0, 10, 10, 100, 20, "Test");
  getGUIScreen()->addElement(testButton);
```
3. Set button's action
```
  GUIButton* btn;
  void clicked(int id) {
    btn->setMsg("Clicked!");
  }
  void showButton() {
    btn = new GUIButton(0, 10, 10, 100, 20, "Test");
    btn->setClick(clicked);
    getGUIScreen()->addElement(btn);
  }
```
