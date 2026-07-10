#ifndef UI_H
#define UI_H

#include <raylib.h>
#include "timer.h"

typedef enum{
    MESSAGE_INFO,
    MESSAGE_WARNING,
    MESSAGE_ERROR
}MessageType;

typedef struct{
    Vector2 position;
    const char* text;
    Timer timer;
    MessageType type;
    bool active;
}ContextMessage;

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

typedef struct{
    Rectangle boundingBox;
    const char* text;
    bool isHovered;
    bool active;
    void(*callback)();
}Button;

void InitContextMenu(ContextMenu* menu);
void UpdateContextMenu(ContextMenu* menu, Vector2 position,bool mousePressed);
void ShowContextMenu(ContextMenu* menu);
void HideContextMenu(ContextMenu* menu);
void DrawContextMenu(ContextMenu* menu, Vector2 position);
void UnloadContextMenu(ContextMenu* menu);

void AddButtonToContextMenu(ContextMenu* menu, const char* text, void (*callback)());
void UpdateButtonContext(ButtonMenu* button,int buttonIndex, Vector2 position, Vector2 mousePosition, bool mousePressed);
void DrawButtonContext(ButtonMenu* button,Vector2 position);

void InitContextMesage(ContextMessage* message,Vector2 position);
void UpdateContextMessage(ContextMessage* message);
void DrawContextMessage(ContextMessage* message);
void ShowContextMessage(ContextMessage* message, const char* text, int duration, MessageType type, void (*callback)());
void HideContextMessage(ContextMessage* message);

void InitButton(Button* button, const char* text, Vector2 position, void (*callback)());
void UpdateButton(Button* button, Vector2 mousePosition, bool mousePressed);
void DrawButton(Button* button);
void HideButton(Button* button);
void ShowButton(Button* button);

#endif