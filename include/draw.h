#ifndef DRAW_H
#define DRAW_H

#include "raylib.h"

// 오프셋
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

Vector2 offset_position(Vector2 pos, Vector2 size, int offset); //위치랑 오프셋이 주어졌을 때 좌표를 구하기
void draw_text(const char *text, float x, float y, int font_size, int offset, Color color); // 텍스트 그리기
void draw_circle(float x, float y, float radius, int offset, Color color); // 원 그리기
void draw_rectangle(float x, float y, float width, float height, int offset, Color color); // 사각형 그리기

#endif