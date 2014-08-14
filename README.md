UILib
=====

Minecraft: PE User Interface Library

### Usage
Step 1) Include UILib.cpp:
```
  #include "UILib.cpp"
```
Step 2) In your JNI_OnLoad add:
```
  setupGUI();
```

### Example code
Custom screen:
```
  GUIScreen* screen = new GUIScreen(BACKGROUND_DIRT);
  GUIButton* testButton = new GUIButton(0, 10, 10, 100, 20, "Test");
  screen->addElement(testButton);
  setScreen(screen);
```
In-game button:
```
  GUIButton* testButton = new GUIButton(0, 10, 10, 100, 20, "Test");
  getGUIScreen()->addElement(testButton);
```
Set button's action:
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
