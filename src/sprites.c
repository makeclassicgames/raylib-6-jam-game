#include "raylib.h"
#include "sprites.h"

void drawSprite(int sprite, Vector2 drawPosition) {
    DrawTextureRec(spritesTexture, (Rectangle){16*sprite, sprite/8, 16.0f, 16.0f }, drawPosition, WHITE);
}