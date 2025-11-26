#ifndef DRAW_H
#define DRAW_H

#include "raylib.h"

enum {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_CENTER,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

Vector2 offset_position(Vector2 pos, Vector2 size, int offset);
void draw_text(const char *text, float x, float y, int font_size, int offset, Color color);
void draw_circle(float x, float y, float radius, int offset, Color color);
void draw_rectangle(float x, float y, float width, float height, int offset, Color color);

#endif