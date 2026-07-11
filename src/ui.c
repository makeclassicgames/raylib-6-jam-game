#include <stdlib.h>

#include "ui.h"
#include "timer.h"
#include "input.h"


void InitContextMenu(ContextMenu *menu)
{
    menu->buttonCount = 0;
    menu->buttons = NULL;
    menu->active = false;
    TraceLog(LOG_INFO, "Context menu initialized");
}
void UpdateContextMenu(ContextMenu *menu, Vector2 position, bool mousePressed)
{

    if (!menu->active)
    {
        return;
    }
    for (int i = 0; i < menu->buttonCount; i++)
    {
        UpdateButtonContext(&menu->buttons[i], i, position, GetMousePosition(), mousePressed);
    }
}
void ShowContextMenu(ContextMenu *menu)
{
    menu->active = true;
    TraceLog(LOG_INFO, "Context menu shown");
}

void HideContextMenu(ContextMenu *menu)
{
    menu->active = false;
}
void DrawContextMenu(ContextMenu *menu, Vector2 position)
{
    if (!menu->active)
    {
        return;
    }
    Rectangle contextMenuRect = (Rectangle){position.x, position.y, 100, menu->buttonCount * 40};
    DrawRectangleRec(contextMenuRect, GRAY);
    for (int i = 0; i < menu->buttonCount; i++)
    {
        Vector2 buttonPosition = (Vector2){position.x, position.y + i * 40};
        DrawButtonContext(&menu->buttons[i], buttonPosition);
    }
}
void UnloadContextMenu(ContextMenu *menu)
{
}

void AddButtonToContextMenu(ContextMenu *menu, const char *text, void (*callback)())
{
    ButtonMenu *buttonMenu = (ButtonMenu *)malloc(sizeof(ButtonMenu));
    buttonMenu->isHovered = false;
    buttonMenu->isClicked = false;
    buttonMenu->Callback = callback;
    buttonMenu->text = text;
    menu->buttonCount++;

    menu->buttons = (ButtonMenu *)realloc(menu->buttons, menu->buttonCount * sizeof(ButtonMenu));
    menu->buttons[menu->buttonCount - 1] = *buttonMenu;
    TraceLog(LOG_INFO, "Button '%s' added to context menu", text);
}
void UpdateButtonContext(ButtonMenu *button, int buttonIndex, Vector2 position, Vector2 mousePosition, bool mousePressed)
{
    Rectangle buttonBoundingBox = (Rectangle){position.x, position.y + buttonIndex * 40, 80, 40};

    if (CheckCollisionPointRec(mousePosition, buttonBoundingBox))
    {
        button->isHovered = true;
        if (mousePressed)
        {
            button->Callback();
        }
    }
    else
    {
        button->isHovered = false;
    }
}
void DrawButtonContext(ButtonMenu *button, Vector2 position)
{
    Rectangle buttonRect = (Rectangle){position.x, position.y, 90, 30};
    Color color = DARKGRAY;
    if (button->isHovered)
    {
        color = GRAY;
    }
    DrawRectangleRec(buttonRect, color);
    DrawText(button->text, buttonRect.x + 20, buttonRect.y + 5, 20, WHITE);
}

void InitContextMesage(ContextMessage *message, Vector2 position)
{
    message->position = position;
    message->active = false;
}
void UpdateContextMessage(ContextMessage *message){
    if(!message->active){
        return;
    }
    UpdateTimer(&message->timer);
}
void DrawContextMessage(ContextMessage *message)
{
    if (!message->active)
    {
        return;
    }
    DrawRectangle(message->position.x, message->position.y, GetScreenWidth()*0.75, 40, GRAY);
    Color color = WHITE;
    switch (message->type)
    {
    case MESSAGE_INFO:
        color = WHITE;
        break;
    case MESSAGE_WARNING:
        color = YELLOW;
        break;
    case MESSAGE_ERROR:
        color = RED;
        break;
    }
    DrawText(message->text, message->position.x + 10, message->position.y + 10, 20, color);
}
void ShowContextMessage(ContextMessage *message, const char *text, int duration, MessageType type, void (*callback)()){
    
    message->text = text;
    message->type = type;
    message->active = true;
    InitTimer(&message->timer, duration, false, true, callback);
}

void HideContextMessage(ContextMessage *message)
{
    message->active = false;
}

void InitButton(Button* button, const char* text, Vector2 position, void (*callback)()){
    
    button->boundingBox = (Rectangle){position.x, position.y, 100, 40};
    button->text = text;
    button->isHovered = false;
    button->active = true;
    button->callback = callback;
}
void UpdateButton(Button* button, Vector2 mousePosition, bool mousePressed){
    if(!button->active){
        return;
    }
    if(CheckCollisionPointRec(mousePosition, button->boundingBox)){
        button->isHovered = true;
        if(mousePressed){
            button->callback();
        }
    }else{
        button->isHovered = false;
    }
}
void DrawButton(Button* button){
    if(!button->active){
        return;
    }
    Color color = GRAY;
    if(button->isHovered){
        color = DARKGRAY;
        DrawRectangleLines(button->boundingBox.x-2, button->boundingBox.y-2, button->boundingBox.width+4, button->boundingBox.height+4, WHITE);
    }
    DrawRectangleRec(button->boundingBox, color);
    DrawText(button->text, button->boundingBox.x + 10, button->boundingBox.y + 10, 15, WHITE);
}
void HideButton(Button* button){
    button->active = false;
}
void ShowButton(Button* button){
    button->active = true;
}