#include "raylib.h"
#include "sprites.h"

void drawSprite(int sprite, Vector2 drawPosition) {
    // void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
    DrawTextureRec(spritesTexture, (Rectangle){22*sprite, sprite/8, 22.0f, 22.0f }, drawPosition, WHITE);
}