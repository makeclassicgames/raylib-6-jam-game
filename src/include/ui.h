#ifndef UI_H
#define UI_H

#include <raylib.h>

typedef struct{
    const char* text;
    bool isHovered;
    bool isClicked;
    void (*Callback)();
}ButtonMenu;

typedef struct{
    ButtonMenu* buttons;
    int buttonCount;
    bool active;

}ContextMenu;

void InitContextMenu(ContextMenu* menu);
void UpdateContextMenu(ContextMenu* menu, Vector2 position,bool mousePressed);
void ShowContextMenu(ContextMenu* menu);
void HideContextMenu(ContextMenu* menu);
void DrawContextMenu(ContextMenu* menu, Vector2 position);
void UnloadContextMenu(ContextMenu* menu);

void AddButtonToContextMenu(ContextMenu* menu, const char* text, void (*callback)());
void UpdateButtonContext(ButtonMenu* button,int buttonIndex, Vector2 position, Vector2 mousePosition, bool mousePressed);
void DrawButtonContext(ButtonMenu* button,Vector2 position);

#endif