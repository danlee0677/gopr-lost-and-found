#include <stdio.h>
#include "../include/raylib.h"
#include "../include/draw.h"

//given a position and offset, return the new position
Vector2 offset_position(Vector2 pos, Vector2 size, int offset) {
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

void draw_text(const char *text, float x, float y, int font_size, int offset, Color color) {
    Vector2 pos = {x, y};
    Vector2 size = {(float)MeasureText(text, font_size), (float)font_size};

    Vector2 new_pos = offset_position(pos, size, offset);
    DrawText(text, new_pos.x, new_pos.y, font_size, color);
}

void draw_circle(float x, float y, float radius, int offset, Color color) {
    Vector2 pos = {x, y};
    Vector2 size = {radius * 2, radius * 2};
    
    Vector2 new_pos = offset_position(pos, size, offset);
    DrawCircle(new_pos.x, new_pos.y, radius, color);
}

void draw_rectangle(float x, float y, float width, float height, int offset, Color color) {
    Vector2 pos = {x, y};
    Vector2 size = {width, height};
    
    Vector2 new_pos = offset_position(pos, size, offset);
    Rectangle rec = {new_pos.x, new_pos.y, width, height};
    DrawRectangleRoundedLinesEx(rec, 0.02, 0, 8, color);
}