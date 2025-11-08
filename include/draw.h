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

//given a position and offset, return the new position
Vector2 offsetPosition(Vector2 pos, Vector2 size, int offset) {
    switch (offset) {
        case TOP_LEFT:
            return pos;
        case TOP_CENTER:
            return (Vector2){pos.x - size.x / 2, pos.y};
        case TOP_RIGHT:
            return (Vector2){pos.x - size.x, pos.y};
        case MIDDLE_LEFT:
            return (Vector2){pos.x, pos.y - size.y / 2};
        case MIDDLE_CENTER:
            return (Vector2){pos.x - size.x / 2, pos.y - size.y / 2};
        case MIDDLE_RIGHT:
            return (Vector2){pos.x - size.x, pos.y - size.y / 2};
        case BOTTOM_LEFT:
            return (Vector2){pos.x, pos.y - size.y};
        case BOTTOM_CENTER:
            return (Vector2){pos.x - size.x / 2, pos.y - size.y};
        case BOTTOM_RIGHT:
            return (Vector2){pos.x - size.x, pos.y - size.y};
    }

    return pos;
}

void Text(const char *text, float x, float y, int font_size, int offset, Color color) {
    Vector2 pos = {x, y};
    Vector2 size = {(float)MeasureText(text, font_size), (float)font_size};

    Vector2 new_pos = offsetPosition(pos, size, offset);
    DrawText(text, new_pos.x, new_pos.y, font_size, color);
}

#endif