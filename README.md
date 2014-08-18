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
### GUI Elements

#### GUIButton

void GUIButton::GUIButton(int id, int x, int y, int width, int height, std::string msg)  
int GUIButton::getId()  
int GUIButton::getX()  
int GUIButton::getY()  
int GUIButton::getWidth()  
int GUIButton::getHeight()  
std::string GUIButton::getMsg()  
void GUIButton::setId(int newId)  
void GUIButton::setX(int newX)  
void GUIButton::setY(int newY)  
void GUIButton::setWidth(int newWidth)  
void GUIButton::setHeight(int newHeight)  
void GUIButton::setClick(OnClick newClick)  
void GUIButton::setMsg(std::string newMsg)  

#### GUIToggleButton

GUIToggleButton::GUIToggleButton(int id, int x, int y, int width, int height, std::string msg)  
int GUIToggleButton::getId()  
int GUIToggleButton::getX()  
int GUIToggleButton::getY()  
int GUIToggleButton::getWidth()  
int GUIToggleButton::getHeight()  
std::string GUIToggleButton::getMsg()  
bool GUIToggleButton::getChecked()  
void GUIToggleButton::setId(int newId)  
void GUIToggleButton::setX(int newX)  
void GUIToggleButton::setY(int newY)  
void GUIToggleButton::setWidth(int newWidth)  
void GUIToggleButton::setHeight(int newHeight)  
void GUIToggleButton::setClick(OnClick newClick)  
void GUIToggleButton::setMsg(std::string newMsg)  
void GUIToggleButton::setChecked(bool check)  

#### GUIHeader

GUIHeader::GUIHeader(int id, int x, int y, int width, int height, std::string msg)  
[Note]: MCPE's height seems to be 26  
int GUIHeader::getId()  
int GUIHeader::getX()  
int GUIHeader::getY()  
int GUIHeader::getWidth()  
int GUIHeader::getHeight()  
std::string GUIHeader::getMsg()  
void GUIHeader::setId(int newId)  
void GUIHeader::setX(int newX)  
void GUIHeader::setY(int newY)  
void GUIHeader::setWidth(int newWidth)  
void GUIHeader::setHeight(int newHeight)  
void GUIHeader::setMsg(std::string newMsg)  

#### GUILabel

GUILabel::GUILabel(int x, int y, int width, int color, std::string msg, bool shadow)  
int GUILabel::getX()  
int GUILabel::getY()  
int GUILabel::getWidth()  
int GUILabel::getColor()  
std::string GUILabel::getMsg()  
void GUILabel::setX(int newX)  
void GUILabel::setY(int newY)  
void GUILabel::setWidth(int newWidth)  
void GUILabel::setColor(int newColor)  
void GUILabel::setMsg(std::string newMsg)  

#### GUIItem

GUIItem::GUIItem(int id, int x, int y, int itemId, int itemDamage)  
int GUIItem::getId()  
int GUIItem::getX()  
int GUIItem::getY()  
int GUIItem::getItemId()  
int GUIItem::getItemDamage()  
void GUIItem::setId(int newId)  
void GUIItem::setX(int newX)  
void GUIItem::setY(int newY)  
void GUIItem::setItemId(int newId)  
void GUIItem::setItemDamage(int newDmg)  
